#include "Game/Handler/SceneEventHandler.hpp"
#include "Game/Engine/DeviceManager.hpp"
#include "Game/Net/NetEventEncoder.hpp"
namespace cge
{
	SceneEventHandler::SceneEventHandler( NetEventEncoder* enc, DeviceManager* dev )
		: m_enc(enc),m_dev(dev)
	{

	}

	SceneEventHandler::~SceneEventHandler(void)
	{
	}

	void SceneEventHandler::playSound( const std::string& sound )
	{
		m_dev->getAudioManager()->play(sound);
	}

	void SceneEventHandler::stopSounds()
	{
		m_dev->getAudioManager()->stopAll();
	}

	void SceneEventHandler::lostConnectionCS()
	{
		if(!m_dev->getShared()->isDisconnectionPlanned())
		{
			if(m_dev->getShared()->getSceneMessenger()->getCurrentScene() != SceneManagerMessage::LOGIN)
			{
				if(m_dev->getShared()->getLastLoginError() != "logout.inactivity")
				{
					//did we come from the lobby or genuinely fail to login?
					if(m_dev->getShared()->getLastLoginError() == "logout")
						m_dev->getShared()->_setLastLoginError("logout.success");
					else if(m_dev->getShared()->getLastLoginError() != "server.full")
						m_dev->getShared()->_setLastLoginError("lost.connection");
				}

				m_dev->getShared()->getSceneMessenger()->sendSceneChangeMessage(SceneManagerMessage::LOGIN);
			}
	
			m_dev->getShared()->prepareLobbyChange();
		}
		m_dev->getShared()->setPlannedDisconnection(false);
	}

	void SceneEventHandler::playSoundOnce( const std::string& sound, float gain )
	{
		m_dev->getAudioManager()->playOnce(sound, gain);
	}

}
