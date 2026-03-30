#pragma once
#include <iostream>
#include <vector>
#include <cstdint>

class Image
{
public:
    enum class Type
    {
        UNKNOWN,
        BMP,
    };

    virtual ~Image() = default;

    unsigned int getWidth() const;
    unsigned int getHeight() const;
    unsigned int getChannels() const;
    size_t getSize() const;
    Type getImageType() const;
    std::vector<uint8_t> getFileData() const;

    virtual size_t getPixelDataSize() const = 0;
    virtual uint8_t getPixelData(const size_t index) const = 0;
    virtual void setPixelData(const size_t index, const uint8_t data) = 0;

protected:
    unsigned int width, height;
    unsigned int channels;
    Type image_type;
    std::vector<uint8_t> file_data;
};