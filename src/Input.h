#pragma once

#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

/**
 * Base input class
 */
class IInput {
public:
    virtual ~IInput() = default;
    [[nodiscard]] virtual bool isValid() const = 0;
    [[nodiscard]] virtual std::optional<std::string> nextLine() = 0;
};

/**
 * Read input from a file
 */
class FileInput : public IInput {
public:
    explicit FileInput(std::string_view filepath)
            : file(filepath.data()) {}

    [[nodiscard]] bool isValid() const override {
        return this->file.is_open();
    }

    [[nodiscard]] std::optional<std::string> nextLine() override {
        std::string line;
        if (std::getline(file, line)) {
            return line;
        }
        return std::nullopt;
    }

private:
    std::ifstream file;
};

/**
 * Read input from the terminal
 */
class TerminalInput : public IInput {
public:
    TerminalInput() = default;

    [[nodiscard]] bool isValid() const override {
        return true;
    }

    [[nodiscard]] std::optional<std::string> nextLine() override {
        std::string line;
        if (!std::getline(std::cin, line) || line.empty()) {
            return std::nullopt;
        }
        return line;
    }
};
