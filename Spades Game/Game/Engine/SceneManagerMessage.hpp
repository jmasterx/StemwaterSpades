#ifndef CGE_SCENE_MANAGER_MESSAGE_HPP
#define CGE_SCENE_MANAGER_MESSAGE_HPP


namespace cge
{
	class SceneManagerMessage
	{
	public:
		enum SceneEnum {
			NO_SCENE, //Do not actually use this -_-'
			INITIAL_SCENE,
			MENU,
			GAME,
			LOBBY,
			LOGIN
		};
		virtual void sendSceneChangeMessage(SceneEnum scene) = 0;
		virtual void sendQuitMessage() = 0;
		virtual void sendResizeMessage(int w, int h) = 0;
		virtual void sendContextWillChangedMessage() = 0;
		virtual void sendContextChangedMessage() = 0;
		virtual SceneEnum getCurrentScene() const = 0;
		virtual bool willSceneChange() const = 0;
		SceneManagerMessage(void);
		virtual ~SceneManagerMessage(void);
	};
}
#endif
