#include "Game/Resource/Sound.hpp"
#include "Game/Utility/Exception.hpp"
#include <iostream>
namespace cge
{
	Sound::~Sound(void)
	{
		unload();
	}

	Sound::Sound( const std::string& path )
		: m_sample(NULL),
		m_gain(1.0f), m_speed(1.0f),
		m_panning(0.5f),m_instance(NULL),m_playing(false)
	{
		if(!load(path))
		{
			std::string s;
			s += "Sound at: ";
			s += path;
			s += " failed to load!";
			throw Exception(s);
		}
	}

	Sound::Sound()
		: m_sample(NULL),m_gain(1.0f), m_speed(1.0f),
		m_panning(0.5f),m_instance(NULL),m_playing(false)
	{

	}

	bool Sound::play( ALLEGRO_PLAYMODE playMode /*= ALLEGRO_PLAYMODE_ONCE*/ )
	{
		bool success = al_play_sample(m_sample,m_gain,m_panning,m_speed,playMode,&m_id);
		m_playing = success;
		return success;
	}

	bool Sound::play( float gain, float speed, float panning, ALLEGRO_PLAYMODE playMode /*= ALLEGRO_PLAYMODE_ONCE*/ )
	{
		bool success = al_play_sample(m_sample,gain,panning,speed,playMode,&m_id);
		m_playing = success;
		return success;
	}

	void Sound::stop()
	{
		if(m_playing)
		{
			al_stop_sample(&m_id);
			m_playing = false;
		}
	}

	ALLEGRO_SAMPLE* Sound::operator()() const
	{
		return m_sample;
	}

	ALLEGRO_SAMPLE* Sound::getSample() const
	{
		return m_sample;
	}

	bool Sound::load( const std::string& fileName )
	{
		ALLEGRO_SAMPLE* s = al_load_sample(fileName.c_str());
		if(s)
		{
			unload();
			m_instance = al_create_sample_instance(s);
			al_set_sample_instance_playmode(m_instance, ALLEGRO_PLAYMODE_ONCE); 
			al_attach_sample_instance_to_mixer(m_instance, al_get_default_mixer());
			m_sample = s;
			m_path = fileName;
			return true;
		}

		return false;
	}

	void Sound::unload()
	{
		m_path = "";
		if(m_instance)
		{
			al_destroy_sample_instance(m_instance);
			m_instance = NULL;
		}
		if(m_sample)
		{
			al_destroy_sample(m_sample);
			m_sample = NULL;
		}
	}

	bool Sound::isInstancePlaying() const
	{
		if(m_instance)
		return al_get_sample_instance_playing(m_instance);
		return false;
	}

	bool Sound::playInstance(float gain)
	{
		al_set_sample_instance_playmode(m_instance, ALLEGRO_PLAYMODE_ONCE);
		al_set_sample_instance_gain(m_instance, gain); 
		al_set_sample_instance_speed(m_instance, m_speed);
		al_set_sample_instance_pan(m_instance, m_panning); 
		return al_play_sample_instance(m_instance);
	}

	bool Sound::stopInstance()
	{
		return al_stop_sample_instance(m_instance);
	}

	const std::string& Sound::getPath() const
	{
		return m_path;
	}

}

