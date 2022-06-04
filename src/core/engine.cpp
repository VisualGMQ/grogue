#include "core/engine.hpp"

namespace grogue::core {

bool Engine::shouldQuit_ = false;
Size Engine::initWindowSize_;

void Engine::Init(const std::string& title,
                  uint32_t w, uint32_t h,
                  bool resizable) {
    initWindowSize_.Set(w, h);

    Log::Init(LogLevel::Debug);
    Log::CreateConsoleLogger("console");

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        LOG_CRITICAL("SDL initialized failed");
        exit(CORE_SDL_INIT_FAILED);
    }
    LOG_INFO("SDL initialized");

    if (IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) == 0) {
        LOG_CRITICAL("SDL_image initialized failed!");
        SDL_Quit();
        exit(CORE_IMG_INIT_FAILED);
    }
    LOG_INFO("SDL_image initialized");

    if (TTF_Init() == -1) {
        LOG_CRITICAL("SDL_ttf initialized failed");
        exit(CORE_TTF_INIT_FAILED);
    }
    LOG_INFO("SDL_ttf initialized");

    VideoMgr::Init(title.c_str(), w, h, resizable);
    LOG_INFO("VideoMgr initialized");

    GFX::Init(VideoMgr::GetMainVideo()->renderer.get());
    LOG_INFO("GFX init OK");

    Keyboard::Init();
    Mouse::Init();

    initImGui();
    LOG_INFO("imgui initialized");
}

void Engine::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(VideoMgr::GetMainVideo()->window->window_,
                                      VideoMgr::GetMainVideo()->renderer->renderer_);
    ImGui_ImplSDLRenderer_Init(VideoMgr::GetMainVideo()->renderer->renderer_);
}

void Engine::Exit() {
    shouldQuit_ = true;
}

void Engine::cleanUp() {
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    LOG_INFO("imgui shutdown");

    FontMgr::Clear();
    LOG_INFO("free all fonts");

    TextureMgr::Clear();
    LOG_INFO("free all textures");

    GFX::Shutdown();
    LOG_INFO("GFX shutdown");

    VideoMgr::Quit();
    LOG_INFO("VideoMgr quited");

    TTF_Quit();
    LOG_INFO("SDL_ttf quited");

    IMG_Quit();
    LOG_INFO("SDL_image quited");

    SDL_Quit();
    LOG_INFO("SDL quited");
}

bool Engine::ShouldQuit() {
    return shouldQuit_;
}

}
