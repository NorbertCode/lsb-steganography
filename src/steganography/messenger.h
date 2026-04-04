#pragma once
#include "steganographer.h"

class Messenger
{
public:
    Messenger() { }
    Messenger(Steganographer &steganographer) : steganographer(steganographer) { }

    std::string readMessage(const Image &image, const size_t message_index = 0) const;
    std::vector<std::string> readAllMessages(const Image &image) const;

    void writeMessage(Image &image, const std::string &message);

private:
    inline static const std::vector<uint8_t> magic_number = { 0x4D, 0x53, 0x47 }; // "MSG"
    Steganographer steganographer;

    bool containsMessages(const Image &image) const;
    uint32_t getMessageAmount(const Image &image) const;

    static size_t getMagicNumberOffset() { return 0; }
    static size_t getMessageAmountOffset() { return magic_number.size() * 8; }
    static size_t getMessageArrayOffset() { return (magic_number.size() + sizeof(uint32_t)) * 8; }
    size_t getMessagePointer(const Image &image, const size_t index) const;
    size_t getMessagesEndPointer(const Image &image) const;
};

class NonexistentMessage : public std::logic_error
{
    const size_t index;

public:
    NonexistentMessage(const size_t index)
        : std::logic_error("Message with index " + std::to_string(index) + " doesn't exist."),
          index(index) { }

    const size_t getIndex() const { return index; }
};

class MessageWontFit : public std::logic_error
{
    const size_t available_bits;

public:
    MessageWontFit(const size_t available_bits)
        : std::logic_error("Image cannot fit message. Only " + std::to_string(available_bits) + " bits available to store in image."),
          available_bits(available_bits) { }

    const size_t getAvailableBits() const { return available_bits; }
};