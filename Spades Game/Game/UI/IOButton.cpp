#include "Game/UI/IOButton.hpp"
#include "Game/Engine/Scene.hpp"
#include "Game/Platform/Platf.hpp"
#define MSG_QUIT_GAME 1
namespace cge
{
	IOButton::~IOButton(void)
	{
		m_button->removeMouseListener(this);
		m_ioMenu->removeActionListener(this);
		m_ioMenu->clearItems();
		m_button->removeWidgetListener(this);
	}

	IOButton::IOButton( GuiFactory* factory, LanguageManager* language, Display* display,
		AudioManager* audio, SceneManagerMessage* message, Scene* parentScene )
		: m_display(display), m_language(language), m_scene(message),
		m_autoRelocate(false),m_relocatePaddingX(5),m_relocatePaddingY(5),m_audio(audio),m_listener(NULL),
		m_parentScene(parentScene),m_directQuit(false),m_mbCalled(false)
	{
		m_soundOptions = new SoundOptionsDialog(factory);
		m_ioMenu = factory->createPopUpMenu();
		addDefaultItems();
		m_quitItem.setText(language->getElement("nav.quit"));
		m_leaveItem.setText(language->getElement("game.leave"));
		m_soundItem.setText(language->getElement("sound.options"));
		m_reportItem.setText(language->getElement("report.problem"));
		m_button = factory->createWhiteButton("io");
        if(!Platf::isRetina())
		m_button->setSize(30,30);
        else
        {
            m_button->setSize(75,75);
            m_button->setScaleIcon(true);
            m_button->setImageScale(0.7f);
        }
		m_button->setToggleButton(true);
		m_button->addMouseListener(this);
		m_button->addWidgetListener(this);
		m_button->setMargins(2,4,6,6);

		m_ioMenu->addActionListener(this);
		m_ioMenu->setInvokeButton(m_button);

		m_button->setFocusable(false);
		m_ioMenu->setFocusable(false);
	}

	void IOButton::mouseClickCB( agui::MouseEvent& evt )
	{
		if(evt.getSourceWidget() == m_button && evt.getButton() == agui::MOUSE_BUTTON_LEFT)
		{
			if(m_button->isToggled())
			{
				if(!m_display->isFullscreen())
				{
					m_fullscreenItem.setText(m_language->getElement("nav.fullscreen"));
				}
				else
				{
					m_fullscreenItem.setText(m_language->getElement("nav.windowed"));
				}

				m_ioMenu->showPopUp(m_button,evt.getX(),evt.getY());
			}
		}
	}

	Button* IOButton::getWidget()
	{
		return m_button;
	}

	void IOButton::actionPerformed( const agui::ActionEvent &evt )
	{
		if(evt.getSource() == &m_fullscreenItem)
		{
			if(false && m_display->isIntelGMA4000())
			{
				DISPATCH_SCENE_EVENT
					(*it)->showMessageBox(m_language->getElement("restart.required.title"),m_language->getElement("restart.required.text"),MessageBox::MT_OK,NULL,99);
				DISPATCH_SCENE_EVENT
					(*it)->setBoolSetting("display.fullscreen",!m_display->isFullscreen());
			}
			else
			{
				m_display->toggleFullscreen(!m_display->isFullscreen());
				DISPATCH_SCENE_EVENT
					(*it)->setBoolSetting("display.fullscreen",m_display->isFullscreen());
			}
		}

		if(evt.getSource() == &m_soundItem)
		{
			if(m_parentScene->isGeneralDialogBusy())
			{
				DISPATCH_SCENE_EVENT
					(*it)->showMessageBox(m_language->getElement("sound.options.close.title"),
					m_language->getElement("sound.options.close.text"),MessageBox::MT_OK,NULL,99);
			}
			else
			{
				m_parentScene->showGeneralDialog(getSoundOptions());
			}
			
		}

		if(evt.getSource() == &m_quitItem)
		{
			processQuitRequest();
		}

		if(m_listener)
		{
			m_listener->actionPerformed(evt);
		}
	}


	void IOButton::setAutoRelocate( bool relocate, int paddingX /*= 5*/,int paddingY /*= 5*/ )
	{
		m_autoRelocate = relocate;
		m_relocatePaddingX = paddingX;
		m_relocatePaddingY = paddingY;
        
        if(Platf::isRetina()) {
            m_relocatePaddingY *= 2.0f;
            m_relocatePaddingX *= 2.0f;
        }
	}

	void IOButton::parentSizeChanged( agui::Widget* source, const agui::Dimension &parentInnerSize )
	{
		if(m_autoRelocate)
		{
			m_button->setLocation(
				parentInnerSize.getWidth() - 
				m_button->getWidth() - m_relocatePaddingX, m_relocatePaddingY);
		}
	}

	int IOButton::getRelocatePaddingX() const
	{
		return m_relocatePaddingX;
	}

	int IOButton::getRelocatePaddingY() const
	{
		return m_relocatePaddingY;
	}

	agui::PopUpMenuItem& IOButton::getLeaveItem()
	{
		return m_leaveItem;
	}

	void IOButton::setWantLeaveItem( bool want )
	{
		m_ioMenu->clearItems();

		if(want)
		{
			addItemsWithLeave();
		}
		else
		{
			addDefaultItems();
		}
	}

	void IOButton::addDefaultItems()
	{
		m_ioMenu->clearItems();

        if(!Platf::isMobile())
		m_ioMenu->addItem(&m_fullscreenItem);
		m_ioMenu->addItem(&m_soundItem);
        if(!Platf::isMobile())
		m_ioMenu->addItem(&m_quitItem);
	}

	void IOButton::addItemsWithLeave()
	{
		m_ioMenu->clearItems();
        if(!Platf::isMobile())
		m_ioMenu->addItem(&m_fullscreenItem);
		m_ioMenu->addItem(&m_soundItem);
		m_ioMenu->addItem(&m_leaveItem);
        if(!Platf::isMobile())
		m_ioMenu->addItem(&m_quitItem);
	}

	void IOButton::setActionListener( agui::ActionListener* listener )
	{
		m_listener = listener;
	}

	SoundOptionsDialog* IOButton::getSoundOptions()
	{
		return m_soundOptions;
	}

	void IOButton::messageBoxResult( MessageBox* msgbox,MessageBox::MessageResult result )
	{
		if(msgbox->getTag() == MSG_QUIT_GAME && result == MessageBox::MR_YES)
		{
			m_scene->sendQuitMessage();
		}
		
		if(msgbox->getTag() == MSG_QUIT_GAME)
			m_mbCalled = false;
	}

	void IOButton::setDirectQuit( bool direct )
	{
		m_directQuit = direct;
	}

	void IOButton::processQuitRequest()
	{
		if(m_mbCalled)
			return;

		if(!m_directQuit)
		{
			DISPATCH_SCENE_EVENT
				(*it)->showMessageBox(m_language->getElement("confirm.title"),
				m_language->getElement("confirm.quit.game"),MessageBox::MT_YES_NO,this,MSG_QUIT_GAME);
			m_mbCalled = true;
		}
		else
		{
			m_scene->sendQuitMessage();
		}
	}

	void IOButton::addItemsWithReport()
	{
		m_ioMenu->clearItems();
         if(!Platf::isMobile())
		m_ioMenu->addItem(&m_fullscreenItem);
		m_ioMenu->addItem(&m_soundItem);
		m_ioMenu->addItem(&m_reportItem);
         if(!Platf::isMobile())
		m_ioMenu->addItem(&m_quitItem);
	}

	void IOButton::setWantReportItem( bool want )
	{
		m_ioMenu->clearItems();

		if(want)
		{
			addItemsWithReport();
		}
		else
		{
			addDefaultItems();
		}
	}

	agui::PopUpMenuItem& IOButton::getReportItem()
	{
		return m_reportItem;
	}

}
