#pragma once

#include <iostream>
#include <format>
#include <string>

namespace bmp_reader {


// Wrappers for any `std::ostream`
void any_println(std::ostream& os, const std::string& format, const auto&... a) {
    std::string formattedString = std::vformat(format, std::make_format_args(std::forward<decltype(a)>(a)...));
    os << formattedString << std::endl;
}

void any_print(std::ostream& os, const std::string& format, const auto&... a) {
    std::string formattedString = std::vformat(format, std::make_format_args(std::forward<decltype(a)>(a)...));
    os << formattedString;
}

// Wrappers for `std::cout`
void println(const std::string& format, const auto&... a) {
    any_println(std::cout, format, std::forward<decltype(a)>(a)...);
}

void print(const std::string& format, const auto&... a) {
    any_print(std::cout, format, std::forward<decltype(a)>(a)...);
}

// Wrappers for `std::cerr`
void eprintln(const std::string& format, const auto&... a) {
    any_println(std::cerr, format, std::forward<decltype(a)>(a)...);
}

void eprint(const std::string& format, const auto&... a) {
    any_print(std::cerr, format, std::forward<decltype(a)>(a)...);
}


} // namespace bmp_reader