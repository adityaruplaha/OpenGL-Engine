#pragma once

#include <cstdarg>
#include <ctime>
#include <memory>
#include <iostream>
#include <fstream>

class QuikLogger
{
public:
	~QuikLogger();

	void log(int level, std::string from, int line, const char* format, ...);
	void registerFile(std::ofstream* file);

	void enableLog(bool val);

	static const int LOG_SUCCESS;
	static const int LOG_INFO;
	static const int LOG_WARNING;
	static const int LOG_ERROR;
	static const int LOG_CRITICAL;
	
private:
	static const int LOG_UNKNOWN;

	bool log_enabled = true;

	static std::string getLevel(int level);
	std::unique_ptr<std::ofstream> file;
};

extern std::shared_ptr<QuikLogger> logger;

#define LOG_SUCCESS(...) logger->log(QuikLogger::LOG_SUCCESS, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...) logger->log(QuikLogger::LOG_INFO, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(...) logger->log(QuikLogger::LOG_WARNING, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) logger->log(QuikLogger::LOG_ERROR, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_CRITICAL(...) logger->log(QuikLogger::LOG_CRITICAL, __FUNCTION__, __LINE__, __VA_ARGS__)

#define REGISTER_LOG_FILE(file) logger->registerFile(file)

#define ENABLE_LOGGING() logger->enableLog(true)
#define DISABLE_LOGGING() logger->enableLog(false)