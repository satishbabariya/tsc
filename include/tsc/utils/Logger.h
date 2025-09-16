#pragma once

#include "tsc/Common.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <mutex>

namespace tsc {
namespace utils {

enum class LogLevel {
    ERROR = 0,
    WARN = 1,
    INFO = 2,
    DEBUG = 3
};

class Logger {
public:
    static Logger& getInstance();
    
    // Configuration
    void setLogLevel(LogLevel level);
    void setOutputFile(const std::string& filename);
    void setConsoleOutput(bool enabled);
    
    // Logging methods
    void error(const std::string& message, const std::string& component = "");
    void warn(const std::string& message, const std::string& component = "");
    void info(const std::string& message, const std::string& component = "");
    void debug(const std::string& message, const std::string& component = "");
    
    // Convenience macros for source location
    void error(const std::string& message, const std::string& file, int line, const std::string& component = "");
    void warn(const std::string& message, const std::string& file, int line, const std::string& component = "");
    void info(const std::string& message, const std::string& file, int line, const std::string& component = "");
    void debug(const std::string& message, const std::string& file, int line, const std::string& component = "");

private:
    Logger() = default;
    ~Logger() = default;
    
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    void log(LogLevel level, const std::string& message, const std::string& component = "");
    void log(LogLevel level, const std::string& message, const std::string& file, int line, const std::string& component = "");
    
    std::string getLevelString(LogLevel level) const;
    std::string getTimestamp() const;
    
    LogLevel currentLevel_ = LogLevel::INFO;
    bool consoleOutput_ = true;
    std::unique_ptr<std::ofstream> fileOutput_;
    std::mutex logMutex_;
};

// Convenience macros for logging with source location
#define TSC_LOG_ERROR(msg, component) tsc::utils::Logger::getInstance().error(msg, __FILE__, __LINE__, component)
#define TSC_LOG_WARN(msg, component) tsc::utils::Logger::getInstance().warn(msg, __FILE__, __LINE__, component)
#define TSC_LOG_INFO(msg, component) tsc::utils::Logger::getInstance().info(msg, __FILE__, __LINE__, component)
#define TSC_LOG_DEBUG(msg, component) tsc::utils::Logger::getInstance().debug(msg, __FILE__, __LINE__, component)

// Simple macros without source location
#define TSC_LOG_ERROR_SIMPLE(msg) tsc::utils::Logger::getInstance().error(msg)
#define TSC_LOG_WARN_SIMPLE(msg) tsc::utils::Logger::getInstance().warn(msg)
#define TSC_LOG_INFO_SIMPLE(msg) tsc::utils::Logger::getInstance().info(msg)
#define TSC_LOG_DEBUG_SIMPLE(msg) tsc::utils::Logger::getInstance().debug(msg)

} // namespace utils
} // namespace tsc