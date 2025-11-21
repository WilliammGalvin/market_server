#pragma once

#include <utility>
#include <fstream>
#include <optional>
#include <string>

template <typename OutputT>
class Reader {
private:
    std::string file_path_;
    std::size_t pos_{ 0 };
    std::ifstream file_stream_;
    std::string error_{};
    bool binary_{ false };

public:
    Reader(const char* file_path, bool binary = false)
        : file_path_(file_path), binary_(binary) {}

    ~Reader() = default;

    bool open_file() {
        if (binary_) file_stream_.open(file_path_, std::ios::binary); 
        else file_stream_.open(file_path_);

        if (!file_stream_.is_open()) {
            error_ = "Failed to open file: " + file_path_;
            return false;
        }

        error_.clear();
        return file_stream_.is_open();
    }

    std::optional<OutputT> read_line() {
        OutputT temp;

        if constexpr (std::is_same_v<OutputT, std::string>) {
            if (!std::getline(file_stream_, temp)) {
                return std::nullopt;
            }
        } else {
            file_stream_.read(
                reinterpret_cast<char*>(&temp),
                sizeof(OutputT)
            );

            if (file_stream_.gcount() != sizeof(OutputT)) {
                return std::nullopt;
            }
        }

        pos_++;
        return temp;
    }

    bool is_open() const {
        return file_stream_.is_open(); 
    }

    std::size_t current_line() const {
        return pos_;
    }

    bool eof() const {
        return file_stream_.eof();
    }

    const std::string& error() const {
        return error_;
    }
};