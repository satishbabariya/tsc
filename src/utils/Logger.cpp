#include "tsc/utils/Logger.h"
#include <chrono>
#include <iomanip>
#include <sstream>

namespace tsc {
namespace utils {

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::setLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(logMutex_);
    currentLevel_ = level;
}

void Logger::setOutputFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex_);
    fileOutput_ = std::make_unique<std::ofstream>(filename, std::ios::app);
}

void Logger::setConsoleOutput(bool enabled) {
    std::lock_guard<std::mutex> lock(logMutex_);
    consoleOutput_ = enabled;
}

void Logger::error(const std::string& message, const std::string& component) {
    log(LogLevel::ERROR, message, component);
}

void Logger::warn(const std::string& message, const std::string& component) {
    log(LogLevel::WARN, message, component);
}

void Logger::info(const std::string& message, const std::string& component) {
    log(LogLevel::INFO, message, component);
}

void Logger::debug(const std::string& message, const std::string& component) {
    log(LogLevel::DEBUG, message, component);
}

void Logger::error(const std::string& message, const std::string& file, int line, const std::string& component) {
    log(LogLevel::ERROR, message, file, line, component);
}

void Logger::warn(const std::string& message, const std::string& file, int line, const std::string& component) {
    log(LogLevel::WARN, message, file, line, component);
}

void Logger::info(const std::string& message, const std::string& file, int line, const std::string& component) {
    log(LogLevel::INFO, message, file, line, component);
}

void Logger::debug(const std::string& message, const std::string& file, int line, const std::string& component) {
    log(LogLevel::DEBUG, message, file, line, component);
}

void Logger::log(LogLevel level, const std::string& message, const std::string& component) {
    if (level > currentLevel_) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(logMutex_);
    
    std::stringstream ss;
    ss << "[" << getTimestamp() << "] "
       << "[" << getLevelString(level) << "] ";
    
    if (!component.empty()) {
        ss << "[" << component << "] ";
    }
    
    ss << message << std::endl;
    
    std::string logLine = ss.str();
    
    if (consoleOutput_) {
        if (level == LogLevel::ERROR) {
            std::cerr << logLine;
        } else {
            std::cout << logLine;
        }
    }
    
    if (fileOutput_ && fileOutput_->is_open()) {
        *fileOutput_ << logLine;
        fileOutput_->flush();
    }
}

void Logger::log(LogLevel level, const std::string& message, const std::string& file, int line, const std::string& component) {
    if (level > currentLevel_) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(logMutex_);
    
    std::stringstream ss;
    ss << "[" << getTimestamp() << "] "
       << "[" << getLevelString(level) << "] ";
    
    if (!component.empty()) {
        ss << "[" << component << "] ";
    }
    
    ss << "[" << file << ":" << line << "] " << message << std::endl;
    
    std::string logLine = ss.str();
    
    if (consoleOutput_) {
        if (level == LogLevel::ERROR) {
            std::cerr << logLine;
        } else {
            std::cout << logLine;
        }
    }
    
    if (fileOutput_ && fileOutput_->is_open()) {
        *fileOutput_ << logLine;
        fileOutput_->flush();
    }
}

std::string Logger::getLevelString(LogLevel level) const {
    switch (level) {
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

std::string Logger::getTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    
    return ss.str();
}

} // namespace utils
} // namespace tsc