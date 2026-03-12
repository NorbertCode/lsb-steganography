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

std::string Steganographer::readMessage(const unsigned int length_bytes) const
{
    if (image == nullptr)
    {
        // TODO: Error handling
        return "";
    }

    // TODO: Handling length > actual length

    std::vector<uint8_t> message_bytes(length_bytes, 0);

    for (unsigned int i = 0; i < length_bytes * 8; i++)
    {
        const uint8_t byte = image->getPixelData(i);
        const uint8_t lsb = byte & 0x1;

        const unsigned int byte_index = i / 8;
        message_bytes[byte_index] <<= 1;
        message_bytes[byte_index] |= lsb;
    }

    return std::string(message_bytes.begin(), message_bytes.end());
}

void Steganographer::writeMessage(const std::string &message)
{
    if (image == nullptr)
    {
        // TODO: Error handling
        return;
    }

    // TODO: Handling message length > actual length

    std::vector<uint8_t> message_bytes(message.begin(), message.end());

    for (unsigned int i = 0; i < message_bytes.size() * 8; i++)
    {
        const unsigned int byte_index = i / 8;

        const uint8_t message_msb = (message_bytes[byte_index] & 0b10000000) >> 7;
        message_bytes[byte_index] <<= 0x1;

        // Change pixel lsb to message msb
        uint8_t pixel_byte = image->getPixelData(i);
        pixel_byte &= 0b11111110; 
        pixel_byte |= message_msb;

        image->setPixelData(i, pixel_byte);
    }
}