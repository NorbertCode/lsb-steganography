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
    Type getImageType() const;
    std::vector<uint8_t> getFileData() const;

    virtual uint8_t getPixelData(const unsigned int pixel_index, const unsigned int channel) const = 0;
    virtual void setPixelData(const unsigned int pixel_index, const unsigned int channel, const uint8_t data) = 0;

protected:
    unsigned int width, height;
    unsigned int channels;
    Type image_type;
    std::vector<uint8_t> file_data;
};