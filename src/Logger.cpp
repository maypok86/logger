#include "Logger.h"

namespace {

std::string to_string(const Logger::Level level)
{
    static std::unordered_map<Logger::Level, std::string> level_map = {
            {Logger::Level::DEBUG, "DEBUG"},
            {Logger::Level::INFO, "INFO"},
            {Logger::Level::WARNING, "WARNING"},
            {Logger::Level::ERROR, "ERROR"}};
    auto it = level_map.find(level);
    if (it == level_map.end()) {
        return "NONE";
    }
    return Logger::l.to_string() + it->second + Logger::def.to_string();
}

} // anonymous namespace

void Logger::log(const Level level, const std::string & message)
{
    const auto id = thread_id();
    if (m_thread_levels.find(id) != m_thread_levels.end() && level < m_thread_levels[id]) {
        return;
    }
    std::string result = wrap_value(timestamp()) + " " + wrap_value(to_string(level)) + " " + message;
    std::unique_lock<std::mutex> lock(m_mutex);
    std::cout << result << std::endl;
}

Logger & Logger::operator()(Logger::Level level)
{
    m_thread_levels[thread_id()] = level;
    return instance();
}
