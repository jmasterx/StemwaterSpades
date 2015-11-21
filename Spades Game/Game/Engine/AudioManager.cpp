#include "Game/Engine/AudioManager.hpp"
#include <fstream>
#include <iostream>
#include "Game/Utility/File.hpp"
namespace cge
{
	float alerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}

	AudioManager::AudioManager( const std::string& path, const std::string& appendPath, SettingsManager* settings )
		: m_prevGain(-1.0f),m_settings(settings),m_stream(NULL),m_streamGain(1.0f),m_ambGain(-1.0f)
	{
		m_path = appendPath;
		if(isAudioInstalled())
		{
			if(al_reserve_samples(64))
			{
			}
			_loadSounds(path,appendPath);
		}
	}

	AudioManager::~AudioManager(void)
	{
		for(std::map<std::string,Sound*>::iterator it = 
			m_sounds.begin(); it != m_sounds.end(); ++it)
		{
			delete it->second;
		}

		for(std::list<Sound*>::iterator it = 
			m_oneTimeSounds.begin(); it != m_oneTimeSounds.end(); ++it)
		{
			delete (*it);
		}

		destroyStream();

		m_sounds.clear();
		m_oneTimeSounds.clear();
	}

	bool AudioManager::isAudioInstalled() const
	{
		return al_is_audio_installed();
	}

	float AudioManager::getGain()
	{
		if(isAudioInstalled())
		{
			return al_get_mixer_gain(getMixer());
		}

		return 0.0f;
	}

	ALLEGRO_MIXER* AudioManager::getMixer()
	{
		return al_get_default_mixer();
	}

	bool AudioManager::setMixerGain( float gain )
	{
		if(isAudioInstalled())
		{
			return al_set_mixer_gain(getMixer(),gain);
		}

		return false;
	}

	bool AudioManager::_loadSounds( const std::string& path, const std::string& appendPath )
	{
		//open the file
		File ifs( path );

		//ensure it is open
		if(!ifs.isOpen())
		{
			return false;
		}

		std::string line;

		//read each sound
		while( ifs.readLine(line) )
		{
			if(line.length() == 0)
			{
				continue;
			}
			else if(line.length() >= 1 && line[0] == '#')
			{
				continue;
			}

			char name[65], imgPath[129];

			int numArgsParsed =	sscanf(line.c_str(), 
				"%64s%*[^\"]%*c%128[^\"]%*c", 
				name, 
				imgPath);

			//probably an empty line
			if(numArgsParsed == 0)
			{
				continue;
			}

			if(!mapSound(std::string(name), appendPath + std::string(imgPath)))
			{
				Log::write("Audio Manager","Failed to map a sound, returning");
				return false;
			}
		}

		return true;
	}

	bool AudioManager::mapSound( const std::string& soundName, const std::string& soundPath )
	{
		Sound* s = new Sound();

		if(!s->load(soundPath))
		{
			delete s;
			return false;
		}

		if(m_sounds.find(soundName) != m_sounds.end())
		{
			delete s;
			return false;
		}

		m_sounds[soundName] = s;

		return true;
	}

	Sound* AudioManager::getSound( const std::string& name )
	{
		if(m_sounds.find(name) == m_sounds.end())
		{
			return NULL;
		}
		else
		{
			return m_sounds[name];
		}

	}

	bool AudioManager::play( const std::string& sound )
	{
		if(isAudioEnabled())
		{
			Sound* s = getSound(sound);
			if(s)
			{
				float masGain = m_settings->getNumericSetting("game.sounds");
				return s->play(masGain,1.0f,0.5f);
			}
		}

		return false;
	}

	bool AudioManager::play( const std::string& sound, float pitch )
	{
		if(isAudioEnabled())
		{
			Sound* s = getSound(sound);
			if(s)
			{
				float masGain = m_settings->getNumericSetting("game.sounds");
				return s->play(masGain,pitch,0.5f);
			}
		}

		return false;
	}

	void AudioManager::stopAll()
	{
		al_stop_samples();
		pauseStream();
	}

	void AudioManager::setAudioEnabled( bool on )
	{
		m_soundEnabled = on;
	}

	bool AudioManager::isAudioEnabled() const
	{
		return m_soundEnabled;
	}

	bool AudioManager::playOnce( const std::string& sound, float gain )
	{
		Sound* snd = new Sound(sound);
		m_oneTimeSounds.push_back(snd);
		float masGain = m_settings->getNumericSetting("gift.sounds");
		return snd->playInstance(gain * masGain);
	}

	void AudioManager::logic()
	{
		std::list<Sound*>::iterator i = m_oneTimeSounds.begin();
		while (i != m_oneTimeSounds.end())
		{
			bool isActive = (*i)->isInstancePlaying();
			if (!isActive)
			{
				delete (*i);
				m_oneTimeSounds.erase(i++); 
			}
			else
			{
				++i;
			}
		}

		float masGain = m_settings->getNumericSetting("master.volume");
		if(masGain != m_prevGain)
		{
			m_prevGain = masGain;
			setMixerGain(alerp(0.0f,2.4f,m_prevGain));
		}

		float ambGain = m_settings->getNumericSetting("music.ambiance");
		if(ambGain != m_ambGain)
		{
			m_ambGain = ambGain;
			if(m_stream)
			al_set_audio_stream_gain(m_stream,m_ambGain * m_streamGain);
		}
	}

	void AudioManager::destroyStream()
	{
		if(m_stream)
		{
			al_set_audio_stream_playing(m_stream,false);
			al_destroy_audio_stream(m_stream);
			m_stream = NULL;
		}
	}

	bool AudioManager::loadStream( const std::string& path )
	{
		destroyStream();
		m_stream = al_load_audio_stream(path.c_str(),4,1024);
		if(m_stream)
		{
			return al_attach_audio_stream_to_mixer(m_stream, getMixer());
		}

		return false;
	}

	bool AudioManager::playStream( const std::string& path, bool loop, float gain )
	{
		if(!loadStream(path))
			return false;

		m_streamGain = gain;
		al_set_audio_stream_gain(m_stream,gain * m_settings->getNumericSetting("music.ambiance"));
		if(loop)
			al_set_audio_stream_playmode(m_stream,ALLEGRO_PLAYMODE_LOOP);

		al_set_audio_stream_playing(m_stream,true);

		return true;
	}

	bool AudioManager::pauseStream()
	{
		if(!m_stream)
			return false;

		return al_set_audio_stream_playing(m_stream,false);
	}

	bool AudioManager::resumeStream()
	{
		if(!m_stream)
			return false;

		return al_set_audio_stream_playing(m_stream,true);
	}

	const std::string& AudioManager::getPath() const
	{
		return m_path;
	}

	void AudioManager::stop( const std::string& sound )
	{
		Sound* s = getSound(sound);
		if(s)
		{
			s->stop();
		}
	}

	bool AudioManager::playLoop( const std::string& sound )
	{
		if(isAudioEnabled())
		{
			Sound* s = getSound(sound);
			if(s)
			{
				float masGain = m_settings->getNumericSetting("game.sounds");
				return s->play(masGain,1.0f,0.5f,ALLEGRO_PLAYMODE_LOOP);
			}
		}

		return false;
	}

}
