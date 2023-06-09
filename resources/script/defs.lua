-- Comments are for lua-lsp hint. Don't remove them!

---@class Resources
---@field get_keyboard fun(): Keyboard get Keyboard resource
---@field get_mouse fun(): Mouse get Mouse resource
---@field get_time fun(): Time get Time resource
---@field get_tilesheet_manager fun(): TilesheetManager get tilesheet manager
---@field get_renderer fun(): Renderer get tilesheet manager
---@field get_signal_manager fun(): SignalManager get tilesheet manager
---@field get_share_context fun(): ShareContext

---@class Commands
---@field spawn fun(): Entity create a entity
---@field destory_entity fun(e: Entity) destroy entity and it's components
---@field add_component fun(e: Entity, component: any) add a component to entity. component can be one of [Node SpriteBundle Panel Transform NodeTransform RectTransform]

---@class Querier
---@field query_transform fun(): table<Entity> 
---@field query_nodetransform fun(): table<Entity> 
---@field query_recttransform fun(): table<Entity> 
---@field query_spritebundle fun(): table<Entity> 
---@field query_panel fun(): table<Entity> 
---@field query_node fun(): table<Entity> 
---@field has_transform fun(e: Entity): boolean
---@field has_nodetransform fun(e: Entity): boolean
---@field has_recttransform fun(e: Entity): boolean
---@field has_spritebundle fun(e: Entity): boolean
---@field has_panel fun(e: Entity): boolean
---@field has_node fun(e: Entity): boolean
---@field get_transform fun(e: Entity): Transform
---@field get_nodetransform fun(e: Entity): NodeTransform
---@field get_recttransform fun(e: Entity): RectTransform
---@field get_spritebundle fun(e: Entity): SpriteBundle
---@field get_panel fun(e: Entity): Panel
---@field get_node fun(e: Entity): Node

---@class Events

---@class Renderer
---@field draw_line fun(p1: Vector2, p2: Vector2)
---@field draw_rect fun(r: Rect)
---@field fill_rect fun(r: Rect)
---@field draw_sprite fun(sprite: SpriteBundle, transform: Transform)
---@field draw_shape fun(shape: Shape, transform: Transform)
---@field draw_text fun(handle: FontHandle, text: string, transform: Transform)
---@field set_cliparea fun(r: Rect)
---@field clear fun()
---@field set_drawcolor fun(c: Color)
---@field get_drawcolor fun(): Color

---@class ShareContext
---@field context table

---@class Shape

---@class Time
---@field set_fps fun(fps: number) set current FPS
---@field get_fps fun(): number get current FPS
---@field elapse fun(): number get time elapse between two frame (in milliseconds)

---@class ImageManager
---@field get fun(handle: ImageHandle): Image get an image from ImageHandle
---@field has fun(handle: ImageHandle): boolean if there has a image
---@field load fun(filename: string): ImageHandle load an image from file
---@field destroy fun(handle: ImageHandle) destroy an image

---@class ImageHandle
---@field is_valid fun(): boolean is handle valid

---@class Image
---@field width fun(): number
---@field height fun(): number
---@field size fun(): Vector2

---@class Vector2
---@field x number
---@field y number
---@field set fun(x: number, y: number)
---@field normalize fun()
---@field dot fun(v: Vector2): number
---@field cross fun(v: Vector2): number
---@field length fun(): number
---@field length_sqrd fun(): number
---@field distance fun(v: Vector2): number
---@field new fun(x: number, y: number): Vector2

---@class Rect
---@field x number
---@field y number
---@field w number
---@field h number
---@field is_intersect fun(r: Rect): boolean
---@field union fun(r: Rect): Rect
---@field contain_pt fun(p: Vector2): boolean
---@field new fun(x: number, y: number, w: number, h: number): Rect  static function for create a Rect

---@class Mouse
---@field position Vector2
---@field offset Vector2
---@field left_btn MouseBtn
---@field middle_btn MouseBtn
---@field right_btn MouseBtn

---@class MouseBtn
---@field is_pressed fun(): boolean
---@field is_pressing fun(): boolean
---@field is_released fun(): boolean
---@field is_releasing fun(): boolean

---@class Keyboard
---@field key function

---@class KeyButton
---@field is_pressed fun(): boolean
---@field is_pressing fun(): boolean
---@field is_released fun(): boolean
---@field is_releasing fun(): boolean

---@class Color
---@field r number must in [0, 255]
---@field g number must in [0, 255]
---@field b number must in [0, 255]
---@field a number must in [0, 255]
---@field new fun(r: number, g: number, b: number, a: number): Color  static function to create Color

---@class FontHandle
---@field is_valid fun(): boolean is handle valid

---@class Transform
---@field position Vector2
---@field rotation number
---@field scale Vector2

---@class RectTransform
---@field transform NodeTransform
---@field size Vector2

---@class NodeTransform
---@field global_transform Transform
---@field local_transform Transform

---@class Node
---@field parent Entity parent node, can be nil
---@field children table<Entity>

---@class Sprite
---@field anchor Vector2
---@field color Color
---@field custom_size Vector2
---@field region Rect

---@class SpriteBundle
---@field flip Flip
---@field image ImageHandle
---@field sprite Sprite
---@field visiable boolean

---@enum Flip
Flip = {
    none = 0,
    horizontal = 0x01,
    vertical = 0x02,
    both = 0x03,
}

---@class ColorBundle
---@field normal Color
---@field press Color
---@field hover Color

---@class Scrollbar
---@field min number
---@field max number
---@field inc_step number
---@field slider_len number
---@field value number
---@field width number

---@class Panel
---@field border_color ColorBundle
---@field content_color ColorBundle
---@field clip_children boolean

---@class SignalManager
---@field remove fun(name: number)  remove signal, number must uint32_t
---@field raise  fun(name: number, c: Commands, q: Querier, r: Resources, e: Events)  raise signal, number must uint32_t
---@field regist fun(name: number, f: fun(c: Commands, q: Querier, r: Resources, e: Events))  regist signal

---@class Entity

---@class Margin
---@field left number
---@field right number
---@field bottom number
---@field top number

---@class Spacing
---@field x number  must unsigned int
---@field y number  must unsigned int

---@class TilesheetManager
---@field create fun(handle: ImageHandle, name: string, col: number, row: number, margin: Margin, spacing: Spacing): Tilesheet
---@field load_from_file fun(filename: string, name: string, col: number, row: number, margin: Margin, spacing: Spacing): Tilesheet
---@field load_from_config fun(filename: string): Tilesheet
---@field find fun(name: string): Tilesheet

---@class Tilesheet
---@field get fun(x: number, y: number): Tile
---@field row fun(): number
---@field col fun(): number
---@field handle fun(): ImageHandle
---@field tile_size fun(): Vector2
---@field is_valid fun(): boolean

---@class Tile
---@field handle ImageHandle
---@field region Rect

---@enum Key
Key = {
    UNKNOWN = 0,
    A = 4,
    B = 5,
    C = 6,
    D = 7,
    E = 8,
    F = 9,
    G = 10,
    H = 11,
    I = 12,
    J = 13,
    K = 14,
    L = 15,
    M = 16,
    N = 17,
    O = 18,
    P = 19,
    Q = 20,
    R = 21,
    S = 22,
    T = 23,
    U = 24,
    V = 25,
    W = 26,
    X = 27,
    Y = 28,
    Z = 29,
    NUM_1 = 30,
    NUM_2 = 31,
    NUM_3 = 32,
    NUM_4 = 33,
    NUM_5 = 34,
    NUM_6 = 35,
    NUM_7 = 36,
    NUM_8 = 37,
    NUM_9 = 38,
    NUM_0 = 39,
    RETURN = 40,
    ESCAPE = 41,
    BACKSPACE = 42,
    TAB = 43,
    SPACE = 44,
    MINUS = 45,
    EQUALS = 46,
    LEFTBRACKET = 47,
    RIGHTBRACKET = 48,
    BACKSLASH = 49,
    NONUSHASH = 50,
    SEMICOLON = 51,
    APOSTROPHE = 52,
    GRAVE = 53,
    COMMA = 54,
    PERIOD = 55,
    SLASH = 56,
    CAPSLOCK = 57,
    F1 = 58,
    F2 = 59,
    F3 = 60,
    F4 = 61,
    F5 = 62,
    F6 = 63,
    F7 = 64,
    F8 = 65,
    F9 = 66,
    F10 = 67,
    F11 = 68,
    F12 = 69,
    PRINTSCREEN = 70,
    SCROLLLOCK = 71,
    PAUSE = 72,
    INSERT = 73,
    HOME = 74,
    PAGEUP = 75,
    DELETE = 76,
    END = 77,
    PAGEDOWN = 78,
    RIGHT = 79,
    LEFT = 80,
    DOWN = 81,
    UP = 82,
    NUMLOCKCLEAR = 83,
    KP_DIVIDE = 84,
    KP_MULTIPLY = 85,
    KP_MINUS = 86,
    KP_PLUS = 87,
    KP_ENTER = 88,
    KP_1 = 89,
    KP_2 = 90,
    KP_3 = 91,
    KP_4 = 92,
    KP_5 = 93,
    KP_6 = 94,
    KP_7 = 95,
    KP_8 = 96,
    KP_9 = 97,
    KP_0 = 98,
    KP_PERIOD = 99,
    NONUSBACKSLASH = 100,
    APPLICATION = 101,
    POWER = 102,
    KP_EQUALS = 103,
    F13 = 104,
    F14 = 105,
    F15 = 106,
    F16 = 107,
    F17 = 108,
    F18 = 109,
    F19 = 110,
    F20 = 111,
    F21 = 112,
    F22 = 113,
    F23 = 114,
    F24 = 115,
    EXECUTE = 116,
    HELP = 117,
    MENU = 118,
    SELECT = 119,
    STOP = 120,
    AGAIN = 121,
    UNDO = 122,
    CUT = 123,
    COPY = 124,
    PASTE = 125,
    FIND = 126,
    MUTE = 127,
    VOLUMEUP = 128,
    VOLUMEDOWN = 129,
    KP_COMMA = 133,
    KP_EQUALSAS400 = 134,
    INTERNATIONAL1 = 135,
    INTERNATIONAL2 = 136,
    INTERNATIONAL3 = 137,
    INTERNATIONAL4 = 138,
    INTERNATIONAL5 = 139,
    INTERNATIONAL6 = 140,
    INTERNATIONAL7 = 141,
    INTERNATIONAL8 = 142,
    INTERNATIONAL9 = 143,
    LANG1 = 144,
    LANG2 = 145,
    LANG3 = 146,
    LANG4 = 147,
    LANG5 = 148,
    LANG6 = 149,
    LANG7 = 150,
    LANG8 = 151,
    LANG9 = 152,
    ALTERASE = 153,
    SYSREQ = 154,
    CANCEL = 155,
    CLEAR = 156,
    PRIOR = 157,
    RETURN2 = 158,
    SEPARATOR = 159,
    OUT = 160,
    OPER = 161,
    CLEARAGAIN = 162,
    CRSEL = 163,
    EXSEL = 164,
    KP_00 = 176,
    KP_000 = 177,
    THOUSANDSSEPARATOR = 178,
    DECIMALSEPARATOR = 179,
    CURRENCYUNIT = 180,
    CURRENCYSUBUNIT = 181,
    KP_LEFTPAREN = 182,
    KP_RIGHTPAREN = 183,
    KP_LEFTBRACE = 184,
    KP_RIGHTBRACE = 185,
    KP_TAB = 186,
    KP_BACKSPACE = 187,
    KP_A = 188,
    KP_B = 189,
    KP_C = 190,
    KP_D = 191,
    KP_E = 192,
    KP_F = 193,
    KP_XOR = 194,
    KP_POWER = 195,
    KP_PERCENT = 196,
    KP_LESS = 197,
    KP_GREATER = 198,
    KP_AMPERSAND = 199,
    KP_DBLAMPERSAND = 200,
    KP_VERTICALBAR = 201,
    KP_DBLVERTICALBAR = 202,
    KP_COLON = 203,
    KP_HASH = 204,
    KP_SPACE = 205,
    KP_AT = 206,
    KP_EXCLAM = 207,
    KP_MEMSTORE = 208,
    KP_MEMRECALL = 209,
    KP_MEMCLEAR = 210,
    KP_MEMADD = 211,
    KP_MEMSUBTRACT = 212,
    KP_MEMMULTIPLY = 213,
    KP_MEMDIVIDE = 214,
    KP_PLUSMINUS = 215,
    KP_CLEAR = 216,
    KP_CLEARENTRY = 217,
    KP_BINARY = 218,
    KP_OCTAL = 219,
    KP_DECIMAL = 220,
    KP_HEXADECIMAL = 221,
    LCTRL = 224,
    LSHIFT = 225,
    LALT = 226,
    LGUI = 227,
    RCTRL = 228,
    RSHIFT = 229,
    RALT = 230,
    RGUI = 231,
    MODE = 257,
    AUDIONEXT = 258,
    AUDIOPREV = 259,
    AUDIOSTOP = 260,
    AUDIOPLAY = 261,
    AUDIOMUTE = 262,
    MEDIASELECT = 263,
    WWW = 264,
    MAIL = 265,
    CALCULATOR = 266,
    COMPUTER = 267,
    AC_SEARCH = 268,
    AC_HOME = 269,
    AC_BACK = 270,
    AC_FORWARD = 271,
    AC_STOP = 272,
    AC_REFRESH = 273,
    AC_BOOKMARKS = 274,
    BRIGHTNESSDOWN = 275,
    BRIGHTNESSUP = 276,
    DISPLAYSWITCH = 277,
    KBDILLUMTOGGLE = 278,
    KBDILLUMDOWN = 279,
    KBDILLUMUP = 280,
    EJECT = 281,
    SLEEP = 282,
    APP1 = 283,
    APP2 = 284,
    AUDIOREWIND = 285,
    AUDIOFASTFORWARD = 286,
    SOFTLEFT = 287,
    SOFTRIGHT = 288,
    CALL = 289,
    ENDCALL = 290,
}