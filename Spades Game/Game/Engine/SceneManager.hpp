#ifndef CGE_SCENE_MANAGER_HPP
#define CGE_SCENE_MANAGER_HPP 
#include "Game/platform.hpp"
#include "Game/Engine/DeviceManager.hpp"
#include "Game/UI/DynamicUIManager.hpp"
#include "Game/Engine/SceneManagerMessage.hpp"
#include "Game/Engine/Scene.hpp"
#ifdef CGE_IPHONE
#include "Game/Platform/KeyboardMediator.h"
#endif
namespace cge
{
    class Core;
	class SceneManager : public SceneManagerMessage
	{
		ALLEGRO_EVENT evt;
		ALLEGRO_EVENT nextEvt;
		ALLEGRO_EVENT_QUEUE *queue;
		Timer* m_gameTimer;
		DeviceManager *m_devices;
		DynamicUIManager* m_dynamicUI;
		bool m_needsRedraw;
		float m_frameRate;
		GraphicsContext m_g;
		int m_newScreenWidth;
		int m_newScreenHeight;
		bool m_needsResize;
		bool m_transitioning;
		float m_transitionOpacity;
		float m_transitionRate;
		bool m_canDraw;
        bool m_needFirstRender;
        bool m_callOnNextLogic;
        int m_nextLogicFrames;
        bool m_wasSignedIn;
        bool m_appResumed;
        Core* m_core;
        bool m_bgexecWasHalted;
        float m_bgTime;
        bool m_bgExecing;

		Scene* m_currentScene;
		SceneEnum m_nextScene;
		SceneEnum m_currentSceneType;
		bool m_gameIsRunning;
		GuiImageManager m_guiImageManager;
		bool m_needsTransition;
		bool m_cannotRender;
		void registerEventSources();
		void processMessages();
		void changeScene();
		void defaultBeginEventHandler(ALLEGRO_EVENT* evt);
		void defaultEndEventHandler(ALLEGRO_EVENT* evt);
		void startTransition();
        
#ifdef CGE_IPHONE
        KeyboardMediator* m_keyMan;
#endif
        
        float m_accelX;
        float m_accelY;
        float m_accelZ;
	public:
		SceneManager(DeviceManager* settings, DynamicUIManager* dynamicUImanager);
		void run();
		virtual void sendSceneChangeMessage(SceneEnum scene);
		virtual void sendQuitMessage();
		virtual void sendResizeMessage(int w, int h);
		virtual SceneEnum getCurrentScene() const;
		virtual bool willSceneChange() const;
		virtual void sendContextChangedMessage();
		virtual void sendContextWillChangedMessage();
        void setCore(Core* core);
        void backgroundExecutionStarted();
        void backgroundExecutionEnded(bool forced);
		virtual ~SceneManager(void);
	};

}

#endif
