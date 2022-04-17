#pragma once

namespace grogue::core {

enum class LogLevel {
    Critical = spdlog::level::critical,
    Info     = spdlog::level::info,
    Warn     = spdlog::level::warn,
    Debug    = spdlog::level::debug,
    Trace    = spdlog::level::trace,
};

class Logger final {
public:
    friend class Log;

    operator bool() const {
        return Valid();
    }

    bool Valid() const {
        return logger_ != nullptr;
    }

    template <typename... Args>
    void Critical(const char* fmt, Args&&... args) {
        if (Valid())
            logger_->critical(fmt, std::forward(args...));
    }

    template <typename... Args>
    void Info(const char* fmt, Args&&... args) {
        if (Valid())
            logger_->info(fmt, std::forward(args...));
    }

    template <typename... Args>
    void Warn(const char* fmt, Args&&... args) {
        if (Valid())
            logger_->warn(fmt, std::forward(args...));
    }

    template <typename... Args>
    void Debug(const char* fmt, Args&&... args) {
        if (Valid())
            logger_->debug(fmt, std::forward(args...));
    }

    template <typename... Args>
    void Trace(const char* fmt, Args&&... args) {
        if (Valid())
            logger_->trace(fmt, std::forward(args...));
    }

    void SetLevel(LogLevel level) {
        if (Valid())
            logger_->set_level(static_cast<spdlog::level::level_enum>(level));
    }

private:
    std::shared_ptr<spdlog::logger> logger_;

    Logger(std::shared_ptr<spdlog::logger> logger): logger_(logger) {}
};


class Log final {
public:
    static void Init(LogLevel = LogLevel::Debug);
    static void Quit();

    static void SetAllLevel(LogLevel);
    
    static Logger CreateConsoleLogger(const char* name);
    static Logger CreateFileLogger(const char* name, const char* filename);

    static void RegistLogger(const Logger& logger);
    static void UnregistLogger(const Logger& logger);

    static Logger GetLogger(const char* name);
};

#define LOG_CRITICAL(...)   SPDLOG_CRITICAL(__VA_ARGS__)
#define LOG_ERROR(...)      SPDLOG_ERROR(__VA_ARGS__)
#define LOG_INFO(...)       SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...)       SPDLOG_WARN(__VA_ARGS__)
#define LOG_DEBUG(...)      SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_TRACE(...)      SPDLOG_TRACE(__VA_ARGS__)

}
