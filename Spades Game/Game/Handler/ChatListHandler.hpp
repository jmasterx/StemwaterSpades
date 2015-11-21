#ifndef CGE_CHAT_LIST_HANDLER_HPP
#define CGE_CHAT_LIST_HANDLER_HPP
#include "Game/UI/GuiFactory.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
#include "Game/Handler/SceneEventHandler.hpp"
#include <vector>
namespace cge
{
	class ChatListHandler : public LobbyEventProvider,
		public SceneEventProvider,
		public agui::MouseListener, public agui::ActionListener
	{
		GuiFactory* m_factory;
		CollapsibleList* m_list;
		ToolContainer* m_container;
		BorderLayout* m_layout;
		std::vector<std::pair<CollapsibleButton*, GroupTypeEnum> > m_groups;
	protected:
		virtual void mouseDoubleClickCB(agui::MouseEvent &mouseEvent);
        virtual void mouseClickCB(agui::MouseEvent &mouseEvent);
		virtual void mouseDownCB(agui::MouseEvent &mouseEvent);
        virtual void mouseAction(agui::MouseEvent &mouseEvent);
	public:
		ChatListHandler(GuiFactory* factory);
		CollapsibleList* getGroupList();
		agui::Widget* getContainer();
		virtual void addGroup(const std::string& groupName, 
			const agui::Color& groupColor, GroupTypeEnum type);
		virtual void addPlayer(const std::string& playerName, 
			GroupTypeEnum type);
		virtual void removePlayer(const std::string& playerName);
		bool isInGroup(const std::string& name, GroupTypeEnum group);
		void movePlayer(const std::string& playerName, GroupTypeEnum newGroup);
		virtual void seatOccupancyChanged(const std::string& userName,
			bool occupied, bool isSelf, bool isSelfTable, 
			int tableNo, int seatNo, int numPlayersAtTable);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void loadSettings(ClientShared* shared);
		void loadToggleStates();
		virtual ~ChatListHandler(void);
	};
}

#endif
