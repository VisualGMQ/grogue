#include "luabind/resource_bind.hpp"
#include "game/config.hpp"

namespace lua_bind {

::Keyboard& ResourcesWrapper::GetKeyboard() {
    return res_.Get<::Keyboard>();
}

::Mouse& ResourcesWrapper::GetMouse() {
    return res_.Get<::Mouse>();
}

::Time& ResourcesWrapper::GetTime() {
    return res_.Get<::Time>();
}

::TilesheetManager& ResourcesWrapper::GetTilesheetManager() {
    return res_.Get<::TilesheetManager>();
}

::Renderer& ResourcesWrapper::GetRenderer() {
    return res_.Get<::Renderer>();
}

::ImageManager& ResourcesWrapper::GetImageManager() {
    return res_.Get<::ImageManager>();
}

::FontManager & ResourcesWrapper::GetFontManager() {
    return res_.Get<::FontManager>();
}

SignalManagerWrapper ResourcesWrapper::GetSignalManager() {
    return SignalManagerWrapper(res_.Get<::SignalManager>());
}

::LuaShareContext& ResourcesWrapper::GetLuaShareContext() {
    return res_.Get<::LuaShareContext>();
}

void BindResourcesWrapper(::LuaScript& script) {
    sol::usertype<ResourcesWrapper> resources =
        script.lua.new_usertype<ResourcesWrapper>("Resources");
    resources["get_keyboard"] = &ResourcesWrapper::GetKeyboard;
    resources["get_mouse"] = &ResourcesWrapper::GetMouse;
    resources["get_time"] = &ResourcesWrapper::GetTime;
    resources["get_tilesheet_manager"] = &ResourcesWrapper::GetTilesheetManager;
    resources["get_renderer"] = &ResourcesWrapper::GetRenderer;
    resources["get_image_manager"] = &ResourcesWrapper::GetImageManager;
    resources["get_font_manager"] = &ResourcesWrapper::GetFontManager;
    resources["get_signal_manager"] = &ResourcesWrapper::GetSignalManager;
    resources["get_share_context"] = &ResourcesWrapper::GetLuaShareContext;
}

void bindShareContext(::LuaScript& script) {
    sol::usertype<::LuaShareContext> context =
        script.lua.new_usertype<::LuaShareContext>("ShareContext");
    context["context"] = &::LuaShareContext::context;
}

void bindKeyboard(::LuaScript& script) {
    sol::usertype<::Keyboard> keyboard =
        script.lua.new_usertype<::Keyboard>("Keyboard");
    keyboard["key"] = &Keyboard::Key;

    sol::usertype<::KeyButton> keybutton =
        script.lua.new_usertype<::KeyButton>("KeyButton");
    keybutton["is_pressed"] = &KeyButton::IsPressed;
    keybutton["is_pressing"] = &KeyButton::IsPressing;
    keybutton["is_released"] = &KeyButton::IsReleased;
    keybutton["is_releasing"] = &KeyButton::IsReleasing;
}

void bindMouse(::LuaScript& script) {
    sol::usertype<::Mouse> mouse = script.lua.new_usertype<::Mouse>("Mouse");
    mouse["position"] = &Mouse::Position;
    mouse["offset"] = &Mouse::Offset;
    mouse["left_btn"] = &Mouse::LeftBtn;
    mouse["right_btn"] = &Mouse::RightBtn;
    mouse["middle_btn"] = &Mouse::MiddleBtn;

    sol::usertype<::MouseButton> mouseButton =
        script.lua.new_usertype<::MouseButton>("MouseButton");
    mouseButton["is_pressed"] = &MouseButton::IsPressed;
    mouseButton["is_pressing"] = &MouseButton::IsPressing;
    mouseButton["is_released"] = &MouseButton::IsReleased;
    mouseButton["is_releasing"] = &MouseButton::IsReleasing;
}

void bindTime(::LuaScript& script) {
    sol::usertype<::Time> time = script.lua.new_usertype<::Time>("Time");
    time["set_fps"] = &Time::SetFPS;
    time["get_fps"] = &Time::GetFPS;
    time["elapse"] = &Time::Elapse;
}

void bindImageMgr(::LuaScript& script) {
    sol::usertype<::ImageManager> imageMgr =
        script.lua.new_usertype<::ImageManager>("ImageManager");
    imageMgr["get"] = &::ImageManager::Get;
    imageMgr["has"] = &::ImageManager::Has;
    imageMgr["load"] = &::ImageManager::Load;
    imageMgr["destroy"] = &::ImageManager::Destroy;
    imageMgr["create_solitary"] = &::ImageManager::CreateSolitary;
}

void bindFontMgr(::LuaScript& script) {
    sol::usertype<::FontManager> fontMgr =
        script.lua.new_usertype<::FontManager>("FontManager");
    fontMgr["load"] = &::FontManager::Load;
    fontMgr["has"] = &::FontManager::Has;
    fontMgr["get"] = &::FontManager::Get;
    fontMgr["destroy"] = &::FontManager::Destroy;
}

void bindTilesheetMgr(LuaScript& script) {
    sol::usertype<TilesheetManager> mgr =
        script.lua.new_usertype<TilesheetManager>("TilesheetManager");
    mgr["load_from_file"] = &TilesheetManager::LoadFromFile;
    mgr["load_from_config"] = &TilesheetManager::LoadFromConfig;
    mgr["create"] = &TilesheetManager::CreateFromImage;
    mgr["find"] = &TilesheetManager::Find;
}

void bindRenderer(LuaScript& script) {
    sol::usertype<Renderer> renderer =
        script.lua.new_usertype<Renderer>("Renderer");
    renderer["clear"] = &Renderer::Clear;
    renderer["draw_circle"] = &Renderer::DrawCircle;
    renderer["draw_line"] = &Renderer::DrawLine;
    renderer["draw_rect"] = &Renderer::DrawRect;
    renderer["draw_sprite"] = sol::overload(
        static_cast<void (Renderer::*)(const SpriteBundle&, const Transform&)>(
            &Renderer::DrawSprite),
        static_cast<void (Renderer::*)(const SpriteBundleSolitary&,
                                       const Transform&)>(
            &Renderer::DrawSprite));
    renderer["draw_text"] = sol::overload(
        static_cast<void (Renderer::*)(FontHandle, const std::string&,
                                       const Transform&)>(&Renderer::DrawText),
        static_cast<void (Renderer::*)(Font&, const std::string&,
                                       const Transform&)>(&Renderer::DrawText));
    renderer["set_drawcolor"] = &Renderer::SetDrawColor;
    renderer["get_drawcolor"] = &Renderer::GetDrawColor;
    renderer["fill_rect"] = &Renderer::FillRect;
    renderer["draw_shape"] = &Renderer::DrawShape;
    renderer["set_cliparea"] = &Renderer::SetClipArea;
}

void bindSignalManager(::LuaScript& script) {
    sol::usertype<SignalManagerWrapper> signalMgr =
        script.lua.new_usertype<SignalManagerWrapper>("SignalManager");
    signalMgr["raise"] = &SignalManagerWrapper::Raise;
    signalMgr["remove"] = &SignalManagerWrapper::Remove;
    signalMgr["regist"] = &SignalManagerWrapper::Regist;
}

void BindResources(::LuaScript& script) {
    bindShareContext(script);
    bindKeyboard(script);
    bindMouse(script);
    bindTime(script);
    bindTilesheetMgr(script);
    bindRenderer(script);
    bindImageMgr(script);
    bindFontMgr(script);
    bindSignalManager(script);
}

}  // namespace lua_bind