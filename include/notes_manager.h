#pragma once

#include <string>

class NotesManager
{
public:
    static void AddNote(const std::string &note);
    static void ShowNotes();
};