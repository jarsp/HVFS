#include "main.h"

CpuInfo::CpuInfo()
{
    int32_t info[4];

    // MaxInputValue, Vendor
    __cpuid(info, 0x80000000);

    this->MaxExtendedFunction = info[0];

    int32_t vendor[4] = { 0, 0, 0, 0 };
    vendor[0] = info[1];
    vendor[1] = info[3];
    vendor[2] = info[2];
    this->Vendor = (char *)vendor;

    // SVM Feature Set
    __cpuid(info, 0x80000001);

    this->FeatureSVM = info[2] & 8;
}

bool CheckSVMSupport()
{
    CpuInfo cpuInfo;

    std::cout << std::hex << "[*] MaxExtendedFunction " << cpuInfo.MaxExtendedFunction << std::endl;
    std::cout << "[*] Vendor: " << cpuInfo.Vendor << std::endl;
    if (cpuInfo.Vendor != "AuthenticAMD")
    {
        std::cout << "[-] SimpleHV is written only for AMD SVM." << std::endl;
        return false;
    }

    std::cout << "[*] SVM Feature Set Supported: " << cpuInfo.FeatureSVM << std::endl;
    if (!cpuInfo.FeatureSVM)
    {
        std::cout << "[-] SVM not supported on this processor." << std::endl;
        return false;
    }

    std::cout << "[+] SVM supported in hardware." << std::endl;
    return true;
}

int main()
{
    std::cout << "SimpleHV - AMD Edition" << std::endl;
    std::cout << "======================" << std::endl;

    if (!CheckSVMSupport())
    {
        std::cout << "CheckSVMSupport failed, terminating." << std::endl;
        return 1;
    }

    return 0;
}
