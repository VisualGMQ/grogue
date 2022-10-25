#pragma once

#include "engine/core/pch.hpp"

#if !defined(NDEBUG) && !defined(_NDEBUG)
#define DEBUG_MODE
#endif

namespace engine {

std::string DLLEXPORT ReadWholeFile(const std::string& filename);

}