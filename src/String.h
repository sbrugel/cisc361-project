#pragma once

#include <cstdio>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace string {

std::vector<std::string> split(const std::string& input, char delimiter);

// https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template<typename... Args>
std::string format(const std::string& format, Args... args) {
    int strSize = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    if (strSize <= 0)
        return "";
    auto size = static_cast<std::size_t>(strSize);
    std::unique_ptr<char[]> buf{new char[size]};
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return {buf.get(), buf.get() + size - 1}; // We don't want the '\0'
}

} // namespace string
