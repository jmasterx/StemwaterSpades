#include "Game/Handler/SceneEventProvider.hpp"
#include <algorithm>
namespace cge
{
	SceneEventProvider::SceneEventProvider(void)
	{
	}

	SceneEventProvider::~SceneEventProvider(void)
	{
	}

	void SceneEventProvider::addSceneListener( SceneEventListener* listener )
	{
		m_sceneListeners.push_back(listener);
	}

	void SceneEventProvider::removeSceneListener( SceneEventListener* listener )
	{
		m_sceneListeners.erase(
			std::remove(m_sceneListeners.begin(),
			m_sceneListeners.end(), listener),
			m_sceneListeners.end());
	}

	void SceneEventProvider::clearSceneListeners()
	{
		m_sceneListeners.clear();
	}

	float SceneEventProvider::getNumericSetting( const std::string&setting )
	{
		float r = 0.0f;
		DISPATCH_SCENE_EVENT
		{
			if((*it)->getNumericSettingImpl(setting,&r))
			{
				return r;
			}
		}

		return r;
	}

	bool SceneEventProvider::getBoolSetting( const std::string&setting )
	{
		bool r = false;
		DISPATCH_SCENE_EVENT
		{
			if((*it)->getBoolSettingImpl(setting,&r))
			{
				return r;
			}
		}

		return r;
	}

	std::string SceneEventProvider::getStringSetting( const std::string&setting )
	{
		std::string r = "";
		DISPATCH_SCENE_EVENT
		{
			if((*it)->getStringSettingImpl(setting,&r))
			{
				return r;
			}
		}

		return r;
	}

}
