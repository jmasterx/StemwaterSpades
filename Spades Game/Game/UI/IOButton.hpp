#ifndef CGE_IO_BUTTON_HPP
#define CGE_IO_BUTTON_HPP
#include "Game/UI/GuiFactory.hpp"
#include "Game/Engine/Display.hpp"
#include "Game/Engine/SceneManagerMessage.hpp"
#include "Game/Engine/LanguageManager.hpp"
#include "Agui/MouseListener.hpp"
#include "Agui/ActionListener.hpp"
#include "Agui/Widgets/PopUp/PopUpMenuItem.hpp"
#include "Game/Engine/AudioManager.hpp"
#include "Game/Handler/SceneEventProvider.hpp"
#include "Game/Dialog/SoundOptionsDialog.hpp"
namespace cge
{
	class Scene;
	class IOButton : public agui::MouseListener, public agui::ActionListener,
		public agui::WidgetListener, public SceneEventProvider,
		public MessageBoxResultReceiver
	{
		Button* m_button;
		agui::PopUpMenu* m_ioMenu;
		agui::PopUpMenuItem m_fullscreenItem;
		agui::PopUpMenuItem m_leaveItem;
		agui::PopUpMenuItem m_quitItem;
		agui::PopUpMenuItem m_soundItem;
		agui::PopUpMenuItem m_reportItem;
		LanguageManager* m_language;
		agui::ActionListener* m_listener;
		Display* m_display;
		AudioManager* m_audio;
		SceneManagerMessage* m_scene;
		SoundOptionsDialog* m_soundOptions;
		bool m_autoRelocate;
		int m_relocatePaddingX;
		int m_relocatePaddingY;
		Scene* m_parentScene;
		bool m_directQuit;
		bool m_mbCalled;
		void addDefaultItems();
		void addItemsWithLeave();
		void addItemsWithReport();
	public:
		IOButton(GuiFactory* factory, LanguageManager* language,
			Display* display,AudioManager* audio, SceneManagerMessage* message, Scene* parentScene);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void mouseClickCB(agui::MouseEvent& evt);
		Button* getWidget();
		void setAutoRelocate(bool relocate, int paddingX = 2,int paddingY = 5);
		virtual void parentSizeChanged(agui::Widget* source, const agui::Dimension &parentInnerSize);
		int getRelocatePaddingX() const;
		int getRelocatePaddingY() const;
		agui::PopUpMenuItem& getLeaveItem();
		void setWantLeaveItem(bool want);
		void setWantReportItem(bool want);
		void setActionListener(agui::ActionListener* listener);
		SoundOptionsDialog* getSoundOptions();
		virtual void messageBoxResult(MessageBox* msgbox,MessageBox::MessageResult result);
		void setDirectQuit(bool direct);
		void processQuitRequest();
		agui::PopUpMenuItem& getReportItem();
		virtual ~IOButton(void);
	};
}



#endif