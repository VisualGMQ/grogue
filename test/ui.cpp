#include "app/app.hpp"
#include "test_helper.hpp"

void CreateUISystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& fontMgr = resources.Get<AssetsManager>().Font();
    auto& tilesheetMgr = resources.Get<TilesheetManager>();
    auto& renderer = resources.Get<Renderer>();
    auto fontHandle = fontMgr.Load(
        TestHelper::Instance().GetResourcePath() + "PoppkornRegular.ttf", 20);
    auto& font = fontMgr.Get(fontHandle);

    // a label
    cmd.Spawn<Node, ui::RectTransform, ui::Panel, ui::Text>(
        Node{},
        ui::RectTransform{NodeTransform{Transform::FromPosition({100, 200})},
                          math::Vector2(100, 25)},
        ui::Panel::Create(std::nullopt, std::nullopt),
        ui::Text::Create(
            renderer.GenTextTexture("this is label", font),
            ui::ColorBundle::Create(Color::Black, Color::White, Color::Green),
            {}));

    // a button with text
    cmd.Spawn<Node, ui::RectTransform, ui::Panel, ui::Text, ui::Interaction>(
        Node{},
        ui::RectTransform{NodeTransform{Transform::FromPosition({100, 400})},
                          math::Vector2(100, 25)},
        ui::Panel::Create(std::nullopt, std::nullopt),
        ui::Text::Create(renderer.GenTextTexture("button", font),
                         ui::ColorBundle::CreatePureColor(Color::Black), {}),
        ui::Interaction::Create(
            [](ecs::Entity, ecs::Commands&, ecs::Querier, ecs::Resources,
               ecs::Events&) { LOGT("button click"); },
            nullptr, nullptr)
    );

    auto tilesheet = tilesheetMgr.LoadFromConfig(
        TestHelper::Instance().GetResourcePath() + "airman_desc.lua");
    auto tile = tilesheet.Get(0, 0);

    // a button with text and background image
    cmd.Spawn(
        Node{
    },
        ui::RectTransform{NodeTransform{Transform::FromPosition({400, 400})},
                          math::Vector2(100, 50)},
        ui::Panel::Create(
            ui::ColorBundle::Create(Color::White, Color::Green, Color::Blue),
            ui::ColorBundle::CreatePureColor(Color::Black)),
        ui::Image{tile.handle, tile.region,
                  ui::ColorBundle{Color::White, Color::Green, Color::Blue}},
        ui::Interaction::Create(
            [](ecs::Entity, ecs::Commands&, ecs::Querier, ecs::Resources,
               ecs::Events&) { LOGT("button click"); },
            nullptr, nullptr));

    // a panel
    constexpr int PanelWidth = 500;
    constexpr int PanelHeight = 300;
    auto panel = cmd.SpawnAndReturn(
        Node{},
        ui::RectTransform{NodeTransform{Transform::FromPosition({400, 50})},
                          math::Vector2(PanelWidth, PanelHeight)},
        ui::Panel::Create(ui::ColorBundle::CreatePureColor(Color::White),
                          ui::ColorBundle::CreatePureColor(Color::Black)),
        ui::Scrollbar::CreateDefault(),
        ui::Interaction::Create(
            [](ecs::Entity, ecs::Commands&, ecs::Querier, ecs::Resources,
               ecs::Events&) { LOGT("panel click"); },
            nullptr, nullptr));

    constexpr int padding = 10;
    math::Vector2 ImageSize(float(PanelWidth) / tilesheet.Col() - padding,
                            float(PanelHeight) / tilesheet.Row() - padding);

    std::vector<ecs::Entity> children;

    for (int x = 0; x < tilesheet.Col(); x++) {
        for (int y = 0; y < tilesheet.Row(); y++) {
            auto tile = tilesheet.Get(x, y);
            auto entity = cmd.SpawnAndReturn(
                Node{},
                ui::RectTransform{NodeTransform{Transform::FromPosition(
                                      {(ImageSize.x + padding) * x,
                                       (ImageSize.y + padding) * y})},
                                  ImageSize},
                ui::Panel::Create(
                    ui::ColorBundle::Create(Color::White, Color::Green,
                                            Color::Blue),
                    ui::ColorBundle::CreatePureColor(Color::Black)),
                ui::Image{
                    tile.handle, tile.region,
                    ui::ColorBundle{Color::White, Color::Green, Color::Blue}},
                ui::Interaction::Create(
                    [=](ecs::Entity, ecs::Commands&, ecs::Querier,
                        ecs::Resources, ecs::Events&) {
                        LOGT("button ", x, ", ", y, " click");
                    },
                    nullptr, nullptr));
            children.push_back(entity);
        }
    }

    cmd.ChangeHierarchy(panel).Append(children);
}

class Test : public App {
public:
    Test() {
        auto& world = GetWorld();
        world.AddPlugins<DefaultPlugins>()
            .AddStartupSystem(CreateUISystem)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(Test)