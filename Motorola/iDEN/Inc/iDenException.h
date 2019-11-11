#ifndef _IDEN_EXCEPTION_H_
#define _IDEN_EXCEPTION_H_

#include <string>
#include <exception>

class iDenException : public std::exception
{
public:
    iDenException 
      (
        const std::string& error = "Plugin.Error.Success", 
        const std::string& advice = "Plugin.Advice.None"
      ) : 
        std::exception(error.c_str()),
        error_(error), 
        advice_(advice)
    {}

    std::string GetError()
    {
        return error_;
    }

    std::string GetAdvice()
    {
        return advice_;
    }
private:
    std::string error_;
    std::string advice_;
};

class NotSupportedFeatureException : public std::exception
{
public:
    NotSupportedFeatureException() : std::exception("This feature is not supported yet") {}
};

#endif // _IDEN_EXCEPTION_H_
