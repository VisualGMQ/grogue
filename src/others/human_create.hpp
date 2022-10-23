#pragma once

#include "engine/engine.hpp"
#include "components/human.hpp"
#include "others/object_reader.hpp"
#include <filesystem>

engine::Entity* CreateHuman(const std::string& down, const std::string& up, const std::string& right);