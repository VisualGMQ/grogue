#pragma once

#include "pch.hpp"
#include "video.hpp"
#include "log.hpp"
#include "err_code.hpp"

namespace grogue::core {

void InitSystem(const char* title,
                std::uint32_t w, std::uint32_t h,
                bool resizable = false);
void QuitSystem();

}
