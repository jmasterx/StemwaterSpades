#include "Game/Engine/SceneManager.hpp"
#include "Game/Scene/InitialScene.hpp"
#include "Game/Scene/GameScene.hpp"
#include "Game/Scene/LobbyScene.hpp"
#include "Game/Scene/LoginScene.hpp"
#include "Game/Utility/StringUtil.hpp"
#include "Game/Engine/Core.hpp"
#ifdef CGE_IPHONE
#include "Game/platform/IAPHandler.h"
#endif

namespace cge
{
	SceneManager::SceneManager( DeviceManager* settings, DynamicUIManager* dynamicUImanager )
	: m_devices(settings),m_gameIsRunning(true), m_nextScene(LOGIN),
	m_currentScene(NULL),m_needsRedraw(true),m_frameRate(60.0f),
	m_dynamicUI(dynamicUImanager), m_currentSceneType(NO_SCENE),
	m_needsResize(false),m_newScreenHeight(0),m_newScreenWidth(0),
	m_transitioning(false),m_transitionOpacity(1.0f),m_transitionRate(0.05f), m_needsTransition(false),
	m_canDraw(true),m_accelX(0.0f),m_accelY(0.0f),m_accelZ(0.0f), m_needFirstRender(true), m_callOnNextLogic(false),
	m_guiImageManager("res/gui/gui_image_mapping.conf","res/gui/"),m_nextLogicFrames(0),m_wasSignedIn(false),
    m_appResumed(false),m_bgexecWasHalted(false),m_bgExecing(false),m_bgTime(0.0f)
	{
#ifdef CGE_IPHONE
        m_keyMan = new KeyboardMediator();
        m_keyMan->showKeyboard();
        m_keyMan->hideKeyboard();
#endif
        
		queue = al_create_event_queue();
		m_gameTimer = al_create_timer(1.0 / m_frameRate);
		registerEventSources();
		//Instance the first scene
		processMessages();
		Log::write("Scene Manager","Scene Manager Started");

        if(!Platf::isMobile())
		m_g.setBuffering(settings->getDisplay()->getMaxTextureSize() >= 2048);
        else
            m_g.setBuffering(false);
            
	}

	SceneManager::~SceneManager(void)
	{
		//delete the current scene
		if(m_currentScene)
		{
			m_currentScene->sceneEnd();
			delete m_currentScene;
			m_currentScene = NULL;
		}

		al_destroy_timer(m_gameTimer);
		al_destroy_event_queue(queue);
#ifdef CGE_IPHONE
        delete m_keyMan;
#endif
		Log::write("Scene Manager","Scene Manager Destroyed");
	
	}

	void SceneManager::run()
	{	
		m_devices->setSceneMessenger(this);
#ifdef CGE_MOBILE
       //ALLEGRO_JOYSTICK *joystick = NULL;
       // al_reconfigure_joysticks();
        //joystick=al_get_joystick(al_get_num_joysticks()-1);
#endif
        
		al_start_timer(m_gameTimer);

		//is the event handled?
		bool handled = false;
		ALLEGRO_EVENT next;
		bool skip = false;
		double numFrames = 1;
		double totalDelta = 0.0f;

		int frames = 0;
		double starttime = 0;
		bool first = true;
		float fps = 0.0f;
        
		//main loop
		while(m_gameIsRunning)
		{
			handled = false;
			al_wait_for_event(queue,&evt);

			bool hasNext = al_peek_next_event(queue,&next);
			if(hasNext && next.type == ALLEGRO_EVENT_TIMER)
			{
				al_drop_next_event(queue);
			}
			//render the scene
			if(m_canDraw && m_needsRedraw && al_is_event_queue_empty(queue))
			{
				if(skip)
					skip = false;
				else
				{
					double a = al_get_time();

					if(m_currentScene->canRender())
					{
						m_g.begin();
						m_currentScene->render();
						/*
						m_g.drawText(StringUtil::toString(fps),m_devices->getFontManager()->getDefaultFont(),agui::Color(0,0,0),-2,-2,0);
						m_g.drawText(StringUtil::toString(fps),m_devices->getFontManager()->getDefaultFont(),agui::Color(0,0,0),2,2,0);
						m_g.drawText(StringUtil::toString(fps),m_devices->getFontManager()->getDefaultFont(),agui::Color(255,255,255),0,0,0);
						*/
						if(m_needsTransition)
						{
							m_needsTransition = false;
							m_transitioning = true;
						}
						if(m_transitioning)
							m_g.drawTintedSprite(m_g.getBuffer(),
							Color(m_transitionOpacity,m_transitionOpacity,m_transitionOpacity,m_transitionOpacity),0,0,0);
						m_g.end();
					}

					double t = al_get_time();
					if (first)
					{
						frames = 0;
						starttime = t;
						first = false;
						continue;
					}
					frames++;
					if (t - starttime > 0.25 && frames > 10)
					{
						fps = (double) frames / (t - starttime);
						starttime = t;
						frames = 0;
					}
                    
					m_needsRedraw = false;
                    
                    if(!m_transitioning && m_needFirstRender) {
                        m_needFirstRender = false;
                        m_callOnNextLogic = true;
                        m_nextLogicFrames = 0;
                    }

					double delta = al_get_time() - a;
					totalDelta += delta;
					numFrames++;
					double avgDelta = totalDelta / numFrames;

					if(numFrames > 100)
					{
						numFrames = 0;
						totalDelta = 0;
					}
                    if(avgDelta > (1.0f / 40.0f) || Platf::isMini())
						skip = true;
				}
			}

			defaultBeginEventHandler(&evt);
			m_currentScene->processEvent(&evt,handled);

			//do default behavior if event was not handled by the scene
			if (!handled)
			{
				defaultEndEventHandler(&evt);
			}
			processMessages();
		}
	}

	void SceneManager::registerEventSources()
	{
		al_register_event_source(queue, al_get_keyboard_event_source());
		al_register_event_source(queue, al_get_mouse_event_source());
		al_register_event_source(queue, al_get_timer_event_source(m_gameTimer));
		al_register_event_source(queue, al_get_display_event_source(m_devices->getDisplay()->getContext()));
#ifdef CGE_MOBILE
		al_register_event_source(queue, al_get_touch_input_event_source());
		al_register_event_source(queue, al_get_touch_input_mouse_emulation_event_source());
        //al_register_event_source(queue, al_get_joystick_event_source());
#endif
		Log::write("Scene Manager","Event sources registered");
	}

	void SceneManager::sendSceneChangeMessage( SceneEnum scene )
	{
		m_nextScene = scene;
	}

	void SceneManager::sendQuitMessage()
	{
		m_gameIsRunning = false;
		if(m_currentSceneType != SceneManagerMessage::LOGIN)
		{
			m_devices->getLobbyNetHandler()->logout(false);
		}
	}

	void SceneManager::processMessages()
	{
		changeScene();
	}

	void SceneManager::changeScene()
	{
		if(m_nextScene != NO_SCENE)
		{
			//delete the current scene
			if(m_currentScene)
			{
				if(m_g.isBuffering())
				{
					startTransition();
				}

#ifdef CGE_MOBILE
                m_keyMan->hideKeyboard();
#endif
				m_currentScene->sceneEnd();
				m_currentScene->deinitBaseScene();
#ifdef CGE_MOBILE
                m_keyMan->setScene(NULL);
                m_currentScene->getGui().removeTapListener(m_keyMan);
#endif
				delete m_currentScene;
				m_currentScene = NULL;
				Log::write("Scene Manager","Scene Ended");
			}

			m_currentSceneType = m_nextScene;
	
			//instance the requested scene
			switch(m_nextScene)
			{
			case SceneManagerMessage::GAME:
				m_currentScene = new GameScene();
				Log::write("Scene Manager","Changed to Game Scene");
				break;
			case SceneManagerMessage::LOBBY:
				m_currentScene = new LobbyScene();
				Log::write("Scene Manager","Changed to Lobby Scene");
				break;
			case SceneManagerMessage::LOGIN:
				m_currentScene = new LoginScene();
				Log::write("Scene Manager","Changed to Login Scene");
				break;
			}

            m_needFirstRender = true;
			m_nextScene = NO_SCENE;

			//set some class pointers
			m_currentScene->setGraphics(&m_g);
			m_currentScene->setDeviceManager(m_devices);
            
			m_currentScene->setDynamicUIManager(m_dynamicUI);
			m_currentScene->setGuiImageManager(&m_guiImageManager);
			m_currentScene->setSceneMessenger(this);
			m_currentScene->setGameTimer(m_gameTimer);
			m_currentScene->initBaseScene();
#ifdef CGE_MOBILE
            m_keyMan->setScene(m_currentScene);
            m_currentScene->getGui().addTapListener(m_keyMan);
            m_currentScene->getGui().setFocusEnabled(false);
#endif
			m_currentScene->sceneBegin();
			m_currentScene->resizeEvent(
            m_devices->getDisplay()->getResolution().getX(),
			m_devices->getDisplay()->getResolution().getY());
			m_currentScene->initialResizeCompleted();
			m_currentScene->sceneLogic();
			m_currentScene->logic();
            
            if(m_appResumed)
            {
                m_currentScene->appHasReturned(m_wasSignedIn);
                m_wasSignedIn = false;
                m_appResumed = false;
            }
		}
	}

	void SceneManager::defaultEndEventHandler( ALLEGRO_EVENT* evt )
	{
		switch(evt->type)
		{
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			{
				if(!(m_currentScene != NULL && m_currentSceneType != NO_SCENE && m_currentScene->processQuitRequest()))
					sendQuitMessage();
			}
			break;
		default:
			break;
		}
	}

	void SceneManager::defaultBeginEventHandler( ALLEGRO_EVENT*evt )
	{
		m_currentScene->processGuiInputEvent(evt);

		if(evt->type == ALLEGRO_EVENT_TIMER && evt->timer.source == m_gameTimer)
		{
			if(m_transitioning)
			{
				m_transitionOpacity -= m_transitionRate;

				if(m_transitionOpacity <= 0)
				{
					m_transitionOpacity = 0;
					m_transitioning = false;
				}
			}
			m_needsRedraw = true;
            if(!m_needFirstRender)
            {
                m_devices->getAudioManager()->logic();
                m_currentScene->sceneLogic();
                m_currentScene->processGuiLogic();
                m_currentScene->logic();
#ifdef CGE_IPHONE
                m_keyMan->dequeue();
#endif
                m_devices->getNetClient()->tick();
                
                if(m_bgExecing)
                {
                    m_bgTime += (1.0f/60.0f);
                    if(m_bgTime > 25.0f)
                    {
#ifdef CGE_IPHONE
                        IAPHandler::getInstance().endBackgroundExecution(this);
                        m_bgexecWasHalted = true;
                        m_devices->getNetClient()->setBGMode(false);
                        m_devices->getNetClient()->destroy();
                        Sprite::destroyAll();
                        m_wasSignedIn = m_currentSceneType != LOGIN;
#endif
                    }
                }
            }
            
            if(m_callOnNextLogic) {
                m_nextLogicFrames++;
                if(m_nextLogicFrames >= 5) {
                m_callOnNextLogic = false;
                    m_nextLogicFrames = 0;
                m_currentScene->sceneReady();
                }
            }
		}
		else if(evt->type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN)
		{
			m_currentScene->windowGotFocus();
              printf("GOT FOCUS\n");
		}
		else if(evt->type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT)
		{
			m_currentScene->windowLostFocus();
			printf("LOST FOCUS\n");
		}
		else if(evt->type == ALLEGRO_EVENT_DISPLAY_HALT_DRAWING)
		{
			m_canDraw = false;
			m_wasSignedIn = m_currentSceneType != LOGIN;
			m_devices->getNetClient()->destroy();
			al_acknowledge_drawing_halt(al_get_current_display());
			printf("HALT DRAWING\n");
			//exit(0);
		}
		else if(evt->type == ALLEGRO_EVENT_DISPLAY_RESUME_DRAWING)
		{
			m_canDraw = true;
			m_devices->getNetClient()->regenerate();


			al_acknowledge_drawing_resume(al_get_current_display());
			m_core->renderLoading(0);
			al_rest(0.5);
			if(m_nextScene == NO_SCENE && m_currentScene) {
				m_currentScene->appHasReturned(m_wasSignedIn);
				m_wasSignedIn = false;
				m_appResumed = false;
			}
			else if(m_nextScene != NO_SCENE) {
				m_appResumed = true;
			}
			else {
				m_wasSignedIn = false;
				m_appResumed = false;
			}
			printf("RESUME DRAWING\n");
		}
		else if(evt->type == ALLEGRO_EVENT_DISPLAY_RESIZE)
		{
			m_needsResize = true;
			m_newScreenWidth = evt->display.width;
			m_newScreenHeight = evt->display.height;
			sendResizeMessage(m_newScreenWidth,m_newScreenHeight);
              printf("RESIZE\n");
		}
        else if(evt->type == ALLEGRO_EVENT_DISPLAY_LOST) {
             printf("DEVICE LOST\n");
        }
        else if(evt->type == ALLEGRO_EVENT_DISPLAY_FOUND) {
            printf("DEVICE FOUND\n");
        }
        else if(evt->type == ALLEGRO_EVENT_DISPLAY_CONNECTED) {
            printf("DEVICE CONNECTED\n");
        }
        else if(evt->type == ALLEGRO_EVENT_DISPLAY_DISCONNECTED) {
            printf("DEVICE DISCONNECTED\n");
        }
        else if(evt->type == ALLEGRO_EVENT_DISPLAY_EXPOSE) {
            printf("DEVICE EXPOSE\n");
        }
        else if(evt->type == ALLEGRO_EVENT_JOYSTICK_AXIS){
#ifdef CGE_MOBILE
                if(evt->joystick.axis == 0){ //float joys[3][2]
                    m_accelX = evt->joystick.pos;
                }
                else if(evt->joystick.axis == 1) {
                    m_accelY = evt->joystick.pos;
                }
                else if(evt->joystick.axis == 2) {
                    m_accelZ = evt->joystick.pos;
                }
            
            if(m_currentScene)
                m_currentScene->accelerometerChanged(m_accelX, m_accelY, m_accelZ);
            
#endif
        }

		else if(m_needsResize)
		{
			m_needsResize = false;
		}
	}

	void SceneManager::sendResizeMessage( int w, int h )
	{
		//this is a bug in Allegro
		if(w == 0 && h == 0)
		{
			return;
		}

		if(!Platf::isMobile() && w >= 1900)
			Platf::_setHD(true);
		else
			Platf::_setHD(false);

		al_acknowledge_resize(m_devices->getDisplay()->getContext());
		m_g.resizeBuffer(w,h);
		//stop transition
		m_transitioning = false;
		m_transitionOpacity = 0.0f;
		m_currentScene->processGuiResizeEvent();
		m_currentScene->resizeEvent(
			w,h);
		Log::write("Scene Manager","Resize Event: Width:" +  StringUtil::toString(w) + 
			" Height:" + StringUtil::toString(h));
	}

	SceneManagerMessage::SceneEnum SceneManager::getCurrentScene() const
	{
		return m_currentSceneType;
	}

	bool SceneManager::willSceneChange() const
	{
		return m_nextScene != NO_SCENE;
	}

	void SceneManager::startTransition()
	{
		m_g.setTargetToBuffer();
		m_g.begin();
		m_currentScene->render();
		m_g.setTargetToBackbuffer();
		m_transitioning = false;
		m_needsTransition = true;
		m_transitionOpacity = 1.0f;
	}
    
    void SceneManager::setCore(Core* core) {
        m_core = core;
    }

	void SceneManager::sendContextChangedMessage()
	{
		al_register_event_source(queue, al_get_display_event_source(m_devices->getDisplay()->getContext()));
	}

	void SceneManager::sendContextWillChangedMessage()
	{
		al_unregister_event_source(queue, al_get_display_event_source(m_devices->getDisplay()->getContext()));
	}
    
    void SceneManager::backgroundExecutionStarted()
    {
        m_bgexecWasHalted = false;
        m_devices->getNetClient()->setBGMode(true);
        m_bgTime = 0.0f;
        m_bgExecing = true;
    }
    
    void SceneManager::backgroundExecutionEnded(bool forced)
    {
        m_bgexecWasHalted = forced;
        m_bgTime = 0.0f;
        m_bgExecing = false;
        
        if(forced)
        {
            m_devices->getNetClient()->setBGMode(false);
            m_devices->getNetClient()->destroy();
            Sprite::destroyAll();
             m_wasSignedIn = m_currentSceneType != LOGIN;
        }
    }

}
