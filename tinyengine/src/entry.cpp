#include "engine/engine.hpp"

constexpr int WindowWidth = 800;
constexpr int WindowHeight = 600;

extern void GameInit(void);

int main(int argc, char** argv) {
    engine::Logger::Init();

    auto& config = engine::InitConfig::Instance();

    engine::Random::Init(engine::Random::CPP, std::time(nullptr));
    engine::ECS::Init();
    engine::Video::Init();
    engine::Event::Init();
    engine::Renderer::Init();
    engine::FontFactory::Init();
    engine::TextureFactory::Init();
    engine::UI::Init();
    engine::Input::Init();
    engine::Sound::Init();
    engine::SoundFactory::Init();
    engine::TimerFactory::Init();
    engine::World::Init();
    engine::SceneMgr::Init();

    GameInit();

    engine::Renderer::SetClearColor(engine::Color(100, 100, 100, 255));
    while (!engine::Video::ShouldClose()) {
        engine::World::Instance().TryInitEntities();
        engine::World::Instance().Update();
        engine::UI::Update();
        engine::Timer::UpdateElapse();
        engine::Input::UpdateStates();
        engine::SceneMgr::QuitOldScene();
        engine::World::Instance().CleanUp();
    }

    engine::SceneMgr::Quit();
    engine::World::Quit();
    engine::TimerFactory::Quit();
    engine::SoundFactory::Quit();
    engine::Sound::Quit();
    engine::Input::Quit();
    engine::UI::Quit();
    engine::TextureFactory::Quit();
    engine::FontFactory::Quit();
    engine::Renderer::Quit();
    engine::Event::Quit();
    engine::Video::Quit();
    engine::ECS::Quit();
    engine::Logger::Quit();
    return 0;
}
