#include "messenger.h"

// Messenger follows the following protocol for writing data inside images:
// [magic number]
// [messages amount]   - uint32
// [message 1 length]  - uint32
// [message 1 content] - given amount of chars
// [message 2 length]  - uint32
// ... and so on

std::string Messenger::readMessage(const Image &image, const size_t message_index) const
{
    if (message_index >= getMessageAmount(image))
        throw NonexistentMessage(message_index);

    size_t offset_pointer = getMessagePointer(image, message_index);
    const uint32_t message_length = steganographer.read<uint32_t>(image, offset_pointer);

    offset_pointer += sizeof(uint32_t) * 8; // Start of message

    std::string message;
    message.reserve(message_length);

    while (offset_pointer < offset_pointer + (message_length * 8))
    {
        message += steganographer.read<char>(image, offset_pointer);
        offset_pointer++;
    }

    return message;
}

std::vector<std::string> Messenger::readAllMessages(const Image &image) const
{
    // TODO: Maybe optimize this later
    const size_t message_amount = getMessageAmount(image);
    std::vector<std::string> messages(message_amount);

    for (size_t i = 0; i < message_amount; i++)
        messages[i] = readMessage(image, i);

    return messages;
}

void Messenger::writeMessage(Image &image, const std::string &message)
{
    const uint32_t message_amount = getMessageAmount(image);

    if (message_amount == 0)
        steganographer.write(image, getMagicNumberOffset(), magic_number); // Add magic number if not present

    steganographer.write<uint32_t>(image, getMessageAmountOffset(), message_amount + 1); // Increment message amount

    size_t offset_pointer = getMessagesEndPointer(image);

    // Write message length
    steganographer.write<uint32_t>(image, offset_pointer, message.size());
    offset_pointer += message.size() * 8;

    // Write message content
    for (int i = 0; i < message.size(); i++)
    {
        steganographer.write<char>(image, offset_pointer, message[i]);
        offset_pointer += sizeof(char) * 8;
    }
}

bool Messenger::containsMessages(const Image &image) const
{
    return steganographer.read(image, getMagicNumberOffset(), magic_number.size()) == magic_number;
}

uint32_t Messenger::getMessageAmount(const Image &image) const
{
    if (!containsMessages(image))
        return 0;

    return steganographer.read<uint32_t>(image, getMessageAmountOffset());
}

size_t Messenger::getMessagePointer(const Image &image, const size_t message_index) const
{
    const uint32_t message_amount = steganographer.read<uint32_t>(image, getMessageAmountOffset());

    if (message_index > message_amount) // Allow for searching for all images + 1, so you can find where the next image should be
        throw NonexistentMessage(message_index);

    size_t offset_pointer = getMessageArrayOffset();

    // Iterate through messages
    for (size_t i = 0; i < message_index; i++)
    {
        uint32_t message_length = steganographer.read<uint32_t>(image, offset_pointer);
        offset_pointer += (sizeof(uint32_t) + message_length) * 8;
    }

    return offset_pointer;
}

size_t Messenger::getMessagesEndPointer(const Image &image) const
{
    return getMessagePointer(image, getMessageAmount(image));
}