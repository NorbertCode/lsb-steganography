#include "image.h"

int Image::getWidth() const
{
    return width;
}

int Image::getHeight() const
{
    return height;
}

int Image::getChannels() const
{
    return channels;
}

size_t Image::getSize() const
{
    return file_data.size();
}

Image::Type Image::getImageType() const
{
    return image_type;
}

std::vector<uint8_t> Image::getFileData() const
{
    return file_data;
}