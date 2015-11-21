#ifndef CGE_AUDIO_MANAGER_HPP
#define CGE_AUDIO_MANAGER_HPP
#include "Game/platform.hpp"
#include <map>
#include <list>
#include "Game/Resource/Sound.hpp"
#include "Game/Client/SettingsManager.hpp"
namespace cge
{
	class AudioManager
	{
		std::string m_path;
		ALLEGRO_MIXER* getMixer();
		std::map<std::string,Sound*> m_sounds;
		std::list<Sound*> m_oneTimeSounds;
		bool m_soundEnabled;
		bool _loadSounds(const std::string& path, const std::string& appendPath);
		bool mapSound(const std::string& soundName, const std::string& soundPath);
		float m_prevGain;
		SettingsManager* m_settings;
		ALLEGRO_AUDIO_STREAM* m_stream;
		float m_streamGain;
		float m_ambGain;
		void destroyStream();
		bool loadStream(const std::string& path);
	public:
		AudioManager(const std::string& path, const std::string& appendPath, SettingsManager* settings);
		bool isAudioInstalled() const;
		float getGain();
		bool setMixerGain(float gain);
		Sound* getSound(const std::string& name);
		bool play(const std::string& sound);
		bool play(const std::string& sound, float pitch);
		bool playLoop(const std::string& sound);
		void stop(const std::string& sound);
		bool playOnce(const std::string& sound, float gain);
		void stopAll();
		bool isAudioEnabled() const;
		void setAudioEnabled(bool on);
		void logic();
		bool playStream(const std::string& path, bool loop, float gain);
		bool pauseStream();
		bool resumeStream();
		const std::string& getPath() const;
		~AudioManager(void);
	};
}

#endif
