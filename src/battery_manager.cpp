#include "../include/battery_manager.h"

#include <windows.h>
#include <iostream>

void BatteryManager::ShowBattery()
{
    SYSTEM_POWER_STATUS status{};

    if (!GetSystemPowerStatus(&status))
    {
        std::cout << "Unable to get battery information\n";
        return;
    }

    std::cout << "\n========== BATTERY ==========\n";

    if (status.BatteryLifePercent == 255)
    {
        std::cout << "Battery     : Unknown\n";
    }
    else
    {
        std::cout << "Battery     : "
                  << static_cast<int>(status.BatteryLifePercent)
                  << "%\n";
    }

    if (status.ACLineStatus == 1)
    {
        std::cout << "Power       : Charging / AC Connected\n";
    }
    else
    {
        std::cout << "Power       : On Battery\n";
    }

    std::cout << "=============================\n";
}