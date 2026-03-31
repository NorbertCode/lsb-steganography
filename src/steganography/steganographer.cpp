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

    if (start_index + length_bytes > image->getPixelDataSize())
        throw InvalidMessageLength(length_bytes, image->getPixelDataSize(), start_index);

    std::vector<uint8_t> message_bytes(length_bytes, 0);

    for (size_t byte_index = 0; byte_index < length_bytes; byte_index++)
    {
        size_t byte = start_index + (byte_index * 8);
        uint8_t value = 0;

        for (size_t bit_index = 0; bit_index < 8; bit_index++)
        {
            const uint8_t pixel_data_byte = image->getPixelData(start_index + (byte_index * 8) + bit_index);
            value |= (pixel_data_byte & 0x1) << (7 - bit_index); // Get only LSb shifted to proper bit
        }

        message_bytes[byte_index] = value;
    }

    return message_bytes;
}

void Steganographer::write(const size_t start_index, const std::span<uint8_t> &data)
{
    if (image == nullptr)
        throw NullImageException();

    if (start_index + (data.size() * 8) > image->getPixelDataSize())
        throw InvalidMessageLength(data.size(), image->getPixelDataSize(), start_index);

    for (size_t data_byte_index = 0; data_byte_index < data.size(); data_byte_index++)
    {
        size_t image_byte_index_base = start_index + (data_byte_index * 8);

        for (size_t data_bit_index = 0; data_bit_index < 8; data_bit_index++)
        {
            const size_t image_byte_index = image_byte_index_base + data_bit_index;
            uint8_t image_byte = image->getPixelData(image_byte_index);

            uint8_t bit = (data[data_byte_index] >> (7 - data_bit_index)) & 0x1;
            image_byte = (image_byte & ~1) | bit;

            image->setPixelData(image_byte_index, image_byte);
        }
    }
}