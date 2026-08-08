#pragma once

#include <string>

class ReminderManager
{
public:
    static void AddReminder(const std::string &reminder);
    static void ShowReminders();
};