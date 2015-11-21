#ifndef CGE_SCENE_HPP
#define CGE_SCENE_HPP
#include "Game/Engine/SceneManagerMessage.hpp"
#include "Game/Engine/DeviceManager.hpp"
#include "Game/UI/DynamicUIManager.hpp"
#include "Agui/Agui.hpp"
#include "Agui/ActionListener.hpp"
#include "Agui/WidgetListener.hpp"
#include "Agui/Backends/Allegro5/Allegro5.hpp"
#include "Game/UI/GuiFactory.hpp"
#include "Game/UI/CardFrame.hpp"
#include "Game/UI/GeneralFrame.hpp"
#include "Game/UI/GeneralDialogBox.hpp"
#include "Game/Handler/SceneEventProvider.hpp"
#include <vector>
namespace cge
{
	struct MessageBoxQueuedMessage
	{
		std::string title;
		std::string message;
		MessageBox::MessageType type;
		MessageBoxResultReceiver* receiver;
		int tag;
		bool shouldShow;
	};
	class Scene : public agui::ActionListener, public agui::WidgetListener,
		public SceneEventProvider
	{
		Timer* m_gameTimer;
		DeviceManager* m_settings;
		DynamicUIManager* m_dynamicUI;
		SceneManagerMessage* m_sceneMessenger;
		std::vector<MessageBoxQueuedMessage> m_boxes;
        bool m_mobileKeyboardMode;
		agui::Gui m_gui;
        agui::Gui m_guiOverlay;
		agui::Allegro5Input m_a5GuiInput;
		agui::Allegro5Graphics m_a5GuiGraphics;
		agui::Allegro5CursorProvider m_a5GuiCursor;
        
        agui::Allegro5Input m_a5GuiInputOverlay;
        agui::Allegro5Graphics m_a5GuiGraphicsOverlay;
        agui::Allegro5CursorProvider m_a5GuiCursorOverlay;
		GeneralDialogBox* m_generalContent;
		GuiFactory m_guiFactory;
		CardFrame* m_frame;
		GeneralFrame* m_generalFrame;
		MessageBoxContainer* m_msgbox; //do not free
		GraphicsContext *m_g;
        TextField* m_txtOverlay;
        TextBox* m_txtBoxOverlay;
        Label* m_lblOverlay;
        ToolContainer* m_emoticonsContainer;
        FlowLayout* m_emoticonsFlow;
        std::vector<Button*> m_emoticonButtons;
        int m_numEmoticonsPerLine;
        void resizeOverlayWidgets();
        void resizeEmoticons();
	protected:
		std::vector<SceneEventProvider*> m_sceneEventProviders;
		std::vector<SceneEventListener*> m_sceneEventListeners;
		virtual void actionPerformed(const agui::ActionEvent &evt);
		
	public:
		void _addSceneEventProvider(SceneEventProvider* provider);
		void _addSceneEventListener(SceneEventListener* listener);
		void linkSceneListeners();
		void unlinkSceneListeners();
		void setGraphics(GraphicsContext* g);
        virtual GraphicsContext* getGraphics();
		CardFrame* getCardDialog();
		GeneralFrame* getGeneralDialog();
		virtual void initBaseScene();
		virtual void deinitBaseScene();
		virtual bool showCardDialog(agui::Widget* content,agui::ActionListener* actionReceiver, 
			const std::string& title,
			bool modal = true,const std::string& cancelName = "",
			const std::string& okName = "");
		virtual bool showGeneralDialog(GeneralDialogBox* content);
		Scene(void);
		agui::Gui& getGui();
		GuiFactory& getGuiFactory();
		void processGuiInputEvent(ALLEGRO_EVENT* evt);
		void processGuiResizeEvent();
		void processGuiLogic();
		void processGuiRendering();
		virtual void resizeEvent(int width, int height);
		//process an Allegro Event
		virtual void processEvent(ALLEGRO_EVENT* evt, bool& handled) = 0;
		//called when the scene is created
		virtual void sceneBegin();
		//called when the scene is destroyed
		virtual void sceneEnd();
		virtual void render();
		void sceneLogic();
		virtual void logic();
		virtual void cleanup();
		virtual void initialResizeCompleted();
		void setDeviceManager(DeviceManager* settings);
		void setDynamicUIManager(DynamicUIManager* dynamicUI);
		void setGuiImageManager(GuiImageManager* manager);
		void setSceneMessenger(SceneManagerMessage* messenger);
		void setGameTimer(Timer* timer);
		SceneManagerMessage* getSceneMessenger() const;
		DeviceManager* getDeviceManager() const;
		DynamicUIManager* getDynamicUIManager() const;
		Timer* getGameTimer() const;

		virtual void setNumericSetting(const std::string& setting, float val);
		virtual void setBoolSetting(const std::string& setting, bool val);
		virtual void setStringSetting(const std::string& setting, const std::string& val);

		virtual bool getStringSettingImpl(const std::string&setting, std::string* val);
		virtual bool getNumericSettingImpl(const std::string&setting, float* val);
		virtual bool getBoolSettingImpl(const std::string&setting, bool* val);

		virtual void showMessageBox(const std::string& title, const std::string& message, MessageBox::MessageType type, MessageBoxResultReceiver* rec, int tag);
		virtual void showQueuedMessageBoxes();
		virtual void hideMessageBox();
		void hideGeneralDialog();
		virtual int getMessageBoxTag() const;
		virtual bool isMessageBoxBusy() const;
		virtual bool isGeneralDialogBusy() const;
		virtual void dequeueMessageBoxesWithTag(int tag);
		virtual void visibilityChanged(agui::Widget* widget, bool visible);
		virtual agui::Widget* getIOButton();
		virtual bool processQuitRequest();
		virtual void windowGotFocus();
		virtual void windowLostFocus();
		virtual bool canRender() const;
        virtual void accelerometerChanged(float x, float y, float z) {}
        void setMobileKeyboardMode(bool on, bool textField, bool needsEmoticons, const std::string& overlayText);
        TextField* getOverlayTextField() const;
        TextBox* getOverlayTextBox() const;
        void toggleOverlayEmoticonsContainer(bool on);
        virtual void sceneReady() {}
        virtual void appHasReturned(bool wasSignedIn) {}
		virtual ~Scene(void);
	};
}

#endif

