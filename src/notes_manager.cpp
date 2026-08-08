#include "../include/notes_manager.h"

#include <fstream>
#include <iostream>

void NotesManager::AddNote(const std::string &note)
{
    std::ofstream file("../../data/notes.txt", std::ios::app);

    if (!file)
    {
        std::cout << "Unable to open notes file\n";
        return;
    }

    file << note << '\n';

    std::cout << "Note saved successfully\n";
}

void NotesManager::ShowNotes()
{
    std::ifstream file("../../data/notes.txt");

    if (!file)
    {
        std::cout << "No notes found\n";
        return;
    }

    std::string note;

    std::cout << "\n===== NOTES =====\n";

    while (std::getline(file, note))
    {
        std::cout << "- " << note << '\n';
    }
}