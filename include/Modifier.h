#pragma once

#include <iostream>

namespace Colour {

enum Code
{
    RED = 31,
    GREEN = 32,
    BLUE = 36,
    WHITE = 37,
    YELLOW = 93,
    CYAN = 96,
    DEFAULT = 39,
};

class Modifier
{
    Code code;

public:
    constexpr Modifier(const Code code_)
        : code(code_)
    {
    }

    std::string to_string() const
    {
        return "\033[" + std::to_string(code) + "m";
    }
};

} // namespace Colour
