#pragma once
#include <memory>
#include <type_traits>
#include <span>
#include <cstring>
#include "../images/image.h"

class Steganographer
{
public:
    std::unique_ptr<Image> getImage();
    void setImage(std::unique_ptr<Image> image);

    std::vector<uint8_t> read(const size_t start_index, const size_t length_bytes) const;
    void write(const size_t start_index, const std::span<uint8_t> &data);

    template <typename T> requires std::is_integral_v<T>
    T read(const size_t start_index) const
    {
        std::vector<uint8_t> bytes = read(start_index, sizeof(T));
        T output = 0;

        for (int i = 0; i < sizeof(T); i++)
            output = (output << 8) | bytes[i];

        return output;
    }

    template <typename T> requires std::is_integral_v<T>
    void write(const size_t start_index, const T &data)
    {
        std::vector<uint8_t> bytes(sizeof(T), 0);

        for (int i = 0; i < sizeof(T); i++)
            bytes[i] = data >> (8 * (sizeof(T) - i - 1)) & 0xFF;
        
        write(start_index, bytes);
    }

private:
    std::unique_ptr<Image> image;
};

class NullImageException : public std::logic_error
{
public:
    NullImageException() : std::logic_error("Image is null. Remember to set image before read or write.") { }
};

class InvalidMessageLength : public std::logic_error
{
    const size_t attempted;
    const size_t max;
    const size_t index;

public:
    InvalidMessageLength(const size_t attempted, const size_t max, const size_t index)
        : std::logic_error("Attempted to read or write more bytes than the image can contain: " + std::to_string(index) + " + " + std::to_string(attempted) + " > " + std::to_string(max)), 
          attempted(attempted), max(max), index(index) { }

    const size_t getAttempted() const { return attempted; }
    const size_t getMax() const { return max; }
};