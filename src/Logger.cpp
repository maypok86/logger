#include "Logger.h"

#include <iostream>

void Logger::log(std::string_view message)
{
    std::lock_guard<std::mutex> lock(mt);
    std::cout << "LOG: " << message << std::endl;
}
