#pragma once

#include <iostream>
#include <format>
#include <string>

namespace console_bmp {

void println(const std::string& format, const auto&... a) {
    std::string formattedString = std::vformat(format, std::make_format_args(std::forward<decltype(a)>(a)...));
    std::cout << formattedString << std::endl;
}

void print(const std::string& format, const auto&... a) {
    std::string formattedString = std::vformat(format, std::make_format_args(std::forward<decltype(a)>(a)...));
    std::cout << formattedString;
}

} // namespace console_bmp