
#pragma once

#include <string>
#include <cassert>
#include "debugbreak.hpp"
#include "log.hpp"

#define Assert(expr, msg) do { \
    if (!(expr)) { \
        LOGF(msg); \
        debug_break(); \
    } \
} while(0)