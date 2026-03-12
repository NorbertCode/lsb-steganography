#pragma once
#include <string>
#include <memory>
#include "image.h"

class ImageLoader
{
public:
    std::unique_ptr<Image> openImage(const std::string &file_path) const;
    void saveImage(const std::unique_ptr<Image> image, const std::string &file_path) const;

private:
    std::vector<uint8_t> readData(const std::string &file_path) const;

    Image::Type getImageTypeFromData(const std::vector<uint8_t> &data) const;
    bool isBmp(const std::vector<uint8_t> &data) const;
};