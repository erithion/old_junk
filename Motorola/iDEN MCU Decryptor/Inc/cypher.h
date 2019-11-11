#ifndef _CYPHER_H
#define _CYPHER_H

#include <vector>

namespace cypher_n
{
	/************************************************************************/
	/* Decryption class for Motorola i90c MCU                               */
	/************************************************************************/
	class irl_c
	{
	public:
		typedef std::vector<unsigned char> raw_vt;
	public:
		irl_c();
		/************************************************************************/
		/* Decrypts encrypted content of Motorola S3 Record                     */
		/*   raw - an array of encrypted content of a single row.               */
		/*         Encrypted content for i90c is a set of bytes,                */
		/*         directly followed after S[N] header and finished by 0xA      */
		/* Returns a decrypted row                                              */
		/* NOTE:                                                                */
		/*   There is no any checking of the fact whether the row is incorrect. */
		/*   You should take care of this. The algorithm was not checked        */
		/*   against any other iDEN MCU except i90c.                            */
		/************************************************************************/
		raw_vt decrypt(const raw_vt& raw);
	private:
		// Initial key for decryption. 
		// It was taken from IRL.DLL of i90c upgrade utility
		static const unsigned char _constIrlKey[];
		// Internal key. Required during decryption
		static unsigned char       _cypher[0x50];
	};
}
#endif