#include "imageLoader.h"
#include "types/bmpImage.h"
#include <fstream>
#include <algorithm>

std::unique_ptr<Image> ImageLoader::openImage(const std::string &file_path)
{
    std::vector<uint8_t> data = readData(file_path);

    std::unique_ptr<Image> image;

    switch (getImageTypeFromData(data))
    {
    case Image::Type::BMP:
        image = std::make_unique<BMPImage>(data);
        break;
    
    default:
        break;
    }

    return image;
}

std::vector<uint8_t> ImageLoader::readData(const std::string &file_path)
{
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);

    // TODO: Error handling

    auto file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> file_data(file_size);

    file.read(reinterpret_cast<char*>(file_data.data()), file_size);

    return file_data;
}

Image::Type ImageLoader::getImageTypeFromData(const std::vector<uint8_t> &data) const
{
    if (isBmp(data))
        return Image::Type::BMP;

    return Image::Type::UNKNOWN;
}

bool ImageLoader::isBmp(const std::vector<uint8_t> &data) const
{
    const uint8_t bmp_singature[] = { 0x42, 0x4D };
    return data[0] == bmp_singature[0] && data[1] == bmp_singature[1];
}