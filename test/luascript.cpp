#include "core/engine.hpp"
#include "script/lua_manager.hpp"

class TestLuaScriptLayer: public grogue::core::Layer {
public:
    TestLuaScriptLayer(std::string_view name): grogue::core::Layer(name) {}

    void OnInit() override {
    }

    void Update() override {
        grogue::script::LuaMgr::Update();
    }

    void Render() override {
    }

private:
};

class TestLuaScriptScence: public grogue::core::Scence {
public:
    TestLuaScriptScence(std::string_view name): grogue::core::Scence(name) {
        PushBackLayer<TestLuaScriptLayer>("test_luascript_layer");
    }

    void OnInit() override {
        grogue::script::LuaMgr::Init("test/script.lua");
    }

    bool OnQuit() override {
        grogue::script::LuaMgr::Shutdown();
        return true;
    }
};

int main(int , char**) {
    grogue::core::Engine::Init("grogue", 1024, 720, true);
    grogue::core::Engine::RunScence<TestLuaScriptScence>("TestLuaScriptScence");
}
