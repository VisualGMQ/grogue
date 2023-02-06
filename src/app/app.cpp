#include "app/app.hpp"

void DefaultPlugins::Build(ecs::World* world) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    Mix_Init(MIX_INIT_OGG|MIX_INIT_MP3);
    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);

    world->SetResource(Window("Grogue", 1024, 720));
    world->SetResource(lua::LuaManager());
    world->SetResource(Renderer{*world->GetResource<Window>()});
    world->SetResource(SDL_Event{});
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