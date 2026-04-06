#pragma once

struct ConsoleCommand
{
    const char* name;
    const char* description;
    void (*function) (void);
};