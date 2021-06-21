#include "Logger.h"

#include <random>
#include <thread>
#include <vector>

int main()
{
    std::vector<std::thread> modules;
    for (int id = 1; id <= 5; ++id) {
        modules.emplace_back([id]() {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<> ud(100, 1000);
            Logger::info("module ", id, " started");
            std::this_thread::sleep_for(std::chrono::milliseconds(ud(mt)));
            Logger::info(
                    "module " + std::to_string(id) + " finished");
        });
    }
    for (auto & m : modules) {
        m.join();
    }

    return 0;
}