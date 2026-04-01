#include <gtest/gtest.h>
#include "../../sharedTestData.h"
#include "../../../src/images/types/bmpImage.h"

TEST(BMPImageTest, GetParameters_1x1_ReturnsCorrectValues)
{
    BMPImage image(valid_1x1_3channel_bmp);

    EXPECT_EQ(image.getWidth(), 1);
    EXPECT_EQ(image.getHeight(), 1);
    EXPECT_EQ(image.getChannels(), 3);
    EXPECT_EQ(image.getSize(), valid_1x1_3channel_bmp.size());
    EXPECT_EQ(image.getImageType(), Image::Type::BMP);
    EXPECT_EQ(image.getFileData(), valid_1x1_3channel_bmp);
    EXPECT_EQ(image.getPixelDataSize(), 4);
}

TEST(BMPImageTest, GetParameters_2x2_ReturnsCorrectValues)
{
    BMPImage image(valid_2x2_3channel_bmp);

    EXPECT_EQ(image.getWidth(), 2);
    EXPECT_EQ(image.getHeight(), 2);
    EXPECT_EQ(image.getChannels(), 3);
    EXPECT_EQ(image.getSize(), valid_2x2_3channel_bmp.size());
    EXPECT_EQ(image.getImageType(), Image::Type::BMP);
    EXPECT_EQ(image.getFileData(), valid_2x2_3channel_bmp);
    EXPECT_EQ(image.getPixelDataSize(), 16);
}

TEST(BMPImageTest, GetPixelData_1x1ValidIndex_ReturnsCorrectValue)
{
    BMPImage image(valid_1x1_3channel_bmp);

    EXPECT_EQ(image.getPixelData(0), 0x00); // Pixel 0 B
    EXPECT_EQ(image.getPixelData(1), 0x00); // Pixel 0 G
    EXPECT_EQ(image.getPixelData(2), 0x00); // Pixel 0 R
}

TEST(BMPImageTest, GetPixelData_2x2ValidIndex_ReturnsCorrectValue)
{
    BMPImage image(valid_2x2_3channel_bmp);

    // Bottom left
    EXPECT_EQ(image.getPixelData(0), 0xFF); // Pixel 0 B
    EXPECT_EQ(image.getPixelData(1), 0x00); // Pixel 0 G
    EXPECT_EQ(image.getPixelData(2), 0x00); // Pixel 0 R

    // Bottom right
    EXPECT_EQ(image.getPixelData(3), 0xFF); // Pixel 1 B
    EXPECT_EQ(image.getPixelData(4), 0xFF); // Pixel 1 G
    EXPECT_EQ(image.getPixelData(5), 0xFF); // Pixel 1 R

    // Top left
    EXPECT_EQ(image.getPixelData(8), 0x00); // Pixel 2 B (jump to 8 because padding)
    EXPECT_EQ(image.getPixelData(9), 0x00); // Pixel 2 G
    EXPECT_EQ(image.getPixelData(10), 0xFF); // Pixel 2 R

    // Top right
    EXPECT_EQ(image.getPixelData(11), 0x00); // Pixel 3 B
    EXPECT_EQ(image.getPixelData(12), 0xFF); // Pixel 3 G
    EXPECT_EQ(image.getPixelData(13), 0x00); // Pixel 3 R
}

TEST(BMPImageTest, SetPixelData_1x1ValidIndex_UpdatesPixelData)
{
    BMPImage image(valid_1x1_3channel_bmp);

    image.setPixelData(0, 0xFF); // Set Red to 255
    image.setPixelData(1, 0x00); // Set Green to 0
    image.setPixelData(2, 0x00); // Set Blue to 0

    EXPECT_EQ(image.getPixelData(0), 0xFF); // Red
    EXPECT_EQ(image.getPixelData(1), 0x00); // Green
    EXPECT_EQ(image.getPixelData(2), 0x00); // Blue
}

TEST(BMPImageTest, SetPixelData_2x2ValidIndex_UpdatesPixelData)
{
    BMPImage image(valid_1x1_3channel_bmp);
    const unsigned int pixel_bytes = image.getWidth() * image.getHeight() * image.getChannels();

    for (int i = 0; i < pixel_bytes; i++)
    {
        image.setPixelData(i, 0xFF);
    }

    for (int i = 0; i < pixel_bytes; i++)
    {
        EXPECT_EQ(image.getPixelData(i), 0xFF);
    }
}
