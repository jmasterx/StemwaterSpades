#include "Game/Handler/NotificationHandler.hpp"
#include "Game/Utility/TimeOfDay.hpp"
#include <iostream>
#include <sstream>
namespace cge
{

	NotificationHandler::NotificationHandler( GuiFactory* factory,Button* button, agui::Gui* gui )
		: m_factory(factory),m_button(button),m_numNewNotifications(0),m_gui(gui),
		m_handlingInvites(false),m_language(factory->getLanguageManager())
	{
		if(m_button)
		{
			m_button->addActionListener(this);
			m_button->setToggleButton(true);
		}

		m_list = factory->createNotficationListBox();
		m_list->addSelectionListener(this);
		m_list->setVisibility(false);
		m_list->resizeToContents();

		gui->addMousePreviewListener(this);
	}


	NotificationHandler::~NotificationHandler(void)
	{
		if(m_button)
		{
			m_button->removeActionListener(this);
		}

		m_list->removeSelectionListener(this);

		m_gui->removeMousePreviewListener(this);
	}

	NotificationListBox* NotificationHandler::getWidget()
	{
		return m_list;
	}

	void NotificationHandler::actionPerformed( const agui::ActionEvent &evt )
	{
		if(evt.getSource() == m_button)
		{
			if(m_button->isToggled())
			{
				setNumNewNotifications(0);
				showNotifications();
			}
			else
			{
				hideNotifications();
			}
		}
	}

	void NotificationHandler::setNumNewNotifications( int num )
	{
		m_numNewNotifications = num;
		if(m_numNewNotifications < 0)
		{
			m_numNewNotifications = 0;
		}

		if(m_numNewNotifications == 0)
		{
			if(m_button)
			{
				m_button->setText("");
			}
		}
		else
		{
			std::stringstream ss;
			ss << num;
			m_button->setText(ss.str());
		}
	}

	void NotificationHandler::increaseNumNewNotifications( int q /*= 1*/ )
	{
		setNumNewNotifications(m_numNewNotifications + q);
	}

	void NotificationHandler::showNotifications()
	{
		m_list->bringToFront();
		m_list->setLocation(m_button->getAbsolutePosition().getX(),
			m_button->getAbsolutePosition().getY() + (m_button->getHeight() * 0.93f));
		m_list->setVisibility(true);
	}

	void NotificationHandler::hideNotifications()
	{
		m_list->setVisibility(false);
		if(m_button)
		{
			m_button->setToggleState(false);
		}
	}

	void NotificationHandler::selectionChanged( 
		agui::Widget *source, const std::string &item, int index, bool selected )
	{
		if(source == m_list)
		{
			NotificationItem* itm = m_list->getSelectedNotification();

			if(itm)
			{
				notificationDispatched(itm);

				DISPATCH_LOBBY_EVENT
					(*it)->notificationDispatched(itm);

				m_list->removeNotification(itm);
				m_list->resizeToContents();
				hideNotifications();
			}
		}
	}

	void NotificationHandler::mouseDownCB( agui::MouseEvent &mouseEvent )
	{
		
		m_gui->_widgetLocationChanged();
		agui::Widget* wum = m_gui->getWidgetUnderMouse();

		if(wum != m_list && m_list->isVisible() && wum != m_button)
		{
			hideNotifications();
		}
	}

	void NotificationHandler::setHandlingInvites( bool handling )
	{
		m_handlingInvites = handling;
	}

	void NotificationHandler::notificationDispatched( NotificationItem* item )
	{
		if(!m_handlingInvites || item->getType() != NotificationItem::NOTIFY_INVITE)
		{
			return;
		}

		LobbyTable* t = getLobbyTable(item->getTableNo());

		if(!t)
		{
			return;
		}

		DISPATCH_LOBBY_EVENT
			(*it)->focusOnLobbyTable(item->getTableNo(),true);
	}

	void NotificationHandler::pushNotification( const NotificationItem& item )
	{
		addNotification(item);
	}

	void NotificationHandler::addNotification( const NotificationItem& item )
	{
		m_list->addNotification(item);
		m_list->resizeToContents();

		if(!m_list->isVisible())
		{
			increaseNumNewNotifications();
		}
		
	}

	void NotificationHandler::removeNotification( NotificationItem* item )
	{
		m_list->removeNotification(item);
		m_list->resizeToContents();
		increaseNumNewNotifications(-1);
	}

	void NotificationHandler::popNotification( NotificationItem* item )
	{
		removeNotification(item);
	}

	void NotificationHandler::cleanTable( int tableNo )
	{
		bool found = false;

		do 
		{
			found = false;
			for (agui::ListItem::const_iterator it = m_list->getItemsBegin(); 
				it != m_list->getItemsEnd(); ++it)
			{
				NotificationItem* itm = (NotificationItem*)it->first.tag;

				if(itm && itm->getTableNo() == tableNo 
					&& itm->getType() == NotificationItem::NOTIFY_INVITE)
				{
					removeNotification(itm);
					found = true;
					break;
				}
			}

		} while (found);
	}

	void NotificationHandler::tableIsEmpty( int tableId )
	{
		cleanTable(tableId);
	}

	void NotificationHandler::tableIsFull( int tableId )
	{
		cleanTable(tableId);
	}

	void NotificationHandler::invitationReceived( int tableNo,const std::string& player )
	{
		if(!m_handlingInvites)
		{
			return;
		}

		for (agui::ListItem::const_iterator it = m_list->getItemsBegin(); 
			it != m_list->getItemsEnd(); ++it)
		{
			NotificationItem* itm = (NotificationItem*)it->first.tag;

			if(itm && itm->getTableNo() == tableNo 
				&& itm->getType() == NotificationItem::NOTIFY_INVITE && itm->getUsername() == player)
			{
				return;
			}
		}

		NotificationItem item;
		item.setType(NotificationItem::NOTIFY_INVITE);
		item.setUsername(player);
		item.setUsernameColor(getPlayerColor(player));
		item.setTableNo(tableNo);

		std::stringstream ss;
		ss << tableNo;
		
		std::string inv = m_language->getElement("notification.invitation");
		inv += ss.str();

		LobbyTable* table = getLobbyTable(tableNo);
		if(!table)
		{
			return;
		}
		if(table->isEmpty())
		{
			return;
		}
		
		item.setFirstText(inv);
		item.setSecondText(generateTableInfo(table->getGameInfo()));
		item.setThirdText(TimeOfDay::getShortTime());

		addNotification(item);
	}

	void NotificationHandler::sitPlayerAtTable( 
		int tableNumber, int seatNumber, const std::string& playerName )
	{
		if(isSelfPlayer(playerName))
		{
			cleanTable(tableNumber);
		}
	}

	std::string NotificationHandler::generateTableInfo( const SpadesGameInfo& info )
	{
		std::stringstream ss;

		std::string table;

		std::string gType;

		if(info.isPassTwo())
		{
			gType = m_language->getElement("filter.pass");
		}
		else if(info.isPartnership())
		{
			gType = m_language->getElement("filter.regular");
		}
		else if(info.isCutthroat())
		{
			gType = m_language->getElement("filter.cutthroat");
		}
		else if(info.isIndividual())
		{
			gType = m_language->getElement("filter.individual");
		}

		if(info.isMirror() || info.isSuicide())
		{
			gType += " ";
		}
		

		if(info.isMirror())
		{
			gType += m_language->getElement("filter.mirrorbid");
		}
		else if(info.isSuicide())
		{
			gType += m_language->getElement("filter.suicidebid");
		}

		ss << gType; //ex: Partnership Mirror
		ss << " - ";

		std::string rated = info.isSocial() ?  
			m_language->getElement("filter.social") : m_language->getElement("filter.rated");
		ss << rated;
		ss << " - ";
	

		std::string scoreRT;
		if(info.isScoreEnded())
		{
			scoreRT = m_language->getElement("points");
		}
		else if(info.isTimeEnded())
		{
			scoreRT = m_language->getElement("minutes");
		}
		else if(info.isHandEnded())
		{
			scoreRT = m_language->getElement("hands");
		}

		std::string scoreR = toString(info.getEndValue()) + " " + scoreRT;
		ss << scoreR;

		ss << " - ";
		ss << m_language->getElement("game.nil");
		ss << " ";
		std::string nilVal = toString(info.m_nilValue);
		ss << nilVal << " " << m_language->getElement("points");


		std::string fee = m_language->getElement("fee");
		fee += " ";
		std::stringstream sstr;
		if(info.getFee() > 0)
		{
			sstr << info.getFee();
			fee += m_language->getElementWithArg("n.coins",sstr.str());
		}
		else
		{
			sstr << m_language->getElement("no.fee");
			fee += sstr.str();
		}

		ss << " - ";
		ss << fee;

		/*
		ss << " - ";

		ss << m_language->getElement("game.deckcut");
		ss << " ";
		std::string cutDeckOn = info.isDeckCutEnabled() ? 
			m_language->getElement("on") : m_language->getElement("off");
		ss << cutDeckOn;

		ss << " - ";

		std::string watching = m_language->getElement("game.watching");
		std::string watchOn = info.isWatchingAllowed() ? 
			m_language->getElement("on") : m_language->getElement("off");
		ss << watching << " " << watchOn;

		*/

		return ss.str();
	}

	std::string NotificationHandler::toString( int i ) const
	{
		std::stringstream ss;
		ss << i;
		return ss.str();
	}

}

