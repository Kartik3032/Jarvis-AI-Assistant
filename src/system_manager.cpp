#include "../include/system_manager.h"

#include <iostream>
#include <windows.h>
#include <winreg.h>

void SystemManager::SystemInfo()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    // ---------------- Computer Name ----------------
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName);

    GetComputerNameA(computerName, &size);

    // ---------------- CPU Name ----------------
    HKEY hKey;
    char cpuName[256] = "Unknown";
    DWORD cpuSize = sizeof(cpuName);

    if (RegOpenKeyExA(
            HKEY_LOCAL_MACHINE,
            "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
            0,
            KEY_READ,
            &hKey) == ERROR_SUCCESS)
    {
        RegQueryValueExA(
            hKey,
            "ProcessorNameString",
            NULL,
            NULL,
            (LPBYTE)cpuName,
            &cpuSize);

        RegCloseKey(hKey);
    }

    // ---------------- RAM ----------------
    MEMORYSTATUSEX mem;
    mem.dwLength = sizeof(mem);

    GlobalMemoryStatusEx(&mem);

    double totalRAM =
        mem.ullTotalPhys / (1024.0 * 1024 * 1024);

    double availRAM =
        mem.ullAvailPhys / (1024.0 * 1024 * 1024);

    // ---------------- Battery ----------------
    SYSTEM_POWER_STATUS battery;

    GetSystemPowerStatus(&battery);

    // ---------------- Output ----------------

    std::cout << "\n========== SYSTEM INFORMATION ==========\n\n";

    std::cout << "Computer Name        : "
              << computerName << '\n';

    std::cout << "Processor            : "
              << cpuName << '\n';

    std::cout << "Architecture         : ";

    switch (si.wProcessorArchitecture)
    {
    case PROCESSOR_ARCHITECTURE_AMD64:
        std::cout << "x64 (64-bit)";
        break;

    case PROCESSOR_ARCHITECTURE_INTEL:
        std::cout << "x86 (32-bit)";
        break;

    case PROCESSOR_ARCHITECTURE_ARM:
        std::cout << "ARM";
        break;

    default:
        std::cout << "Unknown";
    }

    std::cout << '\n';

    std::cout << "Processors           : "
              << si.dwNumberOfProcessors << '\n';

    std::cout << "Total RAM            : "
              << totalRAM << " GB\n";

    std::cout << "Available RAM        : "
              << availRAM << " GB\n";

    if (battery.BatteryLifePercent != 255)
    {
        std::cout << "Battery              : "
                  << (int)battery.BatteryLifePercent
                  << "%\n";
    }
    else
    {
        std::cout << "Battery              : Unknown\n";
    }

    std::cout << "Charging             : "
              << ((battery.ACLineStatus == 1) ? "Yes" : "No")
              << '\n';

    std::cout << "\n========================================\n";
}