#ifndef CGE_SCENE_EVENT_LISTENER_HPP
#define CGE_SCENE_EVENT_LISTENER_HPP
#include <stdlib.h>
#include <string>
#include <iostream>
#include "Game/Net/ClientShared.hpp"
#include "Game/UI/MessageBox.hpp"
#include "Game/Handler/MessageBoxResultReceiver.hpp"
namespace cge
{
	class SceneEventListener
	{
	public:
		SceneEventListener(void);
		virtual void playSound(const std::string& sound) {}
		virtual void playSoundOnce(const std::string& sound, float gain) {}
		virtual void stopSounds() {}
		//impl by scene
		virtual void setNumericSetting(const std::string& setting, float val) {}
		virtual void setBoolSetting(const std::string& setting, bool val) {}
		virtual void setStringSetting(const std::string& setting, const std::string& val) {}

		virtual bool getStringSettingImpl(const std::string&setting, std::string* val) {return false;}
		virtual bool getNumericSettingImpl(const std::string&setting, float* val) {return false;}
		virtual bool getBoolSettingImpl(const std::string&setting, bool* val) {return false;}

		virtual void loadSettings( ClientShared* shared ) {}
		virtual void showMessageBox(const std::string& title, const std::string& message, 
			MessageBox::MessageType type, MessageBoxResultReceiver* rec, int tag) {}
		virtual ~SceneEventListener(void);
	};
}

#endif
