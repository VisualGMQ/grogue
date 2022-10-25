#include "engine/core/log.hpp"

namespace engine {

std::unique_ptr<Logger> Logger::defaultLogger_ = nullptr;

void Logger::Init() {
    defaultLogger_.reset(new Logger(std::cout));
}

void Logger::Quit() { }

void Logger::log(const char* fmt) {
    while (*fmt != '\0') {
        stream_ << *fmt;
        fmt++;
    }   
    stream_ << std::endl;
}

}