#ifndef _TYPES_HPP
#define _TYPES_HPP

#include <exception>
#include <string>

// Pre-declaration
namespace allocator_n
{
	template<typename T> class allocator_c;
}

#define _throw(exception)

namespace mcore_n
{
	/************************************************************************/
	/* Base exception class                                                 */
	/************************************************************************/
	class mcore_x : protected std::exception
	{
	public:
		mcore_x() : std::exception() 
		{ 
			__debugbreak(); 
		}
		virtual ~mcore_x()
		{
		}
	};
	/************************************************************************/
	/* Out of range exception class                                         */
	/************************************************************************/
	class out_of_range_x : public mcore_x
	{
	public:
		out_of_range_x() : mcore_x()
		{
		}
	};
	/************************************************************************/
	/* STL string with a custom allocator, that satisfies IDA memory model  */
	/************************************************************************/
	typedef 
		std::basic_string<char, 
		                  std::char_traits<char>, 
						  allocator_n::allocator_c<char> > string;
}
#endif