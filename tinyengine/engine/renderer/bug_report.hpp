#pragma once

#include "engine/core/log.hpp"

namespace engine {

inline void ReportNoRendererBug() {
    Loge1("your application not in video mode, try add `VideoSystem` to the world");
}
    
}
