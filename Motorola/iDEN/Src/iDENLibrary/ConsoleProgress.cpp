#include "ConsoleProgress.h"
#include <iostream>
#include <vector>
#include <string>

ConsoleProgress::ConsoleProgress()
{
}

ConsoleProgress::~ConsoleProgress()
{
}

void ConsoleProgress::SetPercent(int percent)
{
    std::cout << "    \r" << percent << "%\r";
}

void ConsoleProgress::SetText(const std::wstring& text)
{
    std::vector<unsigned char> buffer(text.size() + 1);
    wcstombs((char*)&buffer.front(), text.c_str(), buffer.size());
    std::string str(buffer.begin(), buffer.end());
    std::cout << str << std::endl;
}