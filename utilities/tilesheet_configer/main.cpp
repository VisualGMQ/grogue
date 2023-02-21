#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "nuklear.h"
#include "nuklear_sdl_renderer.h"
#include "app/app.hpp"
#include "tinyfiledialogs.h"
#include <filesystem>

// I'm lazy, so not use ecs resources
std::string imgFilename;
float scale = 1;
math::Vector2 offset;

struct TileSheetConfig {
    ImageHandle handle;
    uint32_t col = 1;
    uint32_t row = 1;
    Margin margin = Margin::Zero();
    math::Vector2 spacing = math::Vector2::Zero;
};

void StartupSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& window = resources.Get<Window>();
    auto& renderer = resources.Get<Renderer>();
    auto ctx = nk_sdl_init(window.Raw(), renderer.Raw());

    /* Load Fonts: if none of these are loaded a default font will be used  */
    {
        float font_scale = 1;
        struct nk_font_atlas *atlas;
        struct nk_font_config config = nk_font_config(0);
        struct nk_font *font;

        /* set up the font atlas and add desired font; note that font sizes are
         * multiplied by font_scale to produce better results at higher DPIs */
        nk_sdl_font_stash_begin(&atlas);
        font = nk_font_atlas_add_default(atlas, 13 * font_scale , &config);
        nk_sdl_font_stash_end();

        /* this hack makes the font appear to be scaled down to the desired
         * size and is only necessary when font_scale > 1 */
        font->handle.height /= font_scale;
        /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
        nk_style_set_font(ctx, &font->handle);
    }

    cmd.SetResource<nk_context*>(std::move(ctx));
    cmd.SetResource<TileSheetConfig>(TileSheetConfig{});
}

void UpdateSystem(ecs::Commands& cmd, ecs::Querier, ecs::Resources resources, ecs::Events&) {
    auto& ctx = resources.Get<nk_context*>();
    auto& window = resources.Get<Window>();
    auto& config = resources.Get<TileSheetConfig>();
    auto& assets = resources.Get<AssetsManager>();

    if (nk_begin(ctx, "TileSheet Configer", nk_rect(0, 0, 200, 450),
                 NK_WINDOW_SCALABLE | NK_WINDOW_MOVABLE |
                     NK_WINDOW_MINIMIZABLE | NK_WINDOW_BORDER)) {
        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_button_label(ctx, "Open TileSheet")) {
            std::array filterPatterns = { "*.png", "*.jpg", "*.png", "*.bmp" };
            char* filename = tinyfd_openFileDialog("Open TileSheet", "./", filterPatterns.size(), filterPatterns.data(), "image type", 0);
            if (filename) {
                LOGD("load ", filename);
                scale = 1.0;
                imgFilename = filename;
                std::replace(imgFilename.begin(), imgFilename.end(), '\\', '/');
                if (config.handle) {
                    assets.Image().Destroy(config.handle);
                }
                config = TileSheetConfig{};
                config.handle = assets.Image().Load(filename);
            } else {
                LOGD("load ", filename, " failed");
            }
        }
        if (nk_button_label(ctx, "Save TileSheet") && config.handle) {
            std::array filterPatterns = { "*.lua" };
            std::string filename = tinyfd_saveFileDialog("Save TileSheet", "./", filterPatterns.size(), filterPatterns.data(), ".lua");
            LOGD("save ", filename);
            if (filename != "") {
                std::ofstream file(filename);
                auto imgPureFilename = imgFilename.substr(imgFilename.find_last_of('/') + 1) ;
                file << "description = {" << std::endl
                     << "\tfilename = \"./" << imgPureFilename << "\"," << std::endl
                     << "\trow = " << config.row << "," << std::endl
                     << "\tcol = " << config.col << "," << std::endl
                     << "\tmargin = {" << config.margin.left << ", "
                     << config.margin.right << ", " << config.margin.top << ", "
                     << config.margin.bottom << "}," << std::endl
                     << "\tspacing = {"
                     << static_cast<uint32_t>(config.spacing.x) << ", "
                     << static_cast<uint32_t>(config.spacing.y) << "},"
                     << std::endl
                     << "}";
                
                std::replace(filename.begin(), filename.end(), '\\', '/');
                auto root = filename.substr(0, filename.find_last_of("/") + 1);
                auto des = root + imgPureFilename;
                if (!std::filesystem::exists(des)) {
                    std::filesystem::copy_file(imgFilename, des);
                    LOGW(des, " also moved success!");
                } else {
                    LOGW(des, " already exists!");
                }
            }
        }
        config.col = nk_propertyi(ctx, "Col", 1, config.col, 50, 1, 1);
        config.row = nk_propertyi(ctx, "Row", 1, config.row, 50, 1, 1);

        nk_label(ctx, "Margin:", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
        config.margin.left =
            nk_propertyi(ctx, "Left", 0, config.margin.left, 2000, 1, 1);
        config.margin.right =
            nk_propertyi(ctx, "Right", 0, config.margin.right, 2000, 1, 1);
        config.margin.top =
            nk_propertyi(ctx, "Top", 0, config.margin.top, 2000, 1, 0.1);
        config.margin.bottom =
            nk_propertyi(ctx, "Bottom", 0, config.margin.bottom, 2000, 1, 1);

        nk_label(ctx, "Spacing:", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
        config.spacing.x =
            nk_propertyi(ctx, "x", 0, config.spacing.x, 500, 1, 1);
        config.spacing.y =
            nk_propertyi(ctx, "y", 0, config.spacing.y, 500, 1, 1);
    }
    nk_end(ctx);
}

void NuklearRenderSystem(ecs::Commands& cmd, ecs::Querier,
                         ecs::Resources resources, ecs::Events&) {
    nk_sdl_render(NK_ANTI_ALIASING_ON);
}

void TileSheetRenderSystem(ecs::Commands& cmd, ecs::Querier,
                           ecs::Resources resources, ecs::Events&) {
    auto& renderer = resources.Get<Renderer>();
    auto& config = resources.Get<TileSheetConfig>();
    auto& window = resources.Get<Window>();
    auto& assets = resources.Get<AssetsManager>();

    if (!config.handle) return;

    auto size = assets.Image().Get(config.handle).Size() * scale;
    auto position = (window.GetSize() - size) / 2.0 + offset;

    SpriteBundle bundle;
    bundle.image = config.handle;
    bundle.sprite = Sprite::Default();
    bundle.transform.position = position;
    bundle.transform.scale = {scale, scale};
    renderer.DrawSprite(bundle);

    renderer.SetDrawColor(Color{0, 0, 255});
    renderer.DrawRect(math::Rect{position.x, position.y, size.x, size.y});

    renderer.SetDrawColor(Color{255, 255, 0});
    float left = position.x + config.margin.left;
    float right = position.x + size.x - config.margin.right;
    float top = position.y + config.margin.top;
    float bottom = position.y + size.y - config.margin.bottom;
    renderer.DrawLine({left, 0}, {left, window.GetSize().y});
    renderer.DrawLine({right, 0}, {right, window.GetSize().y});
    renderer.DrawLine({0, top}, {window.GetSize().x, top});
    renderer.DrawLine({0, bottom}, {window.GetSize().x, bottom});

    float tileWidth = (size.x - config.margin.left - config.margin.right -
                       (config.col - 1) * config.spacing.x) /
                      config.col;
    float tileHeight = (size.y - config.margin.top - config.margin.bottom -
                        (config.row - 1) * config.spacing.y) /
                       config.row;

    renderer.SetDrawColor(Color{0, 255, 0});
    for (int i = 1; i < config.col; i++) {
        float x = i * (tileWidth + config.spacing.x) + left;
        renderer.DrawLine({x, position.y - 50}, {x, position.y + size.y + 50});
        x += config.spacing.x;
        renderer.DrawLine({x, position.y - 50}, {x, position.y + size.y + 50});
    }
    renderer.SetDrawColor(Color{255, 0, 0});
    for (int i = 1; i < config.row; i++) {
        float y = i * (tileHeight + config.spacing.y) + top;
        renderer.DrawLine({position.x - 50, y}, {position.x + size.x + 50, y});
        y += config.spacing.y;
        renderer.DrawLine({position.x - 50, y}, {position.x + size.x + 50, y});
    }
}

void EventHandleSystem(ecs::Commands& cmd, ecs::Querier, ecs::Resources resources, ecs::Events& events) {
    auto& ctx = resources.Get<nk_context*>();
    auto& mouse = resources.Get<Mouse>();
    if (nk_item_is_any_active(ctx)) { return; }

    auto& config = resources.Get<TileSheetConfig>();

    if (!config.handle) return;

    auto reader = events.Reader<SDL_MouseWheelEvent>();
    if (reader.Has()) {
        auto event = reader.Read();
        if (event.preciseY > 0) {
            scale *= 1.2;
        } else if (event.preciseY < 0) {
            scale /= 1.2;
            scale = std::max(scale, 0.2f);
        }
    }

    auto motionReader = events.Reader<SDL_MouseMotionEvent>();
    if (motionReader.Has() && mouse.LeftBtn().IsPressing()) {
        offset += mouse.Offset();
    }
}

class Example : public App {
public:
    Example() {
        auto& world = GetWorld();
        world.AddPlugins<DefaultPlugins>()
            .AddStartupSystem(StartupSystem)
            .AddSystem(UpdateSystem)
            .AddSystem(EventHandleSystem)
            .SetResource<ExtraEventHandler>(ExtraEventHandler(
                [&](const SDL_Event& event) {
                    nk_sdl_handle_event(const_cast<SDL_Event*>(&event));
                },
                [](ecs::Resources resources) {
                    nk_input_begin(resources.Get<nk_context*>());
                },
                [](ecs::Resources resources) {
                    nk_input_end(resources.Get<nk_context*>());
                }))
            .AddSystem(TileSheetRenderSystem)
            .AddSystem(NuklearRenderSystem)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(Example)