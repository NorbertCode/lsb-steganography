#pragma once
#include "../steganalyzer.h"

class RSAnalyzer : public Steganalyzer
{
public:
    RSAnalyzer(const size_t group_size = 4, const std::vector<int> &mask = { 1, 0, 1, 0}) : group_size(group_size), mask(mask) { }

    virtual AnalysisResult analyze(const Image &image) const override;

    unsigned int getGroupSize() const { return group_size; }
    std::vector<int> getMask() const { return mask; }
    std::vector<int> getInverseMask() const;

private:
    const size_t group_size;
    const std::vector<int> mask;

    float rs(const Image &image, const size_t channel_index) const;

    std::vector<std::vector<uint8_t>> getGroups(const Image &image, const size_t channel_index) const;
    std::vector<uint8_t> flip(const std::vector<uint8_t> &group, const std::vector<int> &mask) const;
    float discriminant(const std::vector<uint8_t> &group) const;
};

class MaskNotMatchesGroupSize : public std::logic_error
{
    const size_t mask_size;
    const size_t group_size;

public:
    MaskNotMatchesGroupSize(const size_t mask_size, const size_t group_size)
        : std::logic_error("RS Analyzer's mask does not match group size: " + std::to_string(mask_size) + " != " + std::to_string(group_size)),
          mask_size(mask_size), group_size(group_size) { }

    size_t getMaskSize() const { return mask_size; }
    size_t getGroupSize() const { return group_size; }
};