#include "rsAnalyzer.h"

AnalysisResult RSAnalyzer::analyze(const Image &image) const
{
    AnalysisResult result;
    result.method_name = SteganalysisMethod::RS;

    for (auto channel = 0; channel < image.getChannels(); channel++)
        result.metrics.insert({ "Channel" + std::to_string(channel), rs(image, channel)});

    return result;
}

float RSAnalyzer::rs(const Image &image, const size_t channel_index) const
{
    unsigned int regular = 0;
    unsigned int singular = 0;
    unsigned int unusable = 0;

    unsigned int regular_inverse = 0;
    unsigned int singular_inverse = 0;
    unsigned int unusable_inverse = 0;

    std::vector<int> inverse_mask = getInverseMask();
    std::vector<std::vector<uint8_t>> groups = getGroups(image, channel_index);

    for (const std::vector<uint8_t> &group : groups)
    {
        const float base_discriminant = discriminant(group);

        std::vector<uint8_t> flipped_group = flip(group, mask);
        const float flipped_discriminant = discriminant(flipped_group);

        if (flipped_discriminant > base_discriminant)
            regular += 1;
        else if (flipped_discriminant < base_discriminant)
            singular += 1;
        else
            unusable += 1;

        std::vector<uint8_t> flipped_inverse_group = flip(group, inverse_mask);
        const float flipped_inverse_discriminant = discriminant(flipped_inverse_group);

        if (flipped_inverse_discriminant > base_discriminant)
            regular_inverse += 1;
        else if (flipped_inverse_discriminant < base_discriminant)
            singular_inverse += 1;
        else
            unusable_inverse += 1;
    }

    int numerator = regular - singular;
    int denominator = (regular - singular) + (regular_inverse - singular_inverse);

    float embedding_rate;

    if (denominator == 0)
        embedding_rate = 0;
    else
        embedding_rate = static_cast<float>(numerator) / denominator;

    return embedding_rate;
}

std::vector<int> RSAnalyzer::getInverseMask() const
{
    std::vector<int> inverse_mask(mask.size());

    for (size_t i = 0; i < inverse_mask.size(); i++)
        inverse_mask[i] = -mask[i];

    return inverse_mask;
}

std::vector<std::vector<uint8_t>> RSAnalyzer::getGroups(const Image &image, const size_t channel_index) const
{
    std::vector<uint8_t> channel_data;
    channel_data.reserve(image.getPixelDataSize() / image.getChannels());

    for (size_t i = channel_index; i < image.getPixelDataSize(); i += image.getChannels())
        channel_data.push_back(image.getPixelData(i));

    const size_t group_amount = channel_data.size() / group_size;
    std::vector<std::vector<uint8_t>> groups(group_amount);

    for (size_t group = 0; group < group_amount; group++)
    {
        groups[group].reserve(group_size);

        for (size_t group_element = 0; group_element < group_size; group_element++)
            groups[group].push_back(channel_data[(group * group_size) + group_element]);
    }

    return groups;
}

std::vector<uint8_t> RSAnalyzer::flip(const std::vector<uint8_t> &group, const std::vector<int> &mask) const
{
    if (group.size() != mask.size())
        throw MaskNotMatchesGroupSize(mask.size(), group.size());

    std::vector<uint8_t> flipped(group.size());

    for (size_t i = 0; i < group.size(); i++)
    {
        int value = group[i];

        if (value % 2 == 0)
            value += mask[i];
        else
            value -= mask[i];

        if (value < 0)
            value = 0;
        if (value > 255)
            value = 255;

        flipped[i] = static_cast<uint8_t>(value);
    }

    return flipped;
}

float RSAnalyzer::discriminant(const std::vector<uint8_t> &group) const
{
    float sum = 0;

    for (size_t i = 0; i < group.size() - 1; i++)
        sum += abs(int(group[i + 1]) - int(group[i]));

    return sum;
}