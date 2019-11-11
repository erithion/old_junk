#ifndef _S3_PARSER_H_
#define _S3_PARSER_H_

#include <vector>
#include <string>

namespace parser_n
{
	class parser_c
	{
	public:
		typedef std::vector<unsigned char> raw_t;
	public:
		parser_c();
		~parser_c();
		bool open(const std::string& name);
		bool first();
		bool next(raw_t& raw);
		void close();
	protected:
	private:
		FILE*    _hfile;
	};
}
#endif