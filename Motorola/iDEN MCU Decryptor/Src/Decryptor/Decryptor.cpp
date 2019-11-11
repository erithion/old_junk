// Decryptor.cpp : Defines the entry point for the console application.
//

#include "cypher.h"

#include <tchar.h>
#include <fstream>
#include <string>

/************************************************************************/
/* Converts the hex symbol representation into its appropriate hex value*/
/************************************************************************/
cypher_n::irl_c::raw_vt toHex(const std::string& s)
{
	using std::string;
	using cypher_n::irl_c::raw_vt;
	using std::string::const_iterator;

	raw_vt         raw;
	string         str(s);
	const_iterator cur = s.begin();
	const_iterator end = s.end();

	while (!str.empty())
	{
		unsigned long k = ::strtoul(str.substr(0, 2).c_str(), 0, 16);
		str.swap(str.substr(2));
		raw.push_back((unsigned char)k);
	}
	return raw;
}
/************************************************************************/
/* Converts hex value into its appropriate hex symbol representation    */
/************************************************************************/
std::string toString(const cypher_n::irl_c::raw_vt& raw)
{
	using std::string;
	using cypher_n::irl_c::raw_vt::const_iterator;

	string         res("");

	const_iterator cur(raw.begin());
	const_iterator end(raw.end());

	while (cur != end)
	{
		char str[3];
		::sprintf(str, "%02X", *cur);
		res += str;
		++cur;
	}
	return res;
}

int _tmain(int argc, _TCHAR* argv[])
{
	using std::ifstream;
	using std::ofstream;
	using std::string;
	using std::getline;

	using cypher_n::irl_c;
	using cypher_n::irl_c::raw_vt;

	ifstream in("su");
	ofstream out("su.decrypted");

	if (in)
	{
		string line("");
		irl_c c;

		while (getline(in, line))
		{
			raw_vt raw = toHex(line.substr(2));

			raw = c.decrypt(raw);

			out << (line.substr(0, 2) + toString(raw)) + '\xA';
		}
	}
	return 0;
}

