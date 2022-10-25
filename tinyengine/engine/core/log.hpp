#pragma once

#include <iostream>
#include <string>
#include <memory>
#include "engine/core/dllexport.hpp"

namespace engine {

/********** old version **********/
// #define Loge(fmt, ...) printf("[ERROR][%s - %s][%d]: " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)
// #define Logw(fmt, ...) printf("[WARN][%s - %s][%d]: " fmt "\n",  __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)
// #define Logi(fmt, ...) printf("[INFO][%s - %s][%d]: " fmt "\n",  __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)
// #define Logd(fmt, ...) printf("[DEBUG][%s - %s][%d]: " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)
// #define Logt(fmt, ...) printf("[TRACE][%s - %s][%d]: " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__)
//
// // log once
// #define Loge1(fmt, ...) do { static bool logged = false; if (!logged) { printf("[ONCE][ERROR][%s - %s][%d]: " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__); logged = true;} } while(0)
// #define Logw1(fmt, ...) do { static bool logged = false; if (!logged) { printf("[ONCE][WARN][%s - %s][%d]: " fmt "\n",  __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__); logged = true;} } while(0)
// #define Logi1(fmt, ...) do { static bool logged = false; if (!logged) { printf("[ONCE][INFO][%s - %s][%d]: " fmt "\n",  __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__); logged = true;} } while(0)
// #define Logd1(fmt, ...) do { static bool logged = false; if (!logged) { printf("[ONCE][DEBUG][%s - %s][%d]: " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__); logged = true;} } while(0)
// #define Logt1(fmt, ...) do { static bool logged = false; if (!logged) { printf("[ONCE][TRACE][%s - %s][%d]: " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__); logged = true;} } while(0)

#define Loge(fmt, ...) engine::Logger::DefaultLogger().LogError(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define Logw(fmt, ...) engine::Logger::DefaultLogger().LogWarn(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define Logi(fmt, ...) engine::Logger::DefaultLogger().LogInfo(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define Logd(fmt, ...) engine::Logger::DefaultLogger().LogDebug(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define Logt(fmt, ...) engine::Logger::DefaultLogger().LogTrace(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

// log once
#define Loge1(fmt, ...)                                                                                     \
    do {                                                                                                       \
        static bool logged = false;                                                                         \
        if (!logged) {                                                                                                   \
            engine::Logger::DefaultLogger().LogError(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
            logged = true;                                                                                  \
        }                                                                                                   \
    } while (0)
#define Logw1(fmt, ...)                                                                                    \
    do {                                                                                                      \
        static bool logged = false;                                                                        \
        if (!logged) {                                                                                                  \
            engine::Logger::DefaultLogger().LogWarn(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
            logged = true;                                                                                 \
        }                                                                                                  \
    } while (0)
#define Logi1(fmt, ...)                                                                                    \
    do {                                                                                                      \
        static bool logged = false;                                                                        \
        if (!logged) {                                                                                                  \
            engine::Logger::DefaultLogger().LogInfo(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
            logged = true;                                                                                 \
        }                                                                                                  \
    } while (0)
#define Logd1(fmt, ...)                                                                                     \
    do {                                                                                                       \
        static bool logged = false;                                                                         \
        if (!logged) {                                                                                                   \
            engine::Logger::DefaultLogger().LogDebug(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
            logged = true;                                                                                  \
        }                                                                                                   \
    } while (0)
#define Logt1(fmt, ...)                                                                                     \
    do {                                                                                                       \
        static bool logged = false;                                                                         \
        if (!logged) {                                                                                                   \
            engine::Logger::DefaultLogger().LogTrace(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
            logged = true;                                                                                  \
        }                                                                                                   \
    } while (0)

class DLLEXPORT Logger final {
public:
    static void Init();
    static void Quit();

    static Logger &DefaultLogger() { return *defaultLogger_; }

    Logger(std::ostream &o) : stream_(o) {}

    template <typename... Args>
    void Log(const char *level, const char *filename, const char *function, int linenum, const std::string &fmt, Args &&...args);

    template <typename... Args>
    void LogWarn(const char *filename, const char *function, int linenum, const std::string &fmt, Args &&...args) {
        Log("WARN", filename, function, linenum, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void LogInfo(const char *filename, const char *function, int linenum, const std::string &fmt, Args &&...args) {
        Log("INFO", filename, function, linenum, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void LogError(const char *filename, const char *function, int linenum, const std::string &fmt, Args &&...args) {
        Log("ERROR", filename, function, linenum, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void LogDebug(const char *filename, const char *function, int linenum, const std::string &fmt, Args &&...args) {
        Log("DEBUG", filename, function, linenum, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void LogTrace(const char *filename, const char *function, int linenum, const std::string &fmt, Args &&...args) {
        Log("TRACE", filename, function, linenum, fmt, std::forward<Args>(args)...);
    }

private:
    static std::unique_ptr<Logger> defaultLogger_;

    std::ostream &stream_;

    void log(const char *fmt);

    template <typename T, typename... Args>
    void log(const char *fmt, T &&value, Args &&...args);
};

template <typename... Args>
void Logger::Log(const char *level, const char *filename, const char *function, int linenum, const std::string &fmt, Args &&...args) {
    const char *ptr = filename + strlen(filename);
    while (*ptr != '\\' && *ptr != '/' && ptr - filename != 0) {
        ptr--;
    }
    ptr++;
    stream_ << "[" << level << "]"
            << "[" << ptr << " - " << function << "][" << linenum << "]: " << std::flush;
    log(fmt.c_str(), std::forward<Args>(args)...);
}

template <typename T, typename... Args>
void Logger::log(const char *fmt, T &&value, Args &&... args) {
    while (*fmt != '\0' && *fmt != '{') {
        stream_ << *fmt;
        fmt++;
    }
    if (*fmt == '{') {
        if (*(fmt + 1) == '}') {
            fmt += 2;
            stream_ << value << std::flush;
        }
        log(fmt, std::forward<Args>(args)...);
    }
}

}
