#ifndef _CONSOLE_PROGRESS_H_
#define _CONSOLE_PROGRESS_H_

#include "./IProgress.h"

class ConsoleProgress : public IProgressIden
{
public:
    ConsoleProgress();
    void SetPercent(int percent);
    void SetText(const std::wstring& text);
    ~ConsoleProgress();
};

#endif //_CONSOLE_PROGRESS_H_