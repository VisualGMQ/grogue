#include "app/app.hpp"
#include "game/config.hpp"
#include "game/game.hpp"
#include "game/input.hpp"
#include "game/map.hpp"
#include "game/monster.hpp"
#include "game/signal_defs.hpp"
#include "script/script.hpp"

struct Context {
    FontHandle font;
    bool showDebugInfo = false;
};

void LoadResourceSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& renderer = resources.Get<Renderer>();
    auto& assets = resources.Get<AssetsManager>();
    cmd.SetResource<ImageHandle>(assets.Image().Load("resources/img/role.png"))
        .SetResource<Context>(Context{FontHandle::Null()});

    auto& manager = resources.Get<TilesheetManager>();
    manager.LoadFromConfig("resources/img/tilesheet.lua");
    manager.LoadFromConfig("resources/img/role.lua");
    LOGI("resources loaded");
}

void InitPhysicsSystem(ecs::Resources res) {
    auto& physicWorld = res.Get<physic::PhysicWorld>();
    physicWorld.forceGenerators.push_back(
        [](physic::Particle& p, TimeType) {
            if (p.vel.LengthSqrd() > 0.000001) {
                constexpr float FrictionFactor = 6.0;
                p.force +=
                    -math::Normalize(p.vel) * 1.0 / p.massInv * FrictionFactor;
            }
        });
}

void InitInputSystem(ecs::Commands& cmd, ecs::Resources res) {
#if defined(GROGUE_PLATFORM_ANDROID) || \
    defined(GROGUE_PLATFORM_APPLE)  // Touch devices
    std::unique_ptr<RawInput> input = std::make_unique<TouchInput>();
#else                               // Keyboard devices
    auto& config = res.Get<GameConfig>().GetMiscGameConfig();
    std::unordered_map<std::string, Key> actions;
    for (auto& [key, value] : config.actions) {
        auto sdlKey = SDL_GetScancodeFromName(value.c_str());
        if (sdlKey != SDL_SCANCODE_UNKNOWN) {
            actions[key] = static_cast<Key>(sdlKey);
        } else {
            LOGW("[Config]: your action ", value,
                 "don't correspond to a SDL key. Please check it in "
                 "game_conf.lua");
        }
    }
    std::unique_ptr<RawInput> input = std::make_unique<KeyboardInput>(
        res.Get<Keyboard>(), std::move(actions));
#endif
    cmd.SetResource<Input>(Input(std::move(input)));
}

void ReadConfigSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& luaMgr = resources.Get<AssetsManager>().Lua();
    auto& fontMgr = resources.Get<AssetsManager>().Font();
    auto& tilesheetMgr = resources.Get<TilesheetManager>();
    GameConfig config(luaMgr, fontMgr, tilesheetMgr, "./resources/config/",
                      "./resources/");
    if (!config.Valid()) {
        LOGF("Load config in ./resources/config failed!!! Game can't start!!!");
        exit(1);
    }
    cmd.SetResource<GameConfig>(std::move(config));
    LOGI("config readed");
}

void InitMapSystem(ecs::Commands& cmds, ecs::Resources resources) {
    MapManager mapMgr;
    mapMgr.Add(GenDebugDemo(cmds, resources, 32, 23));

    cmds.SetResource<MapManager>(std::move(mapMgr));
}

void InitMonstersSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& tilesheetMgr = resources.Get<TilesheetManager>();
    auto& tilesheet = tilesheetMgr.Find("role");
    auto& luaMgr = resources.Get<AssetsManager>().Lua();

    auto downTile = tilesheet.Get(0, 0);
    auto rightTile = tilesheet.Get(1, 0);
    auto upTile = tilesheet.Get(2, 0);

    auto monster = Monster::CreateMonster(
        SpriteBundle{Sprite::FromRegion(rightTile.region), rightTile.handle},
        SpriteBundle{Sprite::FromRegion(upTile.region), upTile.handle},
        SpriteBundle{Sprite::FromRegion(downTile.region), downTile.handle});
    cmd.Spawn<Monster, Player, Backpack, physic::Particle, physic::CollideShape,
              Script>(std::move(monster), Player{}, Backpack{},
                      physic::Particle::Create(math::Vector2::Zero, 1.0),
                      physic::CollideShape{std::make_shared<physic::AABB>(
                          math::Vector2{16, 34}, math::Vector2{12, 12})},
                      Script::Create(luaMgr, "resources/script/player.lua"));
}

inline math::Vector2 CalcItemPosInBackpack(int idx, const BackpackUIInfo& config) {
    math::Vector2 pos;
    math::Vector2 slotCoord(idx % config.col, idx / config.col);
    pos.x = config.margin + (config.gridSize + config.padding) * slotCoord.x;
    pos.y = config.margin + (config.gridSize + config.padding) * slotCoord.y;
    return pos;
}

void CompactBackpackUISlotFrom(const std::vector<ecs::Entity>& entities, ecs::Querier querier, int idx, const BackpackUIInfo& config) {
    for (int i = idx; i < entities.size(); i++) {
        auto& transform = querier.Get<ui::RectTransform>(entities[i]);
        auto pos = CalcItemPosInBackpack(i - 1, config);
        transform.transform.localTransform.position = pos;
    }
}

// some backpack ui slot events
void OnGrabByMouse(ecs::Entity entity, ecs::Commands& cmds, ecs::Querier querier,
                       ecs::Resources res, ecs::Events&) {
    auto& grab = res.Get<MouseGrabCache>();
    if (grab.entity) return;

    auto entities = querier.Query<Player>();
    if (entities.empty()) {
        return;
    }
    auto& backpack = querier.Get<Backpack>(entities[0]);
    entities = querier.Query<BackpackUIPanel>();
    if (entities.empty()) {
        return;
    }
    auto backpackUIEntity = entities[0];
    auto& node = querier.Get<Node>(backpackUIEntity);
    auto it = std::find(node.children.begin(), node.children.end(), entity);
    if (it == node.children.end()) {
        LOGE("[BackpackUI]: you clicked a item not in backpack ui panel!");
        return;
    }
    size_t idx = it - node.children.begin();
    grab.entity = *it;
    grab.item = backpack.items[idx];
    grab.isGrabJustnow = true;
    querier.Get<ui::Interaction>(grab.entity.value()).Get(ui::EventType::Click).enable = false;
    CompactBackpackUISlotFrom(
        node.children, querier, idx + 1,
        res.Get<GameConfig>().GetBackpackUIConfig().Info());
    backpack.items.erase(backpack.items.begin() + idx);

    cmds.ChangeHierarchy(backpackUIEntity).Remove(grab.entity.value(), idx);
    cmds.ChangeHierarchy(querier.Query<UIRoot>()[0]).Add(grab.entity.value(), std::nullopt);
}

void UpdateBackpackItemCallback(ecs::Commands& cmds, ecs::Querier querier, ecs::Resources resources,
           ecs::Events&, void* param) {
    auto entities = querier.Query<BackpackUIPanel>();
    if (entities.empty()) return;

    auto entity = entities[0];

    entities = querier.Query<Player>();
    if (entities.empty()) return;
    auto backpackEntity = entities[0];
    auto& backpack = querier.Get<Backpack>(backpackEntity);

    BackpackUIPanelUpdateParam* updateParam = (BackpackUIPanelUpdateParam*)param;

    // uint32_t* newItemCount = (uint32_t*)param;
    uint32_t newItemCount = 1;

    auto& panel = querier.Get<ui::Panel>(entity);
    auto& node = querier.Get<Node>(entity);

    if (updateParam->type == BackpackUIPanelUpdateParam::Type::Pile) {
        int piledIdx = updateParam->value;
        auto& text = querier.Get<ui::Text>(node.children[piledIdx]);
        text.text->SetText(std::to_string(backpack.items[piledIdx].amount));
        return;
    }

    auto& backpackUIConfig =
        resources.Get<GameConfig>().GetBackpackUIConfig().Info();
    // FIXME: we assume newItemCount == 1, will fix this later
    int size = node.children.size() + newItemCount - 1;
    auto slotPos = CalcItemPosInBackpack(size, backpackUIConfig);

    const auto& item = backpack.items.back();
    const auto& itemConfig =
        resources.Get<GameConfig>().GetItemConfig().Items();
    auto it = itemConfig.find(backpack.items.back().nameID);
    if (it == itemConfig.end()) {
        LOGW("item ", item.nameID, " don't in config file");
        return;
    }
    const auto& itemInfo = it->second;

    auto& gameConfig = resources.Get<GameConfig>().GetMiscGameConfig();

    auto& renderer = resources.Get<Renderer>();
    auto& font = resources.Get<AssetsManager>().Font().Get(gameConfig.ui_font);

    node.children.push_back(
        cmds.SpawnImmediateAndReturn<Node, ui::Panel, ui::RectTransform,
                                     ui::Image, ui::Text, ui::Interaction>(
            Node{entity},
            ui::Panel::Create(
                ui::ColorBundle::CreatePureColor(Color{70, 70, 70}),
                ui::ColorBundle::CreatePureColor(Color::Black)),
            ui::RectTransform{NodeTransform{Transform::FromPosition(slotPos)},
                              math::Vector2(backpackUIConfig.gridSize,
                                            backpackUIConfig.gridSize)},
            ui::Image::FromSpriteBundle(itemInfo.sprite),
            ui::Text::Create(std::make_shared<TextTexture>(&renderer, item.amount > 1 ? std::to_string(item.amount) : "", font),
                             ui::ColorBundle::CreatePureColor(Color::White),
                             math::Vector2(0, 16)),
            ui::Interaction::Create(OnGrabByMouse, nullptr, nullptr)));
}

void InitBackpackUISystem(ecs::Commands& cmd, ecs::Resources resources,
                          ecs::Entity uiRootEntity) {
    auto& config = resources.Get<GameConfig>();
    auto& window = resources.Get<Window>();
    auto& backpackUIConfig = config.GetBackpackUIConfig().Info();
    auto& luaMgr = resources.Get<AssetsManager>().Lua();
    auto backpackPanel =
        cmd.SpawnAndReturn<Node, ui::Panel, ui::RectTransform, BackpackUIPanel>(
            Node{},
            ui::Panel::Create(
                ui::ColorBundle::CreatePureColor(Color{200, 200, 200}),
                ui::ColorBundle::CreatePureColor(Color::Black)),
            ui::RectTransform{
                NodeTransform{Transform::FromPosition(
                    {0, window.GetSize().y - backpackUIConfig.height})},
                math::Vector2(backpackUIConfig.width, backpackUIConfig.height)},
            {});

    auto leftHandPanel =
        cmd.SpawnAndReturn<Node, ui::Panel, ui::RectTransform, LeftHandUIPanel>(
            Node{},
            ui::Panel::Create(
                ui::ColorBundle::CreatePureColor(Color{200, 200, 200}),
                ui::ColorBundle::CreatePureColor(Color::Black)),
            ui::RectTransform{
                NodeTransform{Transform::FromPosition(
                    backpackUIConfig.left_hand_position)},
                math::Vector2(backpackUIConfig.gridSize)},
            {});

    auto rightHandPanel =
        cmd.SpawnAndReturn<Node, ui::Panel, ui::RectTransform, RightHandUIPanel>(
            Node{},
            ui::Panel::Create(
                ui::ColorBundle::CreatePureColor(Color{200, 200, 200}),
                ui::ColorBundle::CreatePureColor(Color::Black)),
            ui::RectTransform{
                NodeTransform{Transform::FromPosition(
                    backpackUIConfig.right_hand_position)},
                math::Vector2(backpackUIConfig.gridSize)},
            {});

    cmd.ChangeHierarchy(uiRootEntity).Append({backpackPanel, leftHandPanel, rightHandPanel});

    auto& signalMgr = resources.Get<SignalManager>();
    signalMgr.Regist(SignalCallback::UpdateBackpackItem, UpdateBackpackItemCallback);
}

void StartupSystem(ecs::Commands& cmd, ecs::Resources resources) {
    cmd.SetResource<NearestItemHover>(NearestItemHover{});
    ReadConfigSystem(cmd, resources);
    InitPhysicsSystem(resources);
    InitInputSystem(cmd, resources);
    InitMapSystem(cmd, resources);
    InitMonstersSystem(cmd, resources);
    auto uiRootEntity = cmd.SpawnAndReturn(
        Node{},
        UIRoot{},
        ui::RectTransform{NodeTransform{}, resources.Get<Window>().GetSize()});
    InitBackpackUISystem(cmd, resources, uiRootEntity);
    cmd.SetResource(MouseGrabCache{});
    LOGI("game startup");
}

void DetectNearestItem(ecs::Commands& cmd, ecs::Querier querier,
                       ecs::Resources resources, ecs::Events& events) {
    auto entities = querier.Query<Player>();
    if (entities.empty()) {
        return;
    }

    auto entity = entities[0];
    auto& monster = querier.Get<Monster>(entity);
    auto& particle = querier.Get<physic::Particle>(entity);
    auto& nearestItem = resources.Get<NearestItemHover>();
    auto& itemConfig = resources.Get<GameConfig>().GetItemConfig();

    constexpr float PickupHalfRange = 100;
    auto& map = resources.Get<MapManager>().GetCurrentMap();
    auto monsterPos = particle.pos;
    math::Rect pickupArea = math::Rect::Create(
        monsterPos.x - PickupHalfRange, monsterPos.y - PickupHalfRange,
        PickupHalfRange * 2.0, PickupHalfRange * 2.0);
    pickupArea = math::Rect::Create(
        pickupArea.x / MapTileRealSize, pickupArea.y / MapTileRealSize,
        pickupArea.w / MapTileRealSize, pickupArea.h / MapTileRealSize);
    // FIXME: monster should use it's center position
    int left = std::ceil(pickupArea.x);
    int bottom = std::ceil(pickupArea.y);
    int right =
        std::min<float>(map->tiles.Width(), left + std::floor(pickupArea.w));
    int top =
        std::min<float>(map->tiles.Height(), bottom + std::floor(pickupArea.h));
    left = std::max(0, left);
    bottom = std::max(0, bottom);
    double minDistSqrd = std::numeric_limits<double>::max();
    for (int x = left; x < right; x++) {
        for (int y = bottom; y < top; y++) {
            math::Vector2 tileCenter((x + 0.5) * MapTileRealSize,
                                     (y + 0.5) * MapTileRealSize);
            auto tileEntity = map->tiles.Get(x, y);
            auto& tile = querier.Get<MapTile>(tileEntity);
            double distSqrd = math::LengthSqrd(tileCenter - monsterPos);
            if (!tile.items.empty() && distSqrd < minDistSqrd &&
                distSqrd <= PickupHalfRange * PickupHalfRange &&
                !itemConfig.Items().at(tile.items[0].nameID).architecture) {
                minDistSqrd = distSqrd;
                nearestItem.position.Set(x, y);
                nearestItem.valid = true;
            }
        }
    }

    if (minDistSqrd == std::numeric_limits<double>::max()) {
        nearestItem.valid = false;
    }
}

void DrawNearestItemPointer(ecs::Commands& cmd, ecs::Querier querier,
                            ecs::Resources resources, ecs::Events& events) {
    auto& hover = resources.Get<NearestItemHover>();
    if (!hover.valid) return;

    auto& map = resources.Get<MapManager>().GetCurrentMap();
    auto& renderer = resources.Get<Renderer>();
    if (map->tiles.IsInRange(hover.position.x, hover.position.y)) {
        auto& tilesheet = resources.Get<TilesheetManager>().Find("tilesheet");
        auto tile = tilesheet.Get(4, 10);
        SpriteBundle sprite;
        sprite.image = tile.handle;
        sprite.sprite = Sprite::FromRegion(tile.region);
        sprite.sprite.customSize.Set(MapTileRealSize, MapTileRealSize);
        renderer.DrawSprite(
            sprite, Transform::FromPosition(hover.position * MapTileRealSize));
    }
}

void GrabingItemFollowMouse(ecs::Commands& cmds, ecs::Querier querier,
                     ecs::Resources res, ecs::Events& events) {
    auto& grab = res.Get<MouseGrabCache>();
    auto& mouse = res.Get<Mouse>();
    if (grab.entity && querier.Alive(grab.entity.value())) {
        auto entity = grab.entity.value();
        auto& transform = querier.Get<ui::RectTransform>(entity);
        transform.transform.localTransform.position =
            mouse.Position() -
            transform.size * transform.transform.localTransform.scale / 2.0;

        auto backpackUIEntity = querier.Query<BackpackUIPanel>()[0];
        auto& backpackUI = querier.Get<ui::RectTransform>(backpackUIEntity);

        if (grab.isGrabJustnow) {
            grab.isGrabJustnow = false;
            return;
        }

        if (mouse.LeftBtn().IsPressed() &&
            math::Rect::Create(
                backpackUI.transform.globalTransform.position,
                backpackUI.size * backpackUI.transform.globalTransform.scale)
                .ContainPt(mouse.Position())) {
            auto entities = querier.Query<Player>();
            if (entities.empty()) {
                return;
            }
            auto backpackEntity = entities[0];
            auto& backpack = querier.Get<Backpack>(backpackEntity);
            transform.transform.localTransform.position = CalcItemPosInBackpack(
                backpack.items.size(),
                res.Get<GameConfig>().GetBackpackUIConfig().Info());
            querier.Get<ui::Interaction>(entity)
                .Get(ui::EventType::Click)
                .enable = true;
            PutItemIntoBackpack(backpack, grab.item.value(), cmds, querier, res, events);
            cmds.ChangeHierarchy(querier.Query<UIRoot>()[0]).Remove(grab.entity.value(), std::nullopt);
            cmds.DestroyEntity(grab.entity.value());
            grab.entity = std::nullopt;
            grab.item = std::nullopt;
        }
    }
}

class GameApp final : public App {
public:
    GameApp() {
        auto& world = GetWorld();
        world.AddPlugins<DefaultPlugins>()
            .AddStartupSystem(LoadResourceSystem)
            .AddStartupSystem(StartupSystem)
            .AddSystem(DetectNearestItem)
            .AddSystem(DrawMapSystem)
            .AddSystem(DrawMonsterSystem)
            .AddSystem(DrawNearestItemPointer)
            .AddSystem(GrabingItemFollowMouse)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(GameApp)