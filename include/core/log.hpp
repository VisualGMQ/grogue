#pragma once

namespace grogue::core {

enum class LogLevel {
    Trace = 0,
    Debug,
    Warn,
    Info,
    Error,
    Critical,
    NoLog,
};

class Logger final {
public:
    friend class Log;

    static constexpr size_t MaxBufSize = 1024;

    Logger(const char* name, std::ostream& stream, LogLevel level);

    template <typename... Args>
    void Critical(const char* filename,
                  const char* funcname,
                  size_t line,
                  const char* fmt, Args&&... args) {
        rawLog(LogLevel::Critical, filename, funcname, line, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Error(const char* filename,
                  const char* funcname,
                  size_t line,
                  const char* fmt, Args&&... args) {
        rawLog(LogLevel::Error, filename, funcname, line, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Info(const char* filename,
                  const char* funcname,
                  size_t line,
                  const char* fmt, Args&&... args) {
        rawLog(LogLevel::Info, filename, funcname, line, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Warn(const char* filename,
                  const char* funcname,
                  size_t line,
                  const char* fmt, Args&&... args) {
        rawLog(LogLevel::Warn, filename, funcname, line, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Debug(const char* filename,
                  const char* funcname,
                  size_t line,
                  const char* fmt, Args&&... args) {
        rawLog(LogLevel::Debug, filename, funcname, line, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Trace(const char* filename,
                  const char* funcname,
                  size_t line,
                  const char* fmt, Args&&... args) {
        rawLog(LogLevel::Trace, filename, funcname, line, fmt, std::forward<Args>(args)...);
    }

    void SetLevel(LogLevel level) {
        level_ = level;
    }

    LogLevel GetLevel() const {
        return level_;
    }

private:
    const char* const name_;
    std::ostream& stream_;
    LogLevel level_;

    template <typename... Args>
    void rawLog(LogLevel level,
                const char* filename,
                const char* funcname,
                size_t line,
                const char* fmt, Args&&... args) {
        if (level_ <= level) {
            time_t now = time(0);
            auto tm = std::localtime(&now);
            static char preBuf[MaxBufSize] = {0};
            static char outputBuf[MaxBufSize] = {0};

            size_t filenameLen = strlen(filename);
            const char* filenameBegin = filename;
            filename = filename + filenameLen - 1;
            while (filename != filenameBegin
                   && *filename != '\\' && *filename != '/') {
                filename --;
            }
            filename ++;

            snprintf(preBuf, sizeof(preBuf), fmt, std::forward<Args>(args)...);
            snprintf(outputBuf, sizeof(outputBuf),
                     "[%s][%d/%d/%d %d:%d:%d][%s - %s : %zu]: %s",
                     level2Str(level),
                     tm->tm_year + 1900, tm->tm_mon, tm->tm_mday,
                     tm->tm_hour, tm->tm_min, tm->tm_sec,
                     filename, funcname, line, preBuf);
            stream_ << outputBuf << std::endl;
        }
    }

    const char* level2Str(LogLevel level);
};


class Log final {
public:
    static void Init(LogLevel = LogLevel::Debug);
    static void Quit();

    static void SetAllLevel(LogLevel);
    
    static std::shared_ptr<Logger> CreateConsoleLogger(const char* name);
    static std::shared_ptr<Logger> CreateFileLogger(const char* name, const char* filename);

    static std::shared_ptr<Logger> GetLogger(const char* name);

    static bool IsLoggerExists(const char* name);

    template <typename... Args>
    static void Critical(const char* filename,
                         const char* funcname,
                         size_t line,
                         const char* fmt, Args&&... args) {
        for (auto& log : loggers_) {
            log.second->Critical(filename, funcname, line, fmt, std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    static void Error(const char* filename,
                     const char* funcname,
                     size_t line,
                     const char* fmt, Args&&... args) {
        for (auto& log : loggers_) {
            log.second->Error(filename, funcname, line, fmt, std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    static void Warn(const char* filename,
                     const char* funcname,
                     size_t line,
                     const char* fmt, Args&&... args) {
        for (auto& log : loggers_) {
            log.second->Warn(filename, funcname, line, fmt, std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    static void Info(const char* filename,
                     const char* funcname,
                     size_t line,
                     const char* fmt, Args&&... args) {
        for (auto& log : loggers_) {
            log.second->Info(filename, funcname, line, fmt, std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    static void Debug(const char* filename,
                      const char* funcname,
                      size_t line,
                      const char* fmt, Args&&... args) {
        for (auto& log : loggers_) {
            log.second->Debug(filename, funcname, line, fmt, std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    static void Trace(const char* filename,
                      const char* funcname,
                      size_t line,
                      const char* fmt, Args&&... args) {
        for (auto& log : loggers_) {
            log.second->Trace(filename, funcname, line, fmt, std::forward<Args>(args)...);
        }
    }

private:
    static std::unordered_map<std::string_view, std::shared_ptr<Logger>> loggers_;
    static std::list<std::unique_ptr<std::ostream>> files_;
    static LogLevel initLevel_;
};

#define LOG_CRITICAL(...)   grogue::core::Log::Critical(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...)      grogue::core::Log::Error(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)       grogue::core::Log::Warn(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)       grogue::core::Log::Info(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...)      grogue::core::Log::Debug(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_TRACE(...)      grogue::core::Log::Trace(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

}
