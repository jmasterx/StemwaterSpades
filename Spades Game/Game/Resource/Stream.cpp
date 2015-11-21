#include "Game/Resource/Stream.hpp"

namespace cge
{
	Stream::~Stream(void)
	{
		al_destroy_audio_stream(m_stream);
	}

	Stream::Stream( const std::string& fileName,size_t bufferCount, size_t samples )
		: m_stream(NULL)
	{
		m_stream = al_load_audio_stream(fileName.c_str(),bufferCount,samples);
	}

	ALLEGRO_AUDIO_STREAM* Stream::operator()() const
	{
		return m_stream;
	}

	ALLEGRO_AUDIO_STREAM* Stream::getStream() const
	{
		return m_stream;
	}

}
