#include "core/log.hpp"

namespace grogue::core {

Logger::Logger(const char* name, std::ostream& stream, LogLevel level): name_(name), stream_(stream), level_(level) {}

const char* Logger::level2Str(LogLevel level) {
    #define CASE(level) case LogLevel::level: return #level;
    switch (level) {
        CASE(Critical)
        CASE(Error)
        CASE(Info)
        CASE(Warn)
        CASE(Debug)
        CASE(Trace)
        CASE(NoLog)
    }
    #undef CASE
    return "Unknown Level";
}


std::unordered_map<std::string_view, std::shared_ptr<Logger>> Log::loggers_;
std::list<std::unique_ptr<std::ostream>> Log::files_;
LogLevel Log::initLevel_;

#define LOG_INNER_ERROR(fmt, ...) printf("[Logger System Inner Error]: " fmt, ##__VA_ARGS__);

void Log::Init(LogLevel level) {
    initLevel_ = level; 
}

void Log::Quit() {}

void Log::SetAllLevel(LogLevel level) {
    for (auto& pair : loggers_) {
        pair.second->SetLevel(level);
    }
}

std::shared_ptr<Logger> Log::GetLogger(const char* name) {
    auto it = loggers_.find(name);
    if (it != loggers_.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

std::shared_ptr<Logger> Log::CreateConsoleLogger(const char* name) {
    if (IsLoggerExists(name)) {
        return nullptr;
    } else {
        auto logger = std::make_shared<Logger>(name, std::cout, initLevel_);
        loggers_[name] = logger;
        return logger;
    }
}

std::shared_ptr<Logger> Log::CreateFileLogger(const char* name,
                                              const char* filename) {
    if (IsLoggerExists(name)) {
        return nullptr;
    } else {
        files_.emplace_back(new std::ofstream(filename));
        auto& file = files_.back();
        if (file->fail()) {
            files_.pop_back();
            LOG_INNER_ERROR("`%s` logger already exists\n", name);
            return nullptr;
        } else {
            auto logger = std::make_shared<Logger>(name, *(file), initLevel_);
            loggers_[name] = logger;
            return logger;
        }
    }
}

bool Log::IsLoggerExists(const char* name) {
    return loggers_.find(name) != loggers_.end();
}

}
