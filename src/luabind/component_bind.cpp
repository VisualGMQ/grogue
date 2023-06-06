#include "luabind/component_bind.hpp"

namespace lua_bind {

void bindMath(LuaScript& script) {
    sol::usertype<math::Vector2> vector2 =
        script.lua.new_usertype<math::Vector2>(
            "Vector2",
            sol::constructors<math::Vector2(), math::Vector2(float, float)>());
    vector2["x"] = &math::Vector2::x;
    vector2["y"] = &math::Vector2::y;
    vector2["set"] = &math::Vector2::Set;
    vector2["normalize"] = &math::Vector2::Normalize;
    vector2["dot"] = &math::Vector2::Dot;
    vector2["cross"] = &math::Vector2::Cross;
    vector2["length"] = &math::Vector2::Length;
    vector2["length_sqrd"] = &math::Vector2::LengthSqrd;
    vector2["distance"] = &math::Vector2::Distance;
    vector2["__add"] = &math::Vector2::operator+;
    vector2["__sub"] = sol::overload(
        static_cast<math::Vector2(math::Vector2::*)() const>(&math::Vector2::operator-),
        static_cast<math::Vector2(math::Vector2::*)(const math::Vector2&) const>(&math::Vector2::operator-));
    vector2["__mul"] = sol::overload(
        static_cast<math::Vector2(math::Vector2::*)(float) const>(&math::Vector2::operator*),
        static_cast<math::Vector2(math::Vector2::*)(const math::Vector2&) const>(&math::Vector2::operator*));
    vector2["__div"] = &math::Vector2::operator/;
    vector2["__eq"] = &math::Vector2::operator==;

    sol::usertype<math::Rect> rect = script.lua.new_usertype<math::Rect>(
        "Rect", sol::constructors<math::Rect(),
                                  math::Rect(float, float, float, float)>());
    rect["x"] = &math::Rect::x;
    rect["y"] = &math::Rect::y;
    rect["w"] = &math::Rect::w;
    rect["h"] = &math::Rect::h;
    rect["union"] = &math::Rect::Union;
    rect["is_intersect"] = &math::Rect::IsIntersect;

    script.lua["rad2deg"] = math::Rad2Deg;
    script.lua["deg2rad"] = math::Deg2Rad;
    script.lua["lerp"] = math::Lerp<float>;
    script.lua["rotate"] = math::Rotate;
    script.lua["scale"] = math::Scale;
    script.lua["traslate"] = math::Translate;
}

void bindColor(LuaScript& script) {
    sol::usertype<Color> color =
        script.lua.new_usertype<Color>(
            "Color",
            sol::constructors<Color, Color(unsigned char, unsigned char, unsigned char, unsigned char)>());
    color["r"] = &Color::r;
    color["g"] = &Color::g;
    color["b"] = &Color::b;
    color["a"] = &Color::a;
}

void bindHandle(LuaScript& script) {
    sol::usertype<ImageHandle> imageHandle =
        script.lua.new_usertype<ImageHandle>("ImageHandle", sol::constructors<ImageHandle()>());
    imageHandle["is_valid"] = &ImageHandle::IsValid;

    sol::usertype<FontHandle> fontHandle =
        script.lua.new_usertype<FontHandle>("FontHandle", sol::constructors<FontHandle()>());
    fontHandle["is_valid"] = &FontHandle::IsValid;
}

void bindTransform(LuaScript& script) {
    sol::usertype<Transform> transform =
        script.lua.new_usertype<Transform>("Transform");
    transform["position"] = &Transform::position;
    transform["rotateion"] = &Transform::rotation;
    transform["scale"] = &Transform::scale;
}

void bindRectTransform(LuaScript& script) {
    sol::usertype<ui::RectTransform> transform =
        script.lua.new_usertype<ui::RectTransform>("RectTransform");
    transform["transform"] = &ui::RectTransform::transform;
    transform["size"] = &ui::RectTransform::size;
}

void bindNodeTransform(LuaScript& script) {
    sol::usertype<NodeTransform> transform =
        script.lua.new_usertype<NodeTransform>("NodeTransform");
    transform["global_transform"] = &NodeTransform::globalTransform;
    transform["local_transform"] = &NodeTransform::localTransform;
}

void bindSprite(LuaScript& script) {
    sol::usertype<Sprite> sprite =
        script.lua.new_usertype<Sprite>("Sprite");
    sprite["anchor"] = &Sprite::anchor;
    sprite["color"] = &Sprite::color;
    sprite["custom_size"] = &Sprite::customSize;
    sprite["region"] = &Sprite::region;
}

void bindSpriteBundle(LuaScript& script) {
    sol::usertype<SpriteBundle> sprite =
        script.lua.new_usertype<SpriteBundle>("SpriteBundle");
    sprite["flip"] = &SpriteBundle::flip;
    sprite["image"] = &SpriteBundle::image;
    sprite["sprite"] = &SpriteBundle::sprite;
    sprite["visiable"] = &SpriteBundle::visiable;
}

void bindTilesheet(LuaScript& script) {
    sol::usertype<Tilesheet> tilesheet =
        script.lua.new_usertype<Tilesheet>("Tilesheet");
    tilesheet["is_valid"] = &Tilesheet::operator bool;
    tilesheet["get"] = sol::overload(
        static_cast<Tile (Tilesheet::*)(uint32_t, uint32_t)>(&Tilesheet::Get),
        static_cast<Tile (Tilesheet::*)(uint32_t)>(&Tilesheet::Get));
    tilesheet["row"] = &Tilesheet::Row;
    tilesheet["col"] = &Tilesheet::Col;
    tilesheet["handle"] = &Tilesheet::Handle;
    tilesheet["tile_size"] = &Tilesheet::TileSize;
}

void bindTile(LuaScript& script) {
    sol::usertype<Tile> tile =
        script.lua.new_usertype<Tile>("Tile");
    tile["region"] = &Tile::region;
    tile["handle"] = &Tile::handle;
}

void bindMargin(LuaScript& script) {
    sol::usertype<Margin> margin =
        script.lua.new_usertype<Margin>("Margin");
    margin["bottom"] = &Margin::bottom;
    margin["top"] = &Margin::top;
    margin["left"] = &Margin::left;
    margin["right"] = &Margin::right;
}

void bindSpacing(LuaScript& script) {
    sol::usertype<Spacing> spacing =
        script.lua.new_usertype<Spacing>("Spacing");
    spacing["x"] = &Spacing::x;
    spacing["y"] = &Spacing::y;
}

void bindImage(LuaScript& script) {
    sol::usertype<Image> image = script.lua.new_usertype<Image>("Image");
    image["width"] = &Image::W;
    image["height"] = &Image::H;
    image["size"] = &Image::Size;
}

void bindUI(LuaScript& script) {
    sol::usertype<ui::ColorBundle> colorBundle =
        script.lua.new_usertype<ui::ColorBundle>("ColorBundle");
    colorBundle["normal"] = &ui::ColorBundle::normal;
    colorBundle["press"] = &ui::ColorBundle::press;
    colorBundle["hover"] = &ui::ColorBundle::hover;

    sol::usertype<ui::Panel> panel =
        script.lua.new_usertype<ui::Panel>("Panel");
    panel["border_color"] = &ui::Panel::borderColor;
    panel["clip_children"] = &ui::Panel::clipChildren;
    panel["content_color"] = &ui::Panel::contentColor;

    sol::usertype<ui::Scrollbar> scrollbar =
        script.lua.new_usertype<ui::Scrollbar>("Scrollbar");
    scrollbar["min"] = &ui::Scrollbar::min;
    scrollbar["max"] = &ui::Scrollbar::max;
    scrollbar["inc_step"] = &ui::Scrollbar::incStep;
    scrollbar["slider_len"] = &ui::Scrollbar::sliderLen;
    scrollbar["value"] = &ui::Scrollbar::value;
    scrollbar["width"] = &ui::Scrollbar::width;
}

void bindNode(LuaScript& script) {
    sol::usertype<Node> node =
        script.lua.new_usertype<Node>("Node");
    node["parent"] = &Node::parent;
    node["children"] = &Node::children;
}

void bindShape(LuaScript& script) {
    sol::usertype<Shape> shape =
        script.lua.new_usertype<Shape>("Shape");
    shape["create_circle"] = &Shape::CreateCircle;
    shape["create_rect"] = &Shape::CreateRect;
}

void BindComponents(LuaScript& script) {
    bindTransform(script);
    bindRectTransform(script);
    bindNodeTransform(script);
    bindSprite(script);
    bindSpriteBundle(script);
    bindUI(script);
    bindTilesheet(script);
    bindTile(script);
    bindMargin(script);
    bindSpacing(script);
    bindNode(script);
    bindShape(script);
}

void BindSimpleTypes(LuaScript& script) {
    bindMath(script);
    bindColor(script);
    bindHandle(script);
}

#define BIND_QUERIER_FUNC(name, type) \
querier["query_" name] = &LuaQuerier::Query ## type; \
querier["has_" name] = &LuaQuerier::Has ## type; \
querier["get_" name] = &LuaQuerier::Get ## type;

void BindLuaQuerier(LuaScript& script) {
    sol::usertype<LuaQuerier> querier =
        script.lua.new_usertype<LuaQuerier>("Querier");
    BIND_QUERIER_FUNC("transform", Transform);
    BIND_QUERIER_FUNC("nodetransform", NodeTransform);
    BIND_QUERIER_FUNC("recttransform", RectTransform);
    BIND_QUERIER_FUNC("spritebundle", SpriteBundle);
    BIND_QUERIER_FUNC("panel", Panel);
    BIND_QUERIER_FUNC("node", Node);
}

#undef BIND_QUERIER_FUNC

}