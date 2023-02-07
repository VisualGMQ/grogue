#include "app/app.hpp"

// FIXME  ugly global variable, fix it later
bool ShouldClose = false;

void ExitEventHandle(ecs::Commands& cmd, ecs::Queryer queryer,
                     ecs::Resources resources, ecs::Events& events) {
    auto reader = events.Reader<SDL_QuitEvent>();
    if (reader.Has()) {
        ShouldClose = true;
    }
}

void EventUpdateSystem(ecs::Commands& cmd, ecs::Queryer queryer,
                       ecs::Resources resources, ecs::Events& events) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            events.Writer<SDL_QuitEvent>().Write(event.quit);
        }
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            events.Writer<SDL_KeyboardEvent>().Write(event.key);
        }
        if (event.type == SDL_MOUSEMOTION) {
            events.Writer<SDL_MouseMotionEvent>().Write(event.motion);
        }
        if (event.type == SDL_MOUSEBUTTONDOWN ||
            event.type == SDL_MOUSEBUTTONUP) {
            events.Writer<SDL_MouseButtonEvent>().Write(event.button);
        }
    }
}

void DefaultPlugins::Build(ecs::World* world) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    world->SetResource(Window("Grogue", 1024, 720))
        .SetResource(lua::LuaManager())
        .SetResource(Renderer{*world->GetResource<Window>()})
        .SetResource(SDL_Event{})
        .SetResource(Keyboard{})
        .SetResource(Mouse{})
        .AddSystem(EventUpdateSystem)
        .AddSystem(Keyboard::UpdateSystem)
        .AddSystem(Mouse::UpdateSystem);
}

void DefaultPlugins::Quit(ecs::World* world) {
    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

App::App() {
    world_.AddPlugins(std::unique_ptr<DefaultPlugins>(new DefaultPlugins));
}

void App::Run() {
    world_.AddSystem(ExitEventHandle);
    world_.Startup();
    world_.Update();

    while (!ShouldClose) {
        auto& event = *world_.GetResource<SDL_Event>();
        auto renderer = world_.GetResource<Renderer>();

        renderer->SetDrawColor(Color(50, 50, 50));
        renderer->Clear();

        world_.Update();

        renderer->Present();
    }
}