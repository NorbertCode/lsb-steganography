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