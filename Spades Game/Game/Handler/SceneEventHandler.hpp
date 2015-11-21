#ifndef CGE_SCENE_EVENT_HANDLER_HPP
#define CGE_SCENE_EVENT_HANDLER_HPP
#include "Game/Handler/SceneEventProvider.hpp"
#include "Game/Net/NetEventListener.hpp"
namespace cge
{
	class NetEventEncoder;
	class DeviceManager;
	class SceneEventHandler : public SceneEventProvider,
		public NetEventListener
	{
		NetEventEncoder* m_enc;
		DeviceManager* m_dev;
	public:
		SceneEventHandler(NetEventEncoder* enc, DeviceManager* dev);
		virtual void playSound(const std::string& sound);
		virtual void playSoundOnce(const std::string& sound, float gain);
		virtual void stopSounds();
		virtual void lostConnectionCS();
		virtual ~SceneEventHandler(void);
	};
}
#endif
