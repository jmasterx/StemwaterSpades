#ifndef CGE_INITIAL_SCENE_HPP
#define CGE_INITIAL_SCENE_HPP
#include "Game/Engine/Scene.hpp"
namespace cge
{
	class InitialScene : public Scene
	{
	protected:
		virtual void actionPerformed(const agui::ActionEvent &evt);
	public:
		InitialScene(void);
		virtual void processEvent(ALLEGRO_EVENT* evt, bool &handled);
		virtual void resizeEvent(int width, int height);
		virtual void render();
		virtual void sceneBegin();
		virtual ~InitialScene(void);
	};
}

#endif

