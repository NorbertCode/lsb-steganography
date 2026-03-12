#include "bmpImage.h"
#include <cstring>
#include <exception>

#define COLOR_DEPTH_OFFSET  28
#define COLOR_DEPTH_SIZE    2

#define COMPRESSION_OFFSET  30
#define COMPRESSION_SIZE    4

#define PIXEL_ARRAY_OFFSET_ADDR  10
#define PIXEL_ARRAY_OFFSET_SIZE  4

#define WIDTH_OFFSET  18
#define WIDTH_SIZE    4

#define HEIGHT_OFFSET  22
#define HEIGHT_SIZE    4

BMPImage::BMPImage(std::vector<uint8_t> data)
{
    file_data = std::move(data);

    uint16_t color_depth;
    uint32_t compression;

    std::memcpy(&color_depth, file_data.data() + COLOR_DEPTH_OFFSET, COLOR_DEPTH_SIZE);
    std::memcpy(&compression, file_data.data() + COMPRESSION_OFFSET, COMPRESSION_SIZE);

    if (compression != 0x0 || !(color_depth == 24 || color_depth == 32))
        throw InvalidBmpException();

    channels = color_depth / 8;

    std::memcpy(&pixel_array_offset, file_data.data() + PIXEL_ARRAY_OFFSET_ADDR, PIXEL_ARRAY_OFFSET_SIZE);

    std::memcpy(&width, file_data.data() + WIDTH_OFFSET, WIDTH_SIZE);
    std::memcpy(&height, file_data.data() + HEIGHT_OFFSET, HEIGHT_SIZE);
}

uint8_t BMPImage::getPixelData(const unsigned int index) const
{
    return file_data[pixel_array_offset + index];
}

void BMPImage::setPixelData(const unsigned int index, const uint8_t data)
{
    file_data[pixel_array_offset + index] = data;
}