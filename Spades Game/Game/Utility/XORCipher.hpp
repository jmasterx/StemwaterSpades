#ifndef CGE_XOR_CIPHER_HPP
#define CGE_XOR_CIPHER_HPP
#include "Game/Utility/MersenneTwister.hpp"
#include <stdlib.h>
#include <string>
#include <sstream>
namespace cge
{
	class XORCipher
	{
		int m_keylength;
		MersenneTwister m_numgen;
		std::stringstream ss;
		std::string m_defaultKey;
		std::string m_tempKey;
	public:
		XORCipher(void);
		std::string generateKey();
		void encrypt(char* outBuffer,const char* inBuffer,
			int length, const std::string& key);
		void decrypt(char* outBuffer,const char* inBuffer,
			int length, const std::string& key);
		~XORCipher(void);
	};
}

#endif
