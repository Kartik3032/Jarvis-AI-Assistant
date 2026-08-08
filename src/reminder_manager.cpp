#include "../include/reminder_manager.h"

#include <fstream>
#include <iostream>

void ReminderManager::AddReminder(const std::string &reminder)
{
    std::ofstream file("../../data/reminders.txt", std::ios::app);

    if (!file)
    {
        std::cout << "Unable to open reminders file\n";
        return;
    }

    file << reminder << '\n';

    std::cout << "Reminder saved successfully\n";
}

void ReminderManager::ShowReminders()
{
    std::ifstream file("../../data/reminders.txt");

    if (!file)
    {
        std::cout << "No reminders found\n";
        return;
    }

    std::string reminder;

    std::cout << "\n===== REMINDERS =====\n";

    while (std::getline(file, reminder))
    {
        std::cout << "- " << reminder << '\n';
    }
}