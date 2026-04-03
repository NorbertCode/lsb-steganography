#include <gtest/gtest.h>
#include "../sharedTestData.h"
#include "../../src/steganography/steganographer.h"
#include "../../src/images/types/bmpImage.h"

class SteganographerTest : public testing::Test
{
protected:
    Steganographer steganographer;

    std::unique_ptr<Image> image_2x2;
    std::unique_ptr<Image> image_2x2_hidden_char;

    std::unique_ptr<Image> image_5x5;
    std::unique_ptr<Image> image_5x5_hidden_int;

    SteganographerTest()
    {
        image_2x2 = std::make_unique<BMPImage>(valid_2x2_3channel_bmp);
        image_2x2_hidden_char = std::make_unique<BMPImage>(valid_2x2_3channel_bmp_hidden_char);

        image_5x5 = std::make_unique<BMPImage>(valid_5x5_3channel_bmp);
        image_5x5_hidden_int = std::make_unique<BMPImage>(valid_5x5_3channel_bmp_hidden_int);
    }
};

TEST_F(SteganographerTest, Read_ByteValid_ReadsCorrectly) 
{
    const std::vector<uint8_t> output = steganographer.read(*image_2x2, 0, 1);

    EXPECT_EQ(output.size(), 1);
    EXPECT_EQ(output[0], 0b10011100);
}

TEST_F(SteganographerTest, Read_TwoBytesValid_ReadsCorrectly) 
{
    const std::vector<uint8_t> output = steganographer.read(*image_2x2, 0, 2);

    EXPECT_EQ(output.size(), 2);
    EXPECT_EQ(output[0], 0b10011100);
    EXPECT_EQ(output[1], 0b00101000);
}

TEST_F(SteganographerTest, Read_StartIndexChanged_ReadsCorrectly) 
{
    const std::vector<uint8_t> output_index_1 = steganographer.read(*image_2x2, 1, 1);
    const std::vector<uint8_t> output_index_2 = steganographer.read(*image_2x2, 2, 1);

    EXPECT_EQ(output_index_1.size(), 1);
    EXPECT_EQ(output_index_1[0], 0b00111000);

    EXPECT_EQ(output_index_2.size(), 1);
    EXPECT_EQ(output_index_2[0], 0b01110000);
}

TEST_F(SteganographerTest, Read_TooHighStartIndex_ThrowsError) 
{
    EXPECT_THROW(steganographer.read(*image_2x2, 32, 1), InvalidMessageLength);
}

TEST_F(SteganographerTest, Read_LengthGreaterThanDataSize_ThrowsError) 
{
    EXPECT_THROW(steganographer.read(*image_2x2, 0, 32), InvalidMessageLength);
}

TEST_F(SteganographerTest, Read_EndOutOfBounds_ThrowsError) 
{
    EXPECT_THROW(steganographer.read(*image_2x2, 12, 8), InvalidMessageLength);
}

TEST_F(SteganographerTest, ReadTemplate_Char_ReadsCorrectly) 
{
    char output = steganographer.read<char>(*image_2x2_hidden_char, 0);

    EXPECT_EQ(output, 'A');
}

TEST_F(SteganographerTest, ReadTemplate_Int_ReadsCorrectly) 
{
    int output = steganographer.read<int>(*image_5x5_hidden_int, 0);

    EXPECT_EQ(output, 123);
}

TEST_F(SteganographerTest, Write_ByteValid_WritesCorrectly) 
{
    std::unique_ptr<Image> image = std::move(image_2x2);
    std::vector<uint8_t> input = { 0b00000001 };

    steganographer.write(*image, 0, input);

    for (int i = 0; i < 8; i++)
    {
        uint8_t bit = image->getPixelData(i) & 0x1;
        uint8_t expected = (input[0] >> (7 - i)) & 0x1;
        printf("%d: %d %d\n", i, bit, expected);
        EXPECT_EQ(bit, expected);
    }
}

TEST_F(SteganographerTest, Write_TwoBytesValid_WritesCorrectly) 
{
    std::unique_ptr<Image> image = std::move(image_2x2);
    std::vector<uint8_t> input = { 0b11110000, 0b00001111 };

    steganographer.write(*image, 0, input);

    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < 8; j++)
        {
            uint8_t bit = image->getPixelData((i * 8) + j) & 0x1;
            uint8_t expected = (input[i] >> (7 - j)) & 0x1;
            EXPECT_EQ(bit, expected);
        }
    }
}

TEST_F(SteganographerTest, Write_StartIndexChanged_WritesCorrectly) 
{
    std::unique_ptr<Image> image = std::move(image_2x2);
    std::vector<uint8_t> input = { 0b11110000 };

    steganographer.write(*image, 1, input);

    for (int i = 0; i < 8; i++)
    {
        uint8_t bit = image->getPixelData(1 + i) & 0x1;
        uint8_t expected = (input[0] >> (7 - i)) & 0x1;
        EXPECT_EQ(bit, expected);
    }

    steganographer.write(*image, 2, input);

    for (int i = 0; i < 8; i++)
    {
        uint8_t bit = image->getPixelData(2 + i) & 0x1;
        uint8_t expected = (input[0] >> (7 - i)) & 0x1;
        EXPECT_EQ(bit, expected);
    }
}

TEST_F(SteganographerTest, Write_TooHighStartIndex_ThrowsError) 
{
    std::vector<uint8_t> input = { 0b11110000 };

    EXPECT_THROW(steganographer.write(*image_2x2, 32, input), InvalidMessageLength);
}

TEST_F(SteganographerTest, Write_LengthGreaterThanDataSize_ThrowsError) 
{
    std::vector<uint8_t> input = { 0b11110000, 0b00000000, 0b00000000, 0b00000000 }; // 32 bits

    EXPECT_THROW(steganographer.write(*image_2x2, 0, input), InvalidMessageLength);
}

TEST_F(SteganographerTest, Write_EndOutOfBounds_ThrowsError) 
{
    std::vector<uint8_t> input = { 0b11110000, 0b00000000 }; // 8 bits

    EXPECT_THROW(steganographer.write(*image_2x2, 12, input), InvalidMessageLength);
}

TEST_F(SteganographerTest, WriteTemplate_Char_WritesCorrectly)
{
    std::unique_ptr<Image> image = std::move(image_2x2);
    steganographer.write<char>(*image, 0, 'A');

    for (int i = 0; i < 8; i++)
    {
        uint8_t bit = image->getPixelData(i) & 0x1;
        uint8_t expected = ('A' >> (7 - i)) & 0x1;
        printf("%d: %d %d\n", i, bit, expected);
        EXPECT_EQ(bit, expected);
    }
}

TEST_F(SteganographerTest, WriteTemplate_Int_WritesCorrectly)
{
    std::unique_ptr<Image> image = std::move(image_5x5);
    std::vector<uint8_t> input = { 0x0, 0x0, 0x0, 0x05 };

    steganographer.write<int>(*image, 0, 5);

    for (int i = 0; i < sizeof(int); i++)
    {
        for (int j = 0; j < 8; j++)
        {
            uint8_t bit = image->getPixelData((i * 8) + j) & 0x1;
            uint8_t expected = (input[i] >> (7 - j)) & 0x1;
            EXPECT_EQ(bit, expected);
        }
    }
}