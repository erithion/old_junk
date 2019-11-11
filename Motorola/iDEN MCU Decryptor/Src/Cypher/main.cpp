#include "cypher.h"

/************************************************************************/
/* The set of exported symbols from cypher.asm                          */
/* Cypher.asm contains procedures and symbols, which are required       */
/* for decryption. They were taken from IRL.DLL of i90c upgrade utility */
/************************************************************************/
extern "C" unsigned char* keyOffset;
extern "C" int cypherInit(unsigned char* lpInitialKey, int nSize);
extern "C" int cypherPostInit();
extern "C" int cypherDecrypt(unsigned char* lpPos, int dwSize, unsigned char* nKeyVal);
extern "C" int cypherReset(unsigned char* KeyPart);

using namespace cypher_n;

/************************************************************************/
/* The initial key for decryption                                       */
/************************************************************************/
const unsigned char irl_c::_constIrlKey[] = 
{
   0x46, 
   0xCA, 
   0x32, 
   0xDF, 
   0xAC, 
   0x82, 
   0x91, 
   0x8E
};
unsigned char irl_c::_cypher[0x50];

irl_c::irl_c()
{
	keyOffset = irl_c::_cypher;
	cypherInit((unsigned char*)irl_c::_constIrlKey, 0x38);
	cypherPostInit();
}
irl_c::raw_vt irl_c::decrypt(const raw_vt& raw)
{
	raw_vt out(raw);

	cypherReset(keyOffset + 0x37);

	size_t size = out.size() % 2 == 0 ? out.size() : out.size() + 1;

	out.resize(size, 0);

	cypherDecrypt(&out.front(), (int)size, irl_c::_cypher + 0x37);
	
	out.resize(raw.size());

	return out;
}