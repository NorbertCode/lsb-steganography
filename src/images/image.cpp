#include "image.h"

unsigned int Image::getWidth() const
{
    return width;
}

unsigned int Image::getHeight() const
{
    return height;
}

unsigned int Image::getChannels() const
{
    return channels;
}

Image::Type Image::getImageType() const
{
    return image_type;
}

std::vector<uint8_t> Image::getFileData() const
{
    return file_data;
}