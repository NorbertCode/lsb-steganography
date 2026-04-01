#include <gtest/gtest.h>
#include "../../src/images/imageLoader.h"
#include "../sharedTestData.h"

class TestImageLoader : public ImageLoader
{
public:
    void setFakeData(const std::vector<uint8_t> &fake_data)
    {
        this->fake_data = fake_data;
    }

    std::vector<uint8_t> &getWrittenData() const
    {
        return written_data;
    }

protected:
    std::vector<uint8_t> readData(const std::string &file_path) const override
    {
        return fake_data;
    }

    void writeData(const std::vector<uint8_t> &data, const std::string &file_path) const override
    {
        this->written_data = data;
    }
    
private:
    std::vector<uint8_t> fake_data;
    mutable std::vector<uint8_t> written_data;
};

TEST(ImageLoaderTest, OpenImage_ValidBMP_Detected)
{
    TestImageLoader loader;
    loader.setFakeData(valid_1x1_3channel_bmp);

    std::unique_ptr<Image> image = loader.openImage("");

    EXPECT_EQ(image->getImageType(), Image::Type::BMP);
}

TEST(ImageLoaderTest, OpenImage_UnsupportedFormat_ThrowsError)
{
    TestImageLoader loader;
    loader.setFakeData({ 0x00, 0x00 });

    EXPECT_THROW(std::unique_ptr<Image> image = loader.openImage(""), UnsupportedFormatException);
}

TEST(ImageLoaderTest, SaveImage_Valid_WritesCorrectly)
{
    TestImageLoader loader;
    loader.setFakeData(valid_1x1_3channel_bmp);

    std::unique_ptr<Image> image = loader.openImage("");
    loader.saveImage(image, "");

    EXPECT_EQ(loader.getWrittenData(), valid_1x1_3channel_bmp);
}