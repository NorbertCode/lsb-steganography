#pragma once
#include "../steganalyzer.h"

class Chi2Analyzer : public Steganalyzer
{
public:
    virtual AnalysisResult analyse(const Image &image) const override;

private:
    float chi2(const Image &image, const size_t channel_index) const;
};