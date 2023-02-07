#include "app/app.hpp"

void StartupSystem(ecs::Commands& cmd, ecs::Resources resources) {
    auto& renderer = resources.Get<Renderer>();
    cmd.SetResource<ImageHandle>(renderer.LoadImage("resources/img/role.png"));
}

void UpdateSystem(ecs::Commands& cmd, ecs::Queryer queryer, ecs::Resources resources, ecs::Events&) {
    if (resources.Has<Renderer>()) {
        auto& renderer = resources.Get<Renderer>();
        renderer.SetDrawColor(Color{0, 255, 0});
        renderer.DrawLine({100, 200}, {400, 500});
        renderer.SetDrawColor(Color{255, 255, 0});
        renderer.DrawRect(math::Rect{100, 200, 400, 500});

        auto& handle = resources.Get<ImageHandle>();
        auto img = renderer.GetImage(handle);
        renderer.DrawImage(
            *img,
            {0, 0, static_cast<float>(img->W()), static_cast<float>(img->H())},
            {0, 0, static_cast<float>(img->W()), static_cast<float>(img->H())});
    }
}

void DefaultPlugins::Build(ecs::World* world) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    Mix_Init(MIX_INIT_OGG|MIX_INIT_MP3);
    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);

    world->SetResource(Window("Grogue", 1024, 720))
          .SetResource(lua::LuaManager())
          .SetResource(Renderer{*world->GetResource<Window>()})
          .SetResource(SDL_Event{})
          .AddSystem(UpdateSystem)
          .AddStartupSystem(StartupSystem);
}

void DefaultPlugins::Quit(ecs::World* world) {
    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

void App::Run() {
    world_.AddPlugins(std::unique_ptr<DefaultPlugins>(new DefaultPlugins));
    world_.Startup();
    world_.Update();

    while (!shouldClose_) {
        auto& event = *world_.GetResource<SDL_Event>();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                shouldClose_ = true;
            }
            // TODO dispatch event
        }

        auto renderer = world_.GetResource<Renderer>();

        renderer->SetDrawColor(Color(50, 50, 50));
        renderer->Clear();

        world_.Update();

        renderer->Present();
    }
}