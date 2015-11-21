#ifndef CGE_STREAM_HPP
#define CGE_STREAM_HPP
#include <stdlib.h>
#include <string>
#include "Game/platform.hpp"
namespace cge
{
	class Stream
	{
		ALLEGRO_AUDIO_STREAM* m_stream;
	public:
		ALLEGRO_AUDIO_STREAM* operator()() const;
		ALLEGRO_AUDIO_STREAM* getStream() const;
		Stream(const std::string& fileName,size_t bufferCount, size_t samples);
		virtual ~Stream(void);
	};
}

#endif

