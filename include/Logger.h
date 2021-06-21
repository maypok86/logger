#pragma once

#include <mutex>

class Logger
{

protected:
    Logger() = default;

public:
    static Logger & instance()
    {
        static Logger logger;
        return logger;
    }

    Logger(const Logger &) = delete;

    Logger & operator=(const Logger &) = delete;

    void log(std::string_view message);

private:
    std::mutex mt;
};
