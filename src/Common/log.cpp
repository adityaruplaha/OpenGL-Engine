#include <map>
#include <sstream>

#ifdef WIN32
#include <Windows.h>
#endif

#include "log.h"

Logger::~Logger() {
    file->close();
}

void Logger::log(LogLevel level, std::string from, int line, const char* format, ...) {
    if (!log_enabled) {
        return;
    }

    // get time
    time_t rawtime;
    time(&rawtime);
    tm* timeinfo = localtime(&rawtime);
    char time_buf[64];
    strftime(time_buf, sizeof(time_buf) / sizeof(char), "[%D %I:%M%p] ", &*timeinfo);

    // generate log
    va_list args;
    va_start(args, format);
    char buf[512];
    vsprintf(buf, format, args);
    va_end(args);
    // end generate log

    std::string lv = getLevel(level);

    char ss[1024];
    sprintf(ss, "%s %s (%s)::L%i    %s\n", std::string(time_buf).c_str(), lv.c_str(), from.c_str(),
            line, std::string(buf).c_str());

    setColor(level);
    std::cout << ss;
    setColor(LogLevel::Info);

    if (file->is_open()) {
        *file << ss;
    }
}

#ifdef WIN32

void setColor(LogLevel level) {
    static const std::map<LogLevel, int> color_map{
        {LogLevel::Critical, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY},
        {LogLevel::Error, FOREGROUND_RED | FOREGROUND_INTENSITY},
        {LogLevel::Warning, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY},
        {LogLevel::Info,
         FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY},
        {LogLevel::Success, FOREGROUND_GREEN | FOREGROUND_INTENSITY}};
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, color_map[level]);
}

#else

void Logger::setColor(LogLevel level) {
    static const std::string red = "\033[1;31m";
    static const std::string green = "\033[1;32m";
    static const std::string yellow = "\033[1;33m";
    static const std::string magenta = "\033[1;35m";
    static const std::string white = "\033[1;37m";

    switch (level) {
    case (LogLevel::Critical):
        std::cout << magenta;
        break;
    case (LogLevel::Error):
        std::cout << red;
        break;
    case (LogLevel::Warning):
        std::cout << yellow;
        break;
    case (LogLevel::Info):
        std::cout << white;
        break;
    case (LogLevel::Success):
        std::cout << green;
        break;
    }
}

#endif

void Logger::registerFile(std::ofstream* file) {
    this->file = file;
}

void Logger::enableLog(bool val) {
    log_enabled = val;
}

std::string Logger::getLevel(LogLevel level) {
    switch (level) {
    case (LogLevel::Critical):
        return "<Critical>";
    case (LogLevel::Error):
        return "<Error>   ";
    case (LogLevel::Warning):
        return "<Warning> ";
    case (LogLevel::Info):
        return "<Info>    ";
    case (LogLevel::Success):
        return "<Success> ";
    }
}

extern std::shared_ptr<Logger> logger = std::make_shared<Logger>();
