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
}

void UpdateSystem(ecs::Commands& cmd, ecs::Queryer, ecs::Resources resources, ecs::Events&) {
    auto& ctx = resources.Get<nk_context*>();
    auto& window = resources.Get<Window>();

    constexpr int EditorHeight = 200;
    constexpr int TimelineHeight = 50;

    if (nk_begin(ctx, "editor",
                 nk_rect(0, window.GetSize().y - EditorHeight,
                         window.GetSize().x, EditorHeight), NK_WINDOW_BORDER)) {

        nk_layout_row_template_begin(ctx, TimelineHeight);
        nk_layout_row_template_push_static(ctx, 100);
        nk_layout_row_template_push_dynamic(ctx);
        nk_layout_row_template_end(ctx);

        if (nk_button_label(ctx, "Add Track")) {
            // TODO  add a tracker
        }
        nk_button_label(ctx, "button");


        nk_layout_row_dynamic(ctx, EditorHeight - TimelineHeight, 1);
        if (nk_group_begin(ctx, "content", NK_WINDOW_SCROLL_AUTO_HIDE)) {
            nk_layout_row_template_begin(ctx, EditorHeight - TimelineHeight);
            nk_layout_row_template_push_static(ctx, 25);
            nk_layout_row_template_push_dynamic(ctx);
            nk_layout_row_template_end(ctx);

            char buf[1024] = {0};
            int len;
            nk_edit_string(ctx, NK_EDIT_DEFAULT, buf, &len, sizeof(buf), nk_filter_default);
            nk_edit_string(ctx, NK_EDIT_DEFAULT, buf, &len, sizeof(buf), nk_filter_default);
            nk_command_buffer* cmd = nk_window_get_canvas(ctx);
            auto region = nk_window_get_content_region(ctx);
            nk_fill_rect(cmd, region, 1, nk_rgb(255, 0, 0));
        } nk_group_end(ctx);
    } nk_end(ctx);
}

void NuklearRenderSystem(ecs::Commands& cmd, ecs::Queryer,
                         ecs::Resources resources, ecs::Events&) {
    nk_sdl_render(NK_ANTI_ALIASING_ON);
}

void EventHandleSystem(ecs::Commands& cmd, ecs::Queryer, ecs::Resources resources, ecs::Events& events) {
    auto& ctx = resources.Get<nk_context*>();
    auto& mouse = resources.Get<Mouse>();
    if (nk_item_is_any_active(ctx)) { return; }

    // TODO your event handle
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
            .AddSystem(NuklearRenderSystem)
            .AddSystem(ExitTrigger::DetectExitSystem);
    }
};

RUN_APP(Example)
