#include "image.h"

int Image::getWidth() const
{
    return width;
}

int Image::getHeight() const
{
    return height;
}

Image::Type Image::getImageType() const
{
    return image_type;
}

std::vector<uint8_t> Image::getFileData() const
{
    return image_data;
}