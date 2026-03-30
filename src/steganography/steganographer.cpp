#include "steganographer.h"
#include <iostream>
#include <vector>

std::unique_ptr<Image> Steganographer::getImage()
{
    return std::move(image);
}

void Steganographer::setImage(std::unique_ptr<Image> image)
{
    this->image = std::move(image);
}

std::vector<uint8_t> Steganographer::read(const size_t start_index, const size_t length_bytes) const
{
    if (image == nullptr)
        throw NullImageException();

    if (length_bytes > image->getSize())
        throw InvalidMessageLength(length_bytes, image->getSize(), start_index);

    std::vector<uint8_t> message_bytes(length_bytes, 0);

    for (size_t i = start_index; i < start_index + (length_bytes * 8); i++)
    {
        const uint8_t byte = image->getPixelData(i);
        const uint8_t lsb = byte & 0x1;

        const size_t byte_index = (i - start_index) / 8;
        message_bytes[byte_index] <<= 1;
        message_bytes[byte_index] |= lsb;
    }

    return message_bytes;
}

void Steganographer::write(const size_t start_index, const std::span<uint8_t> &data)
{
    if (image == nullptr)
        throw NullImageException();

    if (data.size() > image->getSize())
        throw InvalidMessageLength(data.size(), image->getSize(), start_index);

    std::vector<uint8_t> data_copy(data.begin(), data.end());

    for (size_t i = start_index; i < data.size() * 8; i++)
    {
        const size_t byte_index = i / 8;

        const uint8_t data_msb = (data_copy[byte_index] & 0b10000000) >> 7;
        data_copy[byte_index] <<= 0x1;

        // Change pixel lsb to message msb
        uint8_t pixel_byte = image->getPixelData(i);
        pixel_byte &= 0b11111110; 
        pixel_byte |= data_msb;

        image->setPixelData(i, pixel_byte);
    }
}