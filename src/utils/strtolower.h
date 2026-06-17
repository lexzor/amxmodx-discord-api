#pragma once

#include <string>
#include <string_view>
#include <algorithm>
#include <cctype>

std::string strtolower(std::string_view str)
{
    std::string result(str);

    std::ranges::transform(result, result.begin(),
        [](unsigned char c) { return std::tolower(c); });

    return result;
}