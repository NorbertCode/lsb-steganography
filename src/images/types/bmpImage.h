#pragma once
#include "../image.h"

class BMPImage : public Image
{
public:
    BMPImage(std::vector<uint8_t> data);
    virtual ~BMPImage() = default;

    virtual size_t getPixelDataSize() const override;
    virtual uint8_t getPixelData(const size_t index) const override;
    virtual void setPixelData(const size_t index, const uint8_t data) override;

private:
    uint32_t pixel_array_offset;
};

class InvalidBmpException : public std::runtime_error
{
public:
    InvalidBmpException() : std::runtime_error("Only uncompressed, 24 or 32 bit bmps are allowed.") { }
};