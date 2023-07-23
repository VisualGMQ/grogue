-- some wrapper for C++ objects and function, aime to give some hint to lua

---@alias ComponentID number
---@alias Entity number
---@alias CommandsWrapper userdata
---@alias ResourcesWrapper userdata
---@alias QuerierWrapper userdata
---@alias EventsWrapper userdata
---@alias ResourceEnum userdata
---@alias ComponentEnum userdata
---@alias vector userdata

gamelib = {}

---@enum Resource
gamelib.Resource = {
    LuaShareContext  = ResourceEnum.LuaShareContext,
    ExitTrigger      = ResourceEnum.ExitTrigger,
    AssetsManager    = ResourceEnum.AssetsManager,
    FontManager      = ResourceEnum.FontManager,
    ImageManager     = ResourceEnum.ImageManager,
    Keyboard         = ResourceEnum.Keyboard,
    Mouse            = ResourceEnum.Mouse,
    LuaManager       = ResourceEnum.LuaManager,
    PhysicWorld      = ResourceEnum.PhysicWorld,
    Renderer         = ResourceEnum.Renderer,
    BGMManager       = ResourceEnum.BGMManager,
    DebugConfig      = ResourceEnum.DebugConfig,
    TilesheetManager = ResourceEnum.TilesheetManager,
    Time             = ResourceEnum.Time,
    TimerManager     = ResourceEnum.TimerManager,
    Window           = ResourceEnum.Window,
    GameConfig       = ResourceEnum.GameConfig,
    NearestItemHover = ResourceEnum.NearestItemHover,
    Input            = ResourceEnum.Input,
    MapManager       = ResourceEnum.MapManager,
    MouseGrabCache   = ResourceEnum.MouseGrabCache,
}

---@enum Component
gamelib.Component = {
    Particle            = ComponentEnum.Particle,
    RigidBody           = ComponentEnum.RigidBody,
    CollideShape        = ComponentEnum.CollideShape,
    NodeTransform       = ComponentEnum.NodeTransform,
    Sprite              = ComponentEnum.Sprite,
    SpriteBundle        = ComponentEnum.SpriteBundle,
    Transform           = ComponentEnum.Transform,
    MapTile             = ComponentEnum.MapTile,
    Monster             = ComponentEnum.Monster,
    Backpack            = ComponentEnum.Backpack,
    UIRoot              = ComponentEnum.UIRoot,
    BackpackUIPanel     = ComponentEnum.BackpackUIPanel,
    LeftHandUIPanel     = ComponentEnum.LeftHandUIPanel,
    RightHandUIPanel    = ComponentEnum.RightHandUIPanel,
    Player              = ComponentEnum.Player,
}

---@class Commands
---@field Raw fun(self): CommandsWrapper
---@field Spawn fun(self): Entity
---@field DestroyEntity fun(self, Entity)
---@field DestroyComponent fun(self, Entity, ComponentID)
---@field private SetResource fun(self, ComponentID, any)
---@field RemoveResource fun(self, ComponentID)
---@field AddCppComponent fun(self, entity, Particle)
---@field AddCppComponent fun(self, entity, RigidBody)
---@field AddCppComponent fun(self, entity, CollideShape)
---@field AddCppComponent fun(self, entity, NodeTransform)
---@field AddCppComponent fun(self, entity, Sprite)
---@field AddCppComponent fun(self, entity, SpriteBundle)
---@field AddCppComponent fun(self, entity, Transform)
---@field AddCppComponent fun(self, entity, MapTile)
---@field AddCppComponent fun(self, entity, Monster)
---@field AddCppComponent fun(self, entity, Backpack)
---@field AddCppComponent fun(self, entity, UIRoot)
---@field AddCppComponent fun(self, entity, BackpackUIPanel)
---@field AddCppComponent fun(self, entity, LeftHandUIPanel)
---@field AddCppComponent fun(self, entity, RightHandUIPanel)
---@field AddCppComponent fun(self, entity, Player)
---@field AddLuaComponent fun(self, entity: Entity, id: ComponentID, comp: table)
gamelib.CommandsWrapper = CommandsWrapper
---@param res any
function gamelib.CommandsWrapper:SetRes(res)
    self:SetResource(res.GetComponentID(), res)
end
---@param comp any
function gamelib.CommandsWrapper:AddComponent(entity, comp)
    if comp._component_id < class.LuaComponentStartIdx then
        self:AddCppComponent(entity, comp)
    else
        self:AddLuaComponent(entity, comp._component_id, comp)
    end
end

---@class Resources
---@field Raw fun(self): ResourcesWrapper
---@field Has fun(self, ComponentID): boolean
---@field Get fun(self, ComponentID): any
gamelib.Resources = ResourcesWrapper

---@class Querier
---@field Raw fun(self): QuerierWrapper
---@field Query fun(self, ComponentID): vector<Entity>
---@field Has fun(self, Entity, ComponentID): boolean
---@field Get fun(self, Entity, ComponentID): any
gamelib.Querier = QuerierWrapper

---@class Events
---@field Raw fun(self): EventsWrapper
gamelib.Events = EventsWrapper


---@class Tilesheet
---@field Get fun(self, col: number, row: number):Tile give (x, y) to get tile
---@field Get fun(self, index: number):Tile give index to get tile
---@field Row fun(self):number
---@field Col fun(self):number
---@field Handle fun(self):ImageHandle
---@field TileSize fun(self):Vector2
gamelib.Tilesheet = Tilesheet

---@class Tile
---@field region Rect
---@field handle ImageHandle
gamelib.Tile = Tile

---@class TilesheetManager
---@field CreateFromImage fun(self, handle: ImageHandle, name: string, col: number, row: number, margin: Margin, spacing: Spacing): Tilesheet
---@field LoadFromFile fun(self, filename: string, name: string, col: number, row: number, margin: Margin, spacing: Spacing): Tilesheet
---@field LoadFromConfig fun(self, filename: string): Tilesheet load from config file
---@field Find fun(self, name: string): Tilesheet
gamelib.TilesheetManager = TilesheetManager
gamelib.TilesheetManager._component_id = gamelib.Resource.TilesheetManager

---@class Rect
---@field x number
---@field y number
---@field w number
---@field h number
gamelib.Rect = Rect

---@class Sprite
---@field color Color
---@field region Rect
---@field customSize Vector2
---@field anchor Vector2
---@field Default fun(): Sprite static function
---@field FromRegion fun(region: Rect): Sprite
---@field FromCustomSize fun(size: Vector2): Sprite
gamelib.Sprite = Sprite

---@class SpriteBundle
---@field sprite Sprite
---@field image ImageHandle
---@field flip Flip
---@field visiable boolean
---@field new fun(): SpriteBundle
gamelib.SpriteBundle = SpriteBundle
gamelib.SpriteBundle._component_id = gamelib.Component.SpriteBundle

---@class SpriteBundleSolitary
---@field sprite Sprite
---@field flip Flip
---@field visiable boolean
gamelib.SpriteBundleSolitary = SpriteBundleSolitary

---@enum Flip
gamelib.Flip = {
    None = Flip.None,
    Horizontal  = Flip.Horizontal,
    Vertical = Flip.Vertical,
    Both = Flip.Both,
}

---@class Color
---@field r number [0, 255]
---@field g number [0, 255]
---@field b number [0, 255]
---@field a number [0, 255]
---@field new fun(r: number, g: number, b: number, a: number): Color
gamelib.Color = Color

---@class Image
---@field W fun(self): number
---@field H fun(self): number
---@field Size fun(self): Vector2
gamelib.Image = Image

---@class Transform
---@field position Vector2
---@field rotation number
---@field scale Vector2
---@field Create fun(position: Vector2, rotation: number, scale: Vector2): Transform
---@field FromPosition fun(position: Vector2): Transform
---@field FromRotation fun(rotation: number): Transform
---@field FromScale fun(scale: Vector2): Transform
gamelib.Transform = Transform
gamelib.Transform._component_id = gamelib.Component.Transform

---@class AssetsManager
---@field Image fun(self): ImageManager
---@field Font fun(self): FontManager
---@field Lua fun(self): LuaManager
---@field BGM fun(self): BGMManager
gamelib.AssetsManager = AssetsManager
gamelib.AssetsManager._component_id = gamelib.Resource.AssetsManager

---@class ImageManager
---@field Load fun(self, filename: string): ImageHandle
gamelib.ImageManager = ImageManager
gamelib.ImageManager._component_id = gamelib.Resource.ImageManager

---@class FontManager
---@field Load fun(self, filename: string, ptsize: number): FontHandle
gamelib.FontManager = FontManager
gamelib.FontManager._component_id = gamelib.Resource.FontManager

---@class LuaManager
---@field Load fun(self, filename: string): LuaScriptHandle
---@field Create fun(self): LuaScriptHandle
---@field CreateSolitary fun(self): LuaScriptHandle
---@field CreateSolitary fun(self, filename: string): LuaScriptHandle
---@field CreateSolitaryFromContent fun(self, code: string): LuaScriptHandle
gamelib.LuaManager = LuaManager
gamelib.LuaManager._component_id = gamelib.Resource.LuaManager

gamelib.physics = {}

---@class PhysicWorld
---@field grid Grid
---@field forceGenerators vector<ForceGenerator>
---@field manifolds vector<Manifold>
gamelib.physics.PhysicWorld = PhysicWorld
gamelib.physics.PhysicWorld._component_id = gamelib.Resource.PhysicWorld

---@class Grid
gamelib.physics.Grid = Grid

---@alias ForceGenerator fun(Particle, TimeType)

---@class Particle
---@field massInv number
---@field pos Vector2
---@field vel Vector2
---@field acc Vector2
---@field force Vector2
gamelib.physics.Particle = Particle
gamelib.physics.Particle._component_id = gamelib.Component.Particle

---@class RigidBody
---@field particle Particle
---@field rotation number
---@field angVel number
---@field new fun(): RigidBody
gamelib.physics.RigidBody = RigidBody
gamelib.physics.RigidBody._component_id = gamelib.Component.RigidBody

---@class Shape
---@field center Vector2
---@field offset  Vector2
gamelib.physics.Shape = Shape

---@class AABB: Shape
---@field halfLen Vector2
---@field new fun(): AABB
---@field new fun(offset: Vector2, halfLen: Vector2): AABB
gamelib.physics.AABB = AABB

---@class Circle: Shape
---@field radius number
---@field new fun(): Circle
---@field new fun(offset: Vector2, radius: number): Circle
gamelib.physics.Circle = Circle

---@enum Key
gamelib.Key = {
   Unknown              = Key.KEY_UNKNOWN,
   A                    = Key.KEY_A,
   B                    = Key.KEY_B,
   C                    = Key.KEY_C,
   D                    = Key.KEY_D,
   E                    = Key.KEY_E,
   F                    = Key.KEY_F,
   G                    = Key.KEY_G,
   H                    = Key.KEY_H,
   I                    = Key.KEY_I,
   J                    = Key.KEY_J,
   K                    = Key.KEY_K,
   L                    = Key.KEY_L,
   M                    = Key.KEY_M,
   N                    = Key.KEY_N,
   O                    = Key.KEY_O,
   P                    = Key.KEY_P,
   Q                    = Key.KEY_Q,
   R                    = Key.KEY_R,
   S                    = Key.KEY_S,
   T                    = Key.KEY_T,
   U                    = Key.KEY_U,
   V                    = Key.KEY_V,
   W                    = Key.KEY_W,
   X                    = Key.KEY_X,
   Y                    = Key.KEY_Y,
   Z                    = Key.KEY_Z,
   Num1                 = Key.KEY_NUM_1,
   Num2                 = Key.KEY_NUM_2,
   Num3                 = Key.KEY_NUM_3,
   Num4                 = Key.KEY_NUM_4,
   Num5                 = Key.KEY_NUM_5,
   Num6                 = Key.KEY_NUM_6,
   Num7                 = Key.KEY_NUM_7,
   Num8                 = Key.KEY_NUM_8,
   Num9                 = Key.KEY_NUM_9,
   Num0                 = Key.KEY_NUM_0,
   Return               = Key.KEY_RETURN,
   Escape               = Key.KEY_ESCAPE,
   Backspace            = Key.KEY_BACKSPACE,
   Tab                  = Key.KEY_TAB,
   Space                = Key.KEY_SPACE,
   Minus                = Key.KEY_MINUS,
   Equals               = Key.KEY_EQUALS,
   LeftBracket          = Key.KEY_LEFTBRACKET,
   RightBracket         = Key.KEY_RIGHTBRACKET,
   BackSlash            = Key.KEY_BACKSLASH,
   NonUshash            = Key.KEY_NONUSHASH,
   Semicolon            = Key.KEY_SEMICOLON,
   Apostroph            = Key.KEY_APOSTROPHE,
   Grave                = Key.KEY_GRAVE,
   Comma                = Key.KEY_COMMA,
   Period               = Key.KEY_PERIOD,
   Slash                = Key.KEY_SLASH,
   Capslock             = Key.KEY_CAPSLOCK,
   F1                   = Key.KEY_F1,
   F2                   = Key.KEY_F2,
   F3                   = Key.KEY_F3,
   F4                   = Key.KEY_F4,
   F5                   = Key.KEY_F5,
   F6                   = Key.KEY_F6,
   F7                   = Key.KEY_F7,
   F8                   = Key.KEY_F8,
   F9                   = Key.KEY_F9,
   F10                  = Key.KEY_F10,
   F11                  = Key.KEY_F11,
   F12                  = Key.KEY_F12,
   F13                  = Key.KEY_F13,
   F14                  = Key.KEY_F14,
   F15                  = Key.KEY_F15,
   F16                  = Key.KEY_F16,
   F17                  = Key.KEY_F17,
   F18                  = Key.KEY_F18,
   F19                  = Key.KEY_F19,
   F20                  = Key.KEY_F20,
   F21                  = Key.KEY_F21,
   F22                  = Key.KEY_F22,
   F23                  = Key.KEY_F23,
   F24                  = Key.KEY_F24,
   PrintScreen          = Key.KEY_PRINTSCREEN,
   ScrollLock           = Key.KEY_SCROLLLOCK,
   Pause                = Key.KEY_PAUSE,
   Insert               = Key.KEY_INSERT,
   Home                 = Key.KEY_HOME,
   Pageup               = Key.KEY_PAGEUP,
   Delete               = Key.KEY_DELETE,
   End                  = Key.KEY_END,
   Pagedown             = Key.KEY_PAGEDOWN,
   Right                = Key.KEY_RIGHT,
   Left                 = Key.KEY_LEFT,
   Down                 = Key.KEY_DOWN,
   Up                   = Key.KEY_UP,
   NumLockClear         = Key.KEY_NUMLOCKCLEAR,
   KP_Divide            = Key.KEY_KP_DIVIDE,
   KP_Multip            = Key.KEY_KP_MULTIPLY,
   KP_Minus             = Key.KEY_KP_MINUS,
   KP_Plus              = Key.KEY_KP_PLUS,
   KP_Enter             = Key.KEY_KP_ENTER,
   KP_1                 = Key.KEY_KP_1,
   KP_2                 = Key.KEY_KP_2,
   KP_3                 = Key.KEY_KP_3,
   KP_4                 = Key.KEY_KP_4,
   KP_5                 = Key.KEY_KP_5,
   KP_6                 = Key.KEY_KP_6,
   KP_7                 = Key.KEY_KP_7,
   KP_8                 = Key.KEY_KP_8,
   KP_9                 = Key.KEY_KP_9,
   KP_0                 = Key.KEY_KP_0,
   KP_Period            = Key.KEY_KP_PERIOD,
   KP_Equals            = Key.KEY_KP_EQUALS,
   KP_00                = Key.KEY_KP_00,
   KP_000               = Key.KEY_KP_000,
   NonuseBackslash      = Key.KEY_NONUSBACKSLASH,
   Application          = Key.KEY_APPLICATION,
   Power                = Key.KEY_POWER,
   Execute              = Key.KEY_EXECUTE,
   Help                 = Key.KEY_HELP,
   Menu                 = Key.KEY_MENU,
   Select               = Key.KEY_SELECT,
   Stop                 = Key.KEY_STOP,
   Again                = Key.KEY_AGAIN,
   Undo                 = Key.KEY_UNDO,
   Cut                  = Key.KEY_CUT,
   Copy                 = Key.KEY_COPY,
   Paste                = Key.KEY_PASTE,
   Find                 = Key.KEY_FIND,
   Mute                 = Key.KEY_MUTE,
   VolumeUp             = Key.KEY_VOLUMEUP,
   VolumeDown           = Key.KEY_VOLUMEDOWN,
   KP_Comma             = Key.KEY_KP_COMMA,
   KP_EqualsSAS400      = Key.KEY_KP_EQUALSAS400,
   International1       = Key.KEY_INTERNATIONAL1,
   International2       = Key.KEY_INTERNATIONAL2,
   International3       = Key.KEY_INTERNATIONAL3,
   International4       = Key.KEY_INTERNATIONAL4,
   International5       = Key.KEY_INTERNATIONAL5,
   International6       = Key.KEY_INTERNATIONAL6,
   International7       = Key.KEY_INTERNATIONAL7,
   International8       = Key.KEY_INTERNATIONAL8,
   International9       = Key.KEY_INTERNATIONAL9,
   Lang1                = Key.KEY_LANG1,
   Lang2                = Key.KEY_LANG2,
   Lang3                = Key.KEY_LANG3,
   Lang4                = Key.KEY_LANG4,
   Lang5                = Key.KEY_LANG5,
   Lang6                = Key.KEY_LANG6,
   Lang7                = Key.KEY_LANG7,
   Lang8                = Key.KEY_LANG8,
   Lang9                = Key.KEY_LANG9,
   Alterase             = Key.KEY_ALTERASE,
   Sysreq               = Key.KEY_SYSREQ,
   Cancel               = Key.KEY_CANCEL,
   Clear                = Key.KEY_CLEAR,
   Prior                = Key.KEY_PRIOR,
   Return2              = Key.KEY_RETURN2,
   Separator            = Key.KEY_SEPARATOR,
   Out                  = Key.KEY_OUT,
   Oper                 = Key.KEY_OPER,
   ClearAgain           = Key.KEY_CLEARAGAIN,
   Crsel                = Key.KEY_CRSEL,
   Exsel                = Key.KEY_EXSEL,
   ThousandsSeparator   = Key.KEY_THOUSANDSSEPARATOR,
   DecimalSeparator     = Key.KEY_DECIMALSEPARATOR,
   CurrencyUnit         = Key.KEY_CURRENCYUNIT,
   CurrencySubunit      = Key.KEY_CURRENCYSUBUNIT,
   KP_LeftParen         = Key.KEY_KP_LEFTPAREN,
   KP_RightParen        = Key.KEY_KP_RIGHTPAREN,
   KP_LeftBrace         = Key.KEY_KP_LEFTBRACE,
   KP_RightBrace        = Key.KEY_KP_RIGHTBRACE,
   KP_Tab               = Key.KEY_KP_TAB,
   KP_Backspace         = Key.KEY_KP_BACKSPACE,
   KP_A                 = Key.KEY_KP_A,
   KP_B                 = Key.KEY_KP_B,
   KP_C                 = Key.KEY_KP_C,
   KP_D                 = Key.KEY_KP_D,
   KP_E                 = Key.KEY_KP_E,
   KP_F                 = Key.KEY_KP_F,
   KP_Xor               = Key.KEY_KP_XOR,
   KP_Power             = Key.KEY_KP_POWER,
   KP_Percent           = Key.KEY_KP_PERCENT,
   KP_Less              = Key.KEY_KP_LESS,
   KP_Greate            = Key.KEY_KP_GREATER,
   KP_Ampersand         = Key.KEY_KP_AMPERSAND,
   KP_DblAmpersand      = Key.KEY_KP_DBLAMPERSAND,
   KP_VerticalBar       = Key.KEY_KP_VERTICALBAR,
   KP_DblVerticalBar    = Key.KEY_KP_DBLVERTICALBAR,
   KP_Colon             = Key.KEY_KP_COLON,
   KP_Hash              = Key.KEY_KP_HASH,
   KP_Space             = Key.KEY_KP_SPACE,
   KP_At                = Key.KEY_KP_AT,
   KP_Exclam            = Key.KEY_KP_EXCLAM,
   KP_Memstore          = Key.KEY_KP_MEMSTORE,
   KP_Memrecall         = Key.KEY_KP_MEMRECALL,
   KP_Memclear          = Key.KEY_KP_MEMCLEAR,
   KP_Memadd            = Key.KEY_KP_MEMADD,
   KP_Memsubtract       = Key.KEY_KP_MEMSUBTRACT,
   KP_Memmultiply       = Key.KEY_KP_MEMMULTIPLY,
   KP_Memdivide         = Key.KEY_KP_MEMDIVIDE,
   KP_Plusminus         = Key.KEY_KP_PLUSMINUS,
   KP_Clear             = Key.KEY_KP_CLEAR,
   KP_ClearEntry        = Key.KEY_KP_CLEARENTRY,
   KP_Binary            = Key.KEY_KP_BINARY,
   KP_Octal             = Key.KEY_KP_OCTAL,
   KP_Decimal           = Key.KEY_KP_DECIMAL,
   KP_Hexadecimal       = Key.KEY_KP_HEXADECIMAL,
   LCtrl                = Key.KEY_LCTRL,
   LShift               = Key.KEY_LSHIFT,
   LAlt                 = Key.KEY_LALT,
   LGui                 = Key.KEY_LGUI,
   RCtrl                = Key.KEY_RCTRL,
   RShift               = Key.KEY_RSHIFT,
   RAlt                 = Key.KEY_RALT,
   RGui                 = Key.KEY_RGUI,
   Mode                 = Key.KEY_MODE,
   AudionExt            = Key.KEY_AUDIONEXT,
   AudioPrev            = Key.KEY_AUDIOPREV,
   AudioStop            = Key.KEY_AUDIOSTOP,
   AudioPlay            = Key.KEY_AUDIOPLAY,
   AudioMute            = Key.KEY_AUDIOMUTE,
   MediaSelect          = Key.KEY_MEDIASELECT,
   WWW                  = Key.KEY_WWW,
   Mail                 = Key.KEY_MAIL,
   Calculator           = Key.KEY_CALCULATOR,
   Computer             = Key.KEY_COMPUTER,
   AC_Search            = Key.KEY_AC_SEARCH,
   AC_Home              = Key.KEY_AC_HOME,
   AC_Back              = Key.KEY_AC_BACK,
   AC_Forward           = Key.KEY_AC_FORWARD,
   AC_Stop              = Key.KEY_AC_STOP,
   AC_Refresh           = Key.KEY_AC_REFRESH,
   AC_Bookmarks         = Key.KEY_AC_BOOKMARKS,
   BrightnessDown       = Key.KEY_BRIGHTNESSDOWN,
   BrightnessUp         = Key.KEY_BRIGHTNESSUP,
   DisplaysWitch        = Key.KEY_DISPLAYSWITCH,
   KBDillumToggle       = Key.KEY_KBDILLUMTOGGLE,
   KBDillumDown         = Key.KEY_KBDILLUMDOWN,
   KBDillumUp           = Key.KEY_KBDILLUMUP,
   Eject                = Key.KEY_EJECT,
   Sleep                = Key.KEY_SLEEP,
   App1                 = Key.KEY_APP1,
   App2                 = Key.KEY_APP2,
   AudioreWind          = Key.KEY_AUDIOREWIND,
   AudioFastforward     = Key.KEY_AUDIOFASTFORWARD,
   SoftLeft             = Key.KEY_SOFTLEFT,
   SoftRight            = Key.KEY_SOFTRIGHT,
   Call                 = Key.KEY_CALL,
   EndCall              = Key.KEY_ENDCALL,
}

---@class Keyboard
---@field Key fun(self, Key): KeyButton
gamelib.Keyboard = Keyboard
gamelib.Keyboard._component_id = gamelib.Resource.Keyboard

---@class KeyButton
---@field IsPressed fun(self): boolean
---@field IsPressing fun(self): boolean
---@field IsReleased fun(self): boolean
---@field IsReleasing fun(self): boolean
gamelib.KeyButton = KeyButton

---@class MouseButton
---@field IsPressed fun(self): boolean
---@field IsPressing fun(self): boolean
---@field IsReleased fun(self): boolean
---@field IsReleasing fun(self): boolean
gamelib.MouseButton = MouseButton

---@class Mouse
---@field Position(self): Vector2
---@field Offset(self): Vector2
---@field LeftBtn(self): MouseButton
---@field RightBtn(self): MouseButton
---@field MiddleBtn(self): MouseButton
gamelib.Mouse = Mouse

---@class Vector2
---@field x number
---@field y number
---@field new fun(x:number, y: number): Vector2
---@field new fun(value:number): Vector2
---@field new fun(): Vector2
---@field Set fun(self, x, y)
---@field Normalize fun(self) normalize itself
---@field Dot fun(self, other: Vector2): number
---@field Cross fun(self, other: Vector2): number
---@field Distance fun(self, other: Vector2): number
---@field Length fun(self): number
---@field LengthSqrd fun(self): number
gamelib.Vector2 = Vector2

---@type fun(radians: number): number
gamelib.Rad2Deg = Rad2Deg

---@type fun(degrees: number): number
gamelib.Deg2Rad = Deg2Rad

---@type fun(v: Vector2): number
gamelib.Cross = Cross

---@type fun(v: Vector2): number
gamelib.Dot = Dot

---@type fun(v: Vector2): Vector2
gamelib.Normalize = Normalize

---@type function
---@param name string
---@return Key
gamelib.GetKeyFromName = function(name)
    return GetKeyFromName(name)
end


gamelib.ui = {}

---@class RectTransform
---@field transform NodeTransform
---@field size Vector2
gamelib.ui.RectTransform = RectTransform
gamelib.ui.RectTransform._component_id = gamelib.Component.RectTransform

---@class NodeTransform
---@field localTransform Transform
---@field globalTransform Transform
gamelib.NodeTransform = NodeTransform
gamelib.NodeTransform._component_id = gamelib.Component.NodeTransform

---@class ColorBundle
---@field normal Color
---@field hover Color
---@field press Color
---@field Create fun(normal: Color, hover: Color, press: Color): ColorBundle
---@field CreatePureColor fun(color: Color): ColorBundle
gamelib.ui.ColorBundle = ColorBundle

---@class Text
---@field color ColorBundle
---@field offset Vector2
---@field Create fun(texture, color: Color, offset: Vector2): Text
gamelib.ui.Text = Text
gamelib.ui.Text._component_id = gamelib.Component.Text

---@class Image
---@field image ImageHandle
---@field region Rect
---@field color ColorBundle
---@field flip Flip
---@field FromTilesheet fun(tilesheet: Tilesheet, x: number, y: number): Image
---@field FromSpriteBundle fun(sprite: SpriteBundle): Image
gamelib.ui.Image = Image
gamelib.ui.Image._component_id = gamelib.Component.Image

---@class Panel
---@field contentColor ColorBundle|nil
---@field borderColor ColorBundle|nil
---@field Create fun(content: ColorBundle|nil, border: ColorBundle|nil): Panel
gamelib.ui.Panel = Panel
gamelib.ui.Panel._component_id = gamelib.Component.Panel

---@class Scrollbar
---@field min number
---@field max number
---@field value number
---@field width number
---@field sliderLen number
---@field incStep number
---@field Ratio fun(self):number
---@field CreateDefault fun():Scrollbar
gamelib.ui.Scrollbar = Scrollbar
gamelib.ui.Scrollbar._component_id = gamelib.Component.Scrollbar