// S3SuperFileParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <io.h>
#include <string>
#include <stdlib.h>
#include <direct.h>
#include "s3_parser.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc > 1)
	{
		if (strcmp(argv[1], "/?") == 0)
		{
			std::cout << "Options:\n/? - Help\n/p <S3 SuperFile Name>\n";
		}
		else if (strcmp(argv[1], "/p") == 0 &&
			argc > 2)
		{
			std::string in;
			std::string name = argv[2];
			char        drive[_MAX_DRIVE];
			char        dir[_MAX_DIR];
			char        fname[_MAX_FNAME];
			char        ext[_MAX_EXT];
			::_splitpath(argv[0], drive, dir, fname, ext);
			name = (std::string)drive + (std::string)dir;
			::_splitpath(argv[2], drive, dir, fname, ext);
			name += (std::string)fname + "\\";
			::_splitpath(argv[0], drive, dir, fname, ext);
			in = (std::string)drive + (std::string)dir + argv[2];
			if(::_mkdir(name.c_str()) == 0)
			{
				parser_n::parser_c p;
				if (p.open(in.c_str()) &&
					p.first())
				{
					int i = 1;
					parser_n::parser_c::raw_t raw;
					while(p.next(raw))
					{
						char dg[20];
						::itoa(i, dg, 10);
						std::string filename = name + (std::string)dg + ".s3";
						FILE* hfile = ::fopen(filename.c_str(), "w+b");
						::fwrite((void*)&raw.front(), raw.size(), 1, hfile);
						::fflush(hfile);
						::fclose(hfile);
						++i;
					}
				}
			}
		}
		else
		{
			std::cout << "Please, type /? for Help\n";
		}
	}
	else
	{
		std::cout << "Please, type /? for Help\n";
	}
	return 0;
}

