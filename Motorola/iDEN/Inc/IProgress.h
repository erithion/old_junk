#ifndef _I_PROGRESS_H_
#define _I_PROGRESS_H_

#include <string>

class IProgressIden
{
public:
    virtual void SetPercent(int percent) = 0;
    virtual void SetText(const std::wstring& text) = 0;
    virtual ~IProgressIden() = 0 {}
};

#endif //_I_PROGRESS_H_