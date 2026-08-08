#include "../include/volume_manager.h"

#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#include <iostream>

#pragma comment(lib, "ole32.lib")

void VolumeManager::VolumeUp()
{
    keybd_event(VK_VOLUME_UP, 0, 0, 0);
    keybd_event(VK_VOLUME_UP, 0, KEYEVENTF_KEYUP, 0);

    std::cout << "Volume increased\n";
}

void VolumeManager::VolumeDown()
{
    keybd_event(VK_VOLUME_DOWN, 0, 0, 0);
    keybd_event(VK_VOLUME_DOWN, 0, KEYEVENTF_KEYUP, 0);

    std::cout << "Volume decreased\n";
}

void VolumeManager::Mute()
{
    keybd_event(VK_VOLUME_MUTE, 0, 0, 0);
    keybd_event(VK_VOLUME_MUTE, 0, KEYEVENTF_KEYUP, 0);

    std::cout << "Volume mute toggled\n";
}