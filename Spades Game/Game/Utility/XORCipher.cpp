#include "Game/Utility/XORCipher.hpp"
#include <ctime>
namespace cge
{
	XORCipher::XORCipher(void)
		: m_keylength(128)
	{
		m_defaultKey = "01100110011101010110001101101011001000000110111101100110011001100010000001101110001100000011000001100010";
		m_numgen.init_genrand((long)(
			time(NULL)));
	}

	XORCipher::~XORCipher(void)
	{
	}

	std::string XORCipher::generateKey()
	{
		ss.clear();
		ss.str("");
	
		while((int)ss.str().length() < m_keylength)
		{
			ss << m_numgen.genrand_int32();
		}

		std::string key = ss.str().substr(0,m_keylength);
		for(size_t i = 0; i < key.length(); ++i)
		{
			key[i] += m_numgen.genrand_int32() % 80;
		}
		return key;
	}

	void XORCipher::encrypt( char* outBuffer,const char* inBuffer, int length, const std::string& key )
	{
		for(int i = 0; i < length; ++i)
		{
			outBuffer[i] = inBuffer[i] ^ key[i % key.length()] ^ m_defaultKey[i % m_defaultKey.length()];
		}
	}

	void XORCipher::decrypt( char* outBuffer,const char* inBuffer, int length, const std::string& key )
	{
		encrypt(outBuffer,inBuffer,length,key);
	}

}
