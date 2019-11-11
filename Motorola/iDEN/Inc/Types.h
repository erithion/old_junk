#ifndef _TYPES_H_
#define _TYPES_H_

#include <vector>
#include <exception>

typedef unsigned char             uchar;
typedef unsigned short            ushort;
typedef unsigned int              uint;
typedef unsigned long             ulong;
typedef std::vector<uchar>        uchar_vt;
typedef std::vector<char>         char_vt;
typedef std::vector<uint>         uint_vt;
typedef std::pair<size_t, size_t> pair;

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

	std::string getError()
	{
		return error_;
	}

	std::string getAdvice()
	{
		return advice_;
	}
private:
	std::string error_;
	std::string advice_;
};

class notSupportedFeatureException : public std::exception
{
public:
	notSupportedFeatureException() : std::exception("This feature is not supported yet") {}
};


#endif //_TYPES_H_