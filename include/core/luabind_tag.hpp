#pragma once

//! @brief a macro to tag a struct is a component for parser.
//!     use: `class LUA_BIND_COMPONENT YourComponent {};`
#define LUA_BIND_COMPONENT

//! @brief a macro to tag a struct is a resource for parser.
//!     use: `class LUA_BIND_RESOURCE YourResource {};`
#define LUA_BIND_RESOURCE

//! @brief a macro to tag a struct should bind to lua for parser.
//!     use: `class LUA_BIND YourClass {};`
#define LUA_BIND

//! @brief a macro to tag a struct should not bind to lua for parser.
#define LUA_NOBIND

