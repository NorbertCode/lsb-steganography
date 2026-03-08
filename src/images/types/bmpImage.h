#pragma once
#include "../image.h"

class BMPImage : public Image
{
public:
    BMPImage(std::vector<uint8_t> &data);
    virtual ~BMPImage() = default;

    virtual uint8_t getPixelData(const unsigned int pixel_index, const unsigned int channel) const override;
    virtual void setPixelData(const unsigned int pixel_index, const unsigned int channel, const uint8_t data) override;

private:
    uint32_t pixel_array_offset;
};