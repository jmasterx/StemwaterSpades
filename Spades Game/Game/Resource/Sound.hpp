#ifndef CGE_SOUND_HPP
#define CGE_SOUND_HPP
#include <stdlib.h>
#include <string>
#include "Game/platform.hpp"
namespace cge
{
	class Sound
	{
		ALLEGRO_SAMPLE* m_sample;
		ALLEGRO_SAMPLE_ID m_id;
		ALLEGRO_SAMPLE_INSTANCE* m_instance;
		float m_gain;
		float m_speed;
		float m_panning;
		bool m_playing;
		std::string m_path;
	public:
		Sound(const std::string& fileName);
		Sound();
		bool load(const std::string& fileName);
		void unload();
		ALLEGRO_SAMPLE* operator()() const;
		ALLEGRO_SAMPLE* getSample() const;
		bool play(ALLEGRO_PLAYMODE playMode = ALLEGRO_PLAYMODE_ONCE);
		bool playInstance(float gain);
		bool stopInstance();
		bool play(float gain, float speed, float panning, ALLEGRO_PLAYMODE playMode = ALLEGRO_PLAYMODE_ONCE);
		void stop();
		const std::string& getPath() const;
		bool isInstancePlaying() const;
		virtual ~Sound(void);
	};
}

#endif
