#include "imageLoader.h"
#include "types/bmpImage.h"
#include <fstream>
#include <algorithm>

std::unique_ptr<Image> ImageLoader::openImage(const std::string &file_path) const
{
    std::vector<uint8_t> data = readData(file_path);

    std::unique_ptr<Image> image;

    switch (getImageTypeFromData(data))
    {
    case Image::Type::BMP:
        image = std::make_unique<BMPImage>(std::move(data));
        break;
    
    default:
        throw UnsupportedFormatException();
        break;
    }

    return image;
}

void ImageLoader::saveImage(const std::unique_ptr<Image> &image, const std::string &file_path) const
{
    writeData(image->getFileData(), file_path);
}

std::vector<uint8_t> ImageLoader::readData(const std::string &file_path) const
{
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);

    if (!file.is_open())
        throw InvalidFileException(file_path);

    auto file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> file_data(file_size);

    file.read(reinterpret_cast<char*>(file_data.data()), file_size);

    file.close();

    return file_data;
}

void ImageLoader::writeData(const std::vector<uint8_t> &data, const std::string &file_path) const
{
    std::ofstream file(file_path, std::ios::binary);

    if (!file.is_open())
        throw InvalidFileException(file_path);

    file.write(reinterpret_cast<const char*>(data.data()), data.size());

    file.close();
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