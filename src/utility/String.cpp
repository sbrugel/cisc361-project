#include "String.h"

#include <algorithm>
#include <regex>

bool string::startsWith(std::string_view str, char beginning) {
    return (str.length() >= 1) && (str.at(0) == beginning);
}

bool string::startsWith(std::string_view str, std::string_view beginning) {
    return (str.length() >= beginning.length()) && (str.compare(0, beginning.length(), beginning) == 0);
}

bool string::endsWith(std::string_view str, char end) {
    return (str.length() >= 1) && (str.at(str.length() - 1) == end);
}

bool string::endsWith(std::string_view str, std::string_view end) {
    return (str.length() >= end.length()) && (str.compare(str.length() - end.length(), end.length(), end) == 0);
}

bool string::contains(std::string_view str, char sub) {
    return (str.length() >= 1) && (str.find(sub) != std::string::npos);
}

bool string::contains(std::string_view str, std::string_view substr) {
    return (str.length() >= substr.length()) && (str.find(substr) != std::string::npos);
}

void string::remove(std::string& input, char charToRemove) {
    input.erase(std::remove(input.begin(), input.end(), charToRemove), input.end());
}

std::vector<std::string> string::split(const std::string& input, char delimiter) {
    std::stringstream ss{input};
    std::vector<std::string> out;
    std::string token;
    while (std::getline(ss, token, delimiter))
        out.push_back(token);
    return out;
}

std::string string::toLower(std::string_view str) {
    std::string out{str};
    std::transform(out.begin(), out.end(), out.begin(), toLowerChar);
    return out;
}

std::string string::toUpper(std::string_view str) {
    std::string out{str};
    std::transform(out.begin(), out.end(), out.begin(), toUpperChar);
    return out;
}

// The following strip functions are modified from https://linuxhint.com/trim-string-cpp

std::string string::stripLeft(const std::string& s) {
    static const auto regexStripLeft = std::regex("^[ \t\n\r\v\f]*");
    return std::regex_replace(s, regexStripLeft, "");
}

std::string string::stripRight(const std::string& s) {
    static const auto regexStripRight = std::regex("[ \t\n\r\v\f]*$");
    return std::regex_replace(s, regexStripRight, "");
}

std::string string::strip(const std::string& s) {
    static const auto regexStrip = std::regex("^[ \t\n\r\v\f]*|[ \t\n\r\v\f]*$");
    return std::regex_replace(s, regexStrip, "");
}

std::string string::stripLeft(const std::string& s, std::string_view c) {
    return std::regex_replace(s, std::regex(std::string{"^["} + c.data() +"]*"), "");
}

std::string string::stripRight(const std::string& s, std::string_view c) {
    return std::regex_replace(s, std::regex(std::string{"["} + c.data() +"]*$"), "");
}

std::string string::strip(const std::string& s, std::string_view c) {
    return std::regex_replace(s, std::regex(std::string{"^["} + c.data() + "]*|[" + c.data() + "]*$"), "");
}

std::string string::stripLeft(const std::string& s, char c) {
    return std::regex_replace(s, std::regex("^[" + std::to_string(c) +"]*"), "");
}

std::string string::stripRight(const std::string& s, char c) {
    return std::regex_replace(s, std::regex("[" + std::to_string(c) +"]*$"), "");
}

std::string string::strip(const std::string& s, char c) {
    return std::regex_replace(s, std::regex("^[" + std::to_string(c) + "]*|[" + c + "]*$"), "");
}

void string::replace(std::string& s, std::string_view from, std::string_view to) {
    s = std::regex_replace(s, std::regex{from.data()}, to.data());
}
