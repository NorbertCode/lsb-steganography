#include <gtest/gtest.h>
#include "../../src/steganography/messenger.h"
#include "../../src/images/types/bmpImage.h"
#include "../sharedTestData.h"

TEST(MessengerTest, Read_ValidFirstMessageFromOne_ReadsCorrectly)
{
    std::unique_ptr<Image> image = std::make_unique<BMPImage>(valid_7x7_3channel_bmp_hidden_message);
    Steganographer steganographer;
    Messenger messenger(steganographer);

    std::string output = messenger.readMessage(*image);

    EXPECT_EQ(output, "HELLO");
}

TEST(MessengerTest, Read_ValidFirstMessageFromMultiple_ReadsCorrectly)
{
    std::unique_ptr<Image> image = std::make_unique<BMPImage>(valid_7x7_3channel_bmp_2_hidden_messages);
    Steganographer steganographer;
    Messenger messenger(steganographer);

    std::string output = messenger.readMessage(*image);

    EXPECT_EQ(output, "ABC");
}

TEST(MessengerTest, Read_InvalidFirstMessage_ThrowsError)
{
    std::unique_ptr<Image> image = std::make_unique<BMPImage>(valid_7x7_3channel_bmp);
    Steganographer steganographer;
    Messenger messenger(steganographer);

    EXPECT_THROW(messenger.readMessage(*image), NonexistentMessage);
}

TEST(MessengerTest, Read_ValidSecondMessage_ReadsCorrectly)
{
    std::unique_ptr<Image> image = std::make_unique<BMPImage>(valid_7x7_3channel_bmp_2_hidden_messages);
    Steganographer steganographer;
    Messenger messenger(steganographer);

    std::string output = messenger.readMessage(*image, 1);

    EXPECT_EQ(output, "DE");
}

TEST(MessengerTest, Read_InvalidSecondMessage_ThrowsError)
{
    std::unique_ptr<Image> image = std::make_unique<BMPImage>(valid_7x7_3channel_bmp_hidden_message);
    Steganographer steganographer;
    Messenger messenger(steganographer);

    EXPECT_THROW(messenger.readMessage(*image, 1), NonexistentMessage);
}

TEST(MessengerTest, ReadAll_ExistingMessages_ReadsCorrectly)
{
    std::unique_ptr<Image> image = std::make_unique<BMPImage>(valid_7x7_3channel_bmp_2_hidden_messages);
    Steganographer steganographer;
    Messenger messenger(steganographer);
    std::vector<std::string> expected = { "ABC", "DE" };

    std::vector<std::string> output = messenger.readAllMessages(*image);

    EXPECT_EQ(output, expected);
}

TEST(MessengerTest, ReadAll_NoMessages_ReturnsEmpty)
{
    std::unique_ptr<Image> image = std::make_unique<BMPImage>(valid_7x7_3channel_bmp);
    Steganographer steganographer;
    Messenger messenger(steganographer);
    std::vector<std::string> expected = { };

    std::vector<std::string> output = messenger.readAllMessages(*image);

    EXPECT_EQ(output, expected);
}

TEST(MessengerTest, Write_ValidMessage_WritesCorrectly)
{
    std::unique_ptr<Image> image = std::make_unique<BMPImage>(valid_7x7_3channel_bmp);
    Steganographer steganographer;
    Messenger messenger(steganographer);

    messenger.writeMessage(*image, "HELLO");

    EXPECT_EQ(image->getFileData(), valid_7x7_3channel_bmp_hidden_message);
}

TEST(MessengerTest, Write_ValidMessageExactLength_WritesCorrectly)
{
    std::unique_ptr<Image> image = std::make_unique<BMPImage>(valid_7x7_3channel_bmp);
    Steganographer steganographer;
    Messenger messenger(steganographer);

    // "ABCDEFGHIJ" - 10 chars, a 7x7 bmp can fit a message of at most 10 chars
    EXPECT_NO_THROW(messenger.writeMessage(*image, "ABCDEFGHIJ"));
}

TEST(MessengerTest, Write_NextValidMessage_WritesCorrectly)
{
    std::unique_ptr<Image> image = std::make_unique<BMPImage>(valid_7x7_3channel_bmp);
    Steganographer steganographer;
    Messenger messenger(steganographer);

    messenger.writeMessage(*image, "ABC");
    messenger.writeMessage(*image, "DE");

    EXPECT_EQ(image->getFileData(), valid_7x7_3channel_bmp_2_hidden_messages);
}

TEST(MessengerTest, Write_NoSpaceForHeader_ThrowsError)
{
    std::unique_ptr<Image> image = std::make_unique<BMPImage>(valid_1x1_3channel_bmp);
    Steganographer steganographer;
    Messenger messenger(steganographer);

    EXPECT_THROW(messenger.writeMessage(*image, ""), MessageWontFit);
}

TEST(MessengerTest, Write_MessageWontFit_ThrowsError)
{
    std::unique_ptr<Image> image = std::make_unique<BMPImage>(valid_7x7_3channel_bmp);
    Steganographer steganographer;
    Messenger messenger(steganographer);

    // "ABCDEFGHIJK" - 11 chars, a 7x7 bmp can fit a message of at most 10 chars
    EXPECT_THROW(messenger.writeMessage(*image, "ABCDEFGHIJK"), MessageWontFit);
}

TEST(MessengerTest, Write_NextMessageWontFit_ThrowsError)
{
    std::unique_ptr<Image> image = std::make_unique<BMPImage>(valid_7x7_3channel_bmp);
    Steganographer steganographer;
    Messenger messenger(steganographer);

    messenger.writeMessage(*image, "ABCDEF");
    EXPECT_THROW(messenger.writeMessage(*image, "GHIJK"), MessageWontFit);
}