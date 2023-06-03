#include "app/app.hpp"

void ExitTrigger::DetectExitSystem(ecs::Commands& cmd, ecs::Querier querier,
                                   ecs::Resources resources,
                                   ecs::Events& events) {
    if (!resources.Has<ExitTrigger>()) {
        return;
    }
    auto reader = events.Reader<SDL_QuitEvent>();
    auto& trigger = resources.Get<ExitTrigger>();
    if (reader.Has()) {
        trigger.Exit();
    }
}

void EventUpdateSystem(ecs::Commands& cmd, ecs::Querier querier,
                       ecs::Resources resources, ecs::Events& events) {
    SDL_Event event;

    KeyboardEvents keyboard;
    MouseBtnEvents mouse;

	ExtraEventHandler* handler = nullptr;
    if (resources.Has<ExtraEventHandler>()) {
        handler = &resources.Get<ExtraEventHandler>();
    }

    if (handler) {
        handler->Prepare(resources);
    }

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            events.Writer<QuitEvent>().Write(event.quit);
        }
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            keyboard.events.push_back(event.key);
        }
        if (event.type == SDL_MOUSEMOTION) {
            events.Writer<MouseMotionEvent>().Write(event.motion);
        }
        if (event.type == SDL_MOUSEBUTTONDOWN ||
            event.type == SDL_MOUSEBUTTONUP) {
            mouse.events.push_back(event.button);
        }
        if (event.type == SDL_MOUSEWHEEL) {
            events.Writer<MouseWheelEvent>().Write(event.wheel);
        }

        if (handler) {
            handler->HandleEvent(event);
        }
    }

    if (handler) {
        handler->Finish(resources);
    }

    if (keyboard) {
        events.Writer<KeyboardEvents>().Write(keyboard);
    }
    if (mouse) {
        events.Writer<MouseBtnEvents>().Write(mouse);
    }
}

void DefaultPlugins::Build(ecs::World* world) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    world->SetResource(Window("Grogue", 1024, 720))
        .SetResource(AssetsManager{})
        .SetResource(BGMPlayer{world->GetResource<AssetsManager>()->BGM()})
        .SetResource(Renderer{*world->GetResource<Window>(),
                              world->GetResource<AssetsManager>()->Font()})
        .SetResource(Keyboard{})
        .SetResource(Mouse{})
        .SetResource(ExitTrigger{})
        .SetResource(Time{})
        .SetResource(TimerManager{})
        .SetResource(SignalManager{})
        .SetResource(DebugConfig{});

    auto* assets = world->GetResource<AssetsManager>();
    assets->image_ = std::unique_ptr<ImageManager>(
        new ImageManager(*world->GetResource<Renderer>()));

    world->SetResource(TilesheetManager{assets->Image(), assets->Lua()})
        .AddSystem(EventUpdateSystem)
        .AddSystem(Keyboard::UpdateSystem)
        .AddSystem(Mouse::UpdateSystem)
        .AddSystem(Time::UpdateSystem)
        .AddSystem(TimerManager::UpdateSystem)
        // transform update
        .AddSystem(UpdateNodeTransformSystem)
        .AddSystem(UpdateRectTransformSystem)
        // ui event handle
        .AddSystem(ui::HierarchyUpdateScrollbarSystem)
        .AddSystem(ui::HierarchyHandleUIEventSystem)
        // sprite and shape render
        .AddSystem(RenderSpriteSystem)
        .AddSystem(HierarchyRenderSpriteSystem)
        .AddSystem(RenderShapeSystem)
        .AddSystem(HierarchyRenderShapeSystem)
        // ui render
        .AddSystem(ui::HierarchyRenderPanelSystem)
        .AddSystem(ui::HierarchyRenderLabelSystem)
        // reset render state for next frame
        .AddSystem(ResetRenderStateSystem);
    world->GetResource<Renderer>()->imageManager_ = &assets->Image();
}

void DefaultPlugins::Quit(ecs::World* world) {
    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

void App::Run() {
    Random::Init();
    world_.Startup();
    world_.Update();

    ecs::Resources resources(world_);
    if (!resources.Has<ExitTrigger>()) {
        LOGF("You don't have ExitTrigger resource! Please add DefaultPlugin to "
             "your ECS");
        return;
    }

    auto& exit = resources.Get<ExitTrigger>();
    while (!exit.ShouldExit()) {
        Uint32 fps_start = SDL_GetTicks();
        auto renderer = world_.GetResource<Renderer>();

        renderer->SetDrawColor(Color(50, 50, 50));
        renderer->Clear();

        world_.Update();

        renderer->Present();
    }

    world_.Shutdown();
    Random::Delete();
}