#include "Game/Handler/ChatListHandler.hpp"
namespace cge
{
	ChatListHandler::ChatListHandler( GuiFactory* factory )
		: m_factory(factory),m_list(NULL),m_container(NULL),
		m_layout(NULL)
	{
		m_container = m_factory->createToolContainer();
		m_container->setMargins(4,4,4,4);
		m_list = m_factory->createCollapsibleList();
		m_layout = m_factory->createBorderLayout();
		m_container->add(m_layout);
		m_layout->add(m_list,agui::BorderLayout::CENTER);
	}

	ChatListHandler::~ChatListHandler(void)
	{
		for(size_t i = 0; i < m_groups.size(); ++i)
		{
			m_groups[i].first->removeActionListener(this);
		}

	}

	CollapsibleList* ChatListHandler::getGroupList()
	{
		return m_list;
	}

	agui::Widget* ChatListHandler::getContainer()
	{
		return m_container;
	}

	void ChatListHandler::addGroup( const std::string& groupName, 
		const agui::Color& groupColor, GroupTypeEnum type )
	{
		CollapsibleButton* button = m_factory->createCollapsibleButton();
		button->setFontColor(groupColor);
		button->getList()->setFontColor(groupColor);
		button->setText(groupName);
		button->resizeToContents();
		m_list->addGroup(button);
		button->addActionListener(this);
		m_groups.push_back(std::make_pair(button,type));
		button->getList()->addMouseListener(this);
		button->getList()->setAllowRightClickSelection(true);
        if(!Platf::isRetina())
        {
            button->setFont(m_factory->getFontManager()->getFont(0.95f));
            button->getList()->setFont(m_factory->getFontManager()->getFont(0.9f));
        }
        else
        {
            float retinaAdd = 1.75f;
            button->setFont(m_factory->getFontManager()->getFont(retinaAdd));
            button->resizeToContents();
            button->setFont(m_factory->getFontManager()->getFont(0.95f * retinaAdd));
            button->getList()->setFont(m_factory->getFontManager()->getFont(retinaAdd));
        }
	}

	void ChatListHandler::removePlayer( const std::string& playerName )
	{
		for(size_t i = 0; i < m_groups.size(); ++i)
		{
			m_groups[i].first->getList()->removeItem(playerName);
		}
			m_list->repositionGroups();
	}

	void ChatListHandler::addPlayer(
		const std::string& playerName, GroupTypeEnum group )
	{
		for(size_t i = 0; i < m_groups.size(); ++i)
		{
			if(m_groups[i].second == group)
			{
				m_groups[i].first->getList()->addItem(playerName);
				break;
			}
		}

		m_list->repositionGroups();
	}

	void ChatListHandler::mouseDoubleClickCB( agui::MouseEvent &mouseEvent )
	{
        if(!Platf::isMobile() && mouseEvent.getButton() == agui::MOUSE_BUTTON_LEFT &&
			m_list->getSelectedIndex() != -1)
		{
			std::string selItem = m_list->getSelectedList()->getItemAt
				(m_list->getSelectedIndex());

			//request showing the profile
			DISPATCH_LOBBY_EVENT
			{
				(*it)->requestShowProfile(selItem);
			}
		}
	}

	void ChatListHandler::mouseDownCB( agui::MouseEvent &mouseEvent )
	{
        if(!Platf::isMobile() && mouseEvent.getButton() == agui::MOUSE_BUTTON_RIGHT)
		{
            mouseAction(mouseEvent);
		}
	}
    
    void ChatListHandler::mouseClickCB( agui::MouseEvent &mouseEvent )
    {
        if(Platf::isMobile() && mouseEvent.getButton() == agui::MOUSE_BUTTON_LEFT)
        {
            mouseAction(mouseEvent);
        }
    }
    
    void ChatListHandler::mouseAction( agui::MouseEvent &mouseEvent )
    {
        if(m_list->getSelectedIndex() != -1)
        {
            for(size_t i = 0; i < m_groups.size(); ++i)
            {
                if(m_groups[i].first->getList() == mouseEvent.getSourceWidget())
                {
                    PlayerMenuContext context;
                    switch(m_groups[i].second)
                    {
                        case FRIENDS_GROUP:
                            context = LIST_FRIEND_RCLICK;
                            break;
                        case ENEMIES_GROUP:
                            context = LIST_ENEMY_RCLICK;
                            break;
                        case IN_LOBBY_GROUP:
                            context = LIST_LOBBY_RCLICK;
                            break;
                        case IN_GAME_GROUP:
                            context = LIST_IN_GAME_RCLICK;
                            break;
                    }
                    
                    DISPATCH_LOBBY_EVENT
                    {
                        (*it)->showPlayerContext(
                                                 m_groups[i].first->getList()->getItemAt(
                                                                                         m_groups[i].first->getList()->getSelectedIndex()), 
                                                 mouseEvent.getX(),mouseEvent.getY(),
                                                 context, mouseEvent.getSourceWidget());
                    }
                }
            }
        }
    }

	void ChatListHandler::movePlayer( const std::string& playerName, GroupTypeEnum newGroup )
	{
		removePlayer(playerName);
		addPlayer(playerName,newGroup);
	}

	void ChatListHandler::seatOccupancyChanged( const std::string& userName, bool occupied, bool isSelf, bool isSelfTable, int tableNo, int seatNo, int numPlayersAtTable )
	{
	}

	bool ChatListHandler::isInGroup( const std::string& name, GroupTypeEnum group )
	{
		for(size_t i = 0; i < m_groups.size(); ++i)
		{
			if(m_groups[i].second == group)
			{
				if(m_groups[i].first->getList())
				{
					agui::ListItem::iterator it = m_groups[i].first->getList()->getItemsBegin();
					agui::ListItem::iterator end = m_groups[i].first->getList()->getItemsEnd();

					for(it; it !=
						end; ++it)
					{
						if((*it).first.text == name)
						{
							return true;
						}
					}
				}
			
				break;
			}
		}

		return false;

		m_list->repositionGroups();
	}

	void ChatListHandler::actionPerformed( const agui::ActionEvent &evt )
	{
		for(size_t i = 0; i < m_groups.size(); ++i)
		{
			if(m_groups[i].first == evt.getSource())
			{
				DISPATCH_LOBBY_EVENT
					(*it)->playerGroupToggleChanged(
					m_groups[i].second,m_groups[i].first->isToggled());
				break;
			}
		}

	}

	void ChatListHandler::loadSettings( ClientShared* shared )
	{
		loadToggleStates();
	}

	void ChatListHandler::loadToggleStates()
	{
		for(size_t i = 0; i < m_groups.size(); ++i)
		{
			m_groups[i].first->setToggleState(isPlayerGroupToggled(m_groups[i].second));
		}

		m_list->repositionGroups();
	}

}
