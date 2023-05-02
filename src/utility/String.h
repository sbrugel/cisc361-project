#pragma once

#include <cstdio>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace string {

[[nodiscard]] constexpr char toLowerChar(char in) {
    if (in >= 'A' && in <= 'Z') {
        return static_cast<char>(in + 32);
    }
    return in;
}

[[nodiscard]] constexpr char toUpperChar(char in) {
    if (in >= 'a' && in <= 'z') {
        return static_cast<char>(in - 32);
    }
    return in;
}

bool startsWith(std::string_view str, char beginning);
bool startsWith(std::string_view str, std::string_view beginning);

bool endsWith(std::string_view str, char end);
bool endsWith(std::string_view str, std::string_view end);

bool contains(std::string_view str, char sub);
bool contains(std::string_view str, std::string_view substr);

void remove(std::string& input, char charToRemove);

std::vector<std::string> split(const std::string& input, char delimiter);

std::string toLower(std::string_view str);
std::string toUpper(std::string_view str);

std::string stripLeft(const std::string& s);
std::string stripRight(const std::string& s);
std::string strip(const std::string& s);

std::string stripLeft(const std::string& s, std::string_view c);
std::string stripRight(const std::string& s, std::string_view c);
std::string strip(const std::string& s, std::string_view c);

std::string stripLeft(const std::string& s, char c);
std::string stripRight(const std::string& s, char c);
std::string strip(const std::string& s, char c);

void replace(std::string& s, std::string_view from, std::string_view to);

template<typename T, typename = void>
struct is_joinable : std::false_type {};

template<typename T>
struct is_joinable<T, std::void_t<decltype(
    std::begin(std::declval<T>()),
    std::end(std::declval<T>())
)>> : std::true_type {};

template<typename T>
constexpr bool is_joinable_v = is_joinable<T>::value;

template<typename T>
std::string join(T& vector, std::string_view separator, std::string_view end = "") {
    static_assert(is_joinable_v<T>, "Given type is not a container!");

    auto first = std::begin(vector);
    auto last = std::end(vector);
    if (first == last) {
        return end.data();
    }

    std::stringstream ss;
    ss << *first;
    ++first;

    while (first != last) {
        ss << separator;
        ss << *first;
        ++first;
    }
    ss << end;

    return ss.str();
}

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
