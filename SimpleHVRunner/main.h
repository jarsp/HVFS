#pragma once

#include <iostream>
#include <string>
#include <cstdint>

#include <intrin.h>

class CpuInfo
{
public:
    CpuInfo();

    uint32_t MaxExtendedFunction;
    std::string Vendor;

    bool FeatureSVM;
};
