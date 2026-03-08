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

    int getWidth() const;
    int getHeight() const;
    Type getImageType() const;
    std::vector<uint8_t> getFileData() const;

    virtual uint8_t getPixelDataAtIndex(const unsigned int index) const = 0;
    virtual void setPixelDataAtIndex(const unsigned int index, const uint8_t data) = 0;

protected:
    int width, height;
    Type image_type;
    std::vector<uint8_t> image_data;
};