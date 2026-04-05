#include "chi2analyzer.h"
#include <array>
#include <cmath>

AnalysisResult Chi2Analyzer::analyse(const Image &image) const
{
    AnalysisResult result;
    result.method_name = SteganalysisMethod::CHI2;

    for (auto channel = 0; channel < image.getChannels(); channel++)
        result.metrics.insert({ "Channel" + std::to_string(channel), chi2(image, channel)});

    return result;
}

float Chi2Analyzer::chi2(const Image &image, const size_t channel_index) const
{
    std::array<unsigned int, 256> histogram = {0};

    for (size_t i = channel_index; i < image.getPixelDataSize(); i += image.getChannels())
        histogram[image.getPixelData(i)] += 1;

    float chi2 = 0;

    for (unsigned int k = 0; k < 128; k++)
    {
        const unsigned int a = histogram[2 * k];
        const unsigned int b = histogram[(2 * k) + 1];
        const unsigned int total = a + b;

        if (total == 0)
            continue;

        float expected = total / 2;

        chi2 += pow(a - expected, 2) / expected;
        chi2 += pow(b - expected, 2) / expected;
    }

    return chi2;
}