#ifndef CGE_SCENE_EVENT_PROVIDER_HPP
#define CGE_SCENE_EVENT_PROVIDER_HPP
#include <vector>
#include "Game/Handler/SceneEventListener.hpp"

namespace cge
{
	#define DISPATCH_SCENE_EVENT for(std::vector<SceneEventListener*>::iterator it =  m_sceneListeners.begin(); it != m_sceneListeners.end(); ++it)
	class SceneEventProvider : public SceneEventListener
	{
	protected:
		std::vector<SceneEventListener*> m_sceneListeners;
	public:
		virtual void addSceneListener(SceneEventListener* listener);
		virtual void removeSceneListener(SceneEventListener* listener);
		virtual void clearSceneListeners();
		virtual float getNumericSetting(const std::string&setting);
		virtual bool getBoolSetting(const std::string&setting);
		virtual std::string getStringSetting(const std::string&setting);
		SceneEventProvider(void);
		virtual ~SceneEventProvider(void);
	};
}

#endif
