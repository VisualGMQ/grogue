#pragma once

#include "pch.hpp"

#ifndef NDEBUG
#define GROGUE_ASSERT(expr, msg) \
do { \
    if (!(expr)) { \
        char buf[1024] = {0}; \
        snprintf(buf, sizeof(buf), "%s\n%s [%d]\n%s", __FILE__, __FUNCTION__, __LINE__, #expr); \
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Grogue Assert Failed", buf, nullptr); \
    } \
} while(0)

#define GROGUE_ASSERT_ONCE(expr, msg) \
do { \
    if (!(expr)) { \
        static bool isAsserted = false; \
        if (!isAsserted)  { \
            GROGUE_ASSERT(expr, msg) \
        } \
        isAsserted = true; \
    } \
} while(0)

#else
#define GROGUE_ASSERT(expr, msg)
#define GROGUE_ASSERT_ONCE(expr, msg)
#endif
