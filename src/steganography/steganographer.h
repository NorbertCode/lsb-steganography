#pragma once
#include <memory>
#include "../images/image.h"

class Steganographer
{
public:
    std::unique_ptr<Image> getImage();
    void setImage(std::unique_ptr<Image> image);

    std::string readMessage(const unsigned int length_bytes) const;
    void writeMessage(const std::string &message);

private:
    std::unique_ptr<Image> image;
};

class NullImageException : public std::logic_error
{
public:
    NullImageException() : std::logic_error("Image is null. Remember to set image before read or write.") { }
};

class InvalidMessageLength : public std::logic_error
{
    const unsigned int attempted;
    const unsigned int max;

public:
    InvalidMessageLength(const unsigned int attempted, const unsigned int max)
        : std::logic_error("Attempted to read or write more bytes than the image can contain: " + std::to_string(attempted) + " > " + std::to_string(max)), attempted(attempted), max(max) { }

    const unsigned int getAttempted() const { return attempted; }
    const unsigned int getMax() const { return max; }
};