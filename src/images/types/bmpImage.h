#pragma once
#include "../image.h"

class BMPImage : public Image
{
public:
    BMPImage(std::vector<uint8_t> &data);
    virtual ~BMPImage() = default;

    virtual uint8_t getPixelDataAtIndex(const unsigned int index) const override;
    virtual void setPixelDataAtIndex(const unsigned int index, const uint8_t data) override;

private:
    uint16_t color_depth;
    uint32_t pixel_array_offset;
};