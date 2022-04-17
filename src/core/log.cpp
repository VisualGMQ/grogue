#include "core/log.hpp"

namespace grogue::core {

Logger Log::CreateConsoleLogger(const char* name) {
    Logger logger(nullptr);
    try {
        logger.logger_ = spdlog::stdout_color_mt(name);
    } catch (const spdlog::spdlog_ex& ex) {
        spdlog::warn("console log init failed: {}", ex.what());
    }
    return logger;
}

Logger Log::CreateFileLogger(const char* name, const char* filename) {
    Logger logger(nullptr);
    try {
        logger.logger_ = spdlog::basic_logger_mt(name, filename);
    } catch (const spdlog::spdlog_ex& ex) {
        spdlog::warn("file {} log init failed: {}", filename, ex.what());
    }
    return logger;
}

Logger Log::GetLogger(const char* name) {
    return Logger(spdlog::get(name));
}

void Log::Init(LogLevel level) {
    spdlog::set_pattern("[%^%l%$][%Y-%m-%d %T][%s:%#]: %v");
    SetAllLevel(level);
}

void Log::SetAllLevel(LogLevel level) {
    spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
}

void Log::RegistLogger(const Logger& logger) {
    try {
        spdlog::register_logger(logger.logger_);
    } catch (const spdlog::spdlog_ex& ex) {
        LOG_WARN("regist log error: {}", ex.what());
    }
}

void Log::UnregistLogger(const Logger& logger) {
    spdlog::drop(logger.logger_->name());
}

void Log::Quit() {
}

}
