#pragma once
#include <string>
#include <memory>
#include "image.h"

class ImageLoader
{
public:
    std::unique_ptr<Image> openImage(const std::string &file_path) const;
    void saveImage(const Image &image, const std::string &file_path) const;

protected:
    virtual std::vector<uint8_t> readData(const std::string &file_path) const;
    virtual void writeData(const std::vector<uint8_t> &data, const std::string &file_path) const;

private:
    Image::Type getImageTypeFromData(const std::vector<uint8_t> &data) const;
    bool isBmp(const std::vector<uint8_t> &data) const;
};

class InvalidFileException : public std::runtime_error
{
public:
    InvalidFileException(const std::string &file_path) : std::runtime_error("Unable to open file " + file_path) { }
};

class UnsupportedFormatException : public std::runtime_error
{
public:
    UnsupportedFormatException() : std::runtime_error("Unsupported image format.") { }
};