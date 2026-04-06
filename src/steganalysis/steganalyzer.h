#pragma once
#include "../images/image.h"
#include <map>
#include <string>

enum class SteganalysisMethod
{
    CHI2,
    RS,
};

struct AnalysisResult
{
    SteganalysisMethod method_name;
    std::map<std::string, float> metrics;
};

class Steganalyzer
{
public:
    virtual ~Steganalyzer() = default;

    virtual AnalysisResult analyze(const Image &image) const = 0;
};