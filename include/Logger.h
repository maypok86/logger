#pragma once

#include "Modifier.h"

#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>

class Logger
{
protected:
    Logger()
    {
        m_thread_levels[thread_id()] = Level::DEBUG;
    }

public: // static
    static constexpr auto endl = "\n";
    static constexpr auto def = Colour::Modifier(Colour::DEFAULT);
    static constexpr auto t = Colour::Modifier(Colour::WHITE);
    static constexpr auto l = Colour::Modifier(Colour::CYAN);

    static Logger & instance()
    {
        static Logger logger;
        return logger;
    }

#define LOG(name, level)                              \
    template <class... Args>                          \
    static void name(const Args &... args)            \
    {                                                 \
        return instance().log(Level::level, args...); \
    }

#include "logs.inl"

    static std::string timestamp()
    {
        using namespace std::chrono;
        const auto now = system_clock::now();
        const auto tt = system_clock::to_time_t(now);
        const auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
        std::stringstream ss;
        ss << std::put_time(localtime(&tt), "%F %T");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return t.to_string() + ss.str() + def.to_string();
    }

    static std::string thread_id()
    {
        std::stringstream ss;
        ss << std::dec << std::this_thread::get_id();
        return ss.str();
    }

    static std::string wrap_value(const std::string & value)
    {
        return "[" + value + "]";
    }

public:
    enum class Level
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    Logger(const Logger &) = delete;

    Logger & operator=(const Logger &) = delete;

    Logger & operator()(Level level);

private:
    void log(Level level, const std::string & message);

    template <class... Args>
    void log(Level level, const Args &... args)
    {
        std::stringstream ss;
        print(ss, args...);
        log(level, ss.str());
    }

    template <class T, class... Args>
    void print(std::ostream & os, T && first, const Args &... args) const
    {
        os << first;
        print(os, args...);
    }

    template <class T>
    void print(std::ostream & os, const T & value) const
    {
        os << value;
    }

private:
    std::mutex m_mutex;

    std::map<std::string /* thread_id */, Level /* level */> m_thread_levels;
};
