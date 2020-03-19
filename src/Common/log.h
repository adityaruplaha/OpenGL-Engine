#pragma once

#include <cstdarg>
#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>

enum class LogLevel : uint8_t { Success, Info, Warning, Error, Critical };

class Logger {
public:
    ~Logger();

    void log(LogLevel level, std::string from, int line, const char* format, ...);
    void registerFile(std::ofstream* file);

    void enableLog(bool val);

private:
    bool log_enabled = true;
    void setColor(LogLevel level);

    static std::string getLevel(LogLevel level);
    std::ofstream* file;
};

extern std::shared_ptr<Logger> logger;

#define LOG_SUCCESS(...) logger->log(LogLevel::Success, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...) logger->log(LogLevel::Info, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(...) logger->log(LogLevel::Warning, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) logger->log(LogLevel::Error, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_CRITICAL(...) logger->log(LogLevel::Critical, __FUNCTION__, __LINE__, __VA_ARGS__)

#define REGISTER_LOG_FILE(file) logger->registerFile(file)

#define ENABLE_LOGGING() logger->enableLog(true)
#define DISABLE_LOGGING() logger->enableLog(false)
