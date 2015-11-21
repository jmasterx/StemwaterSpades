#ifndef CGE_FRIEND_LOBBY_CHANGE_DIALOG_HPP
#define CGE_FRIEND_LOBBY_CHANGE_DIALOG_HPP
#include "Game/UI/GeneralDialogBox.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
#include <vector>
namespace cge
{
	struct FriendLobbyChangeCell
	{
		Label* lblFriendName;
		Label* lblServerName;
		Label* lblNumUsers;
		Button* btnGo;
		agui::Widget* container;
		FriendLobbyChangeCell()
			: lblFriendName(NULL),
			lblServerName(NULL),
			lblNumUsers(NULL),
			btnGo(NULL),
			container(NULL) {
		}
	};

	struct FriendLobbyChangeCellData
	{
		std::string friendName;
		std::string serverName;
		std::string ip;
		int port;
		std::string numUsers;
		bool full;

		FriendLobbyChangeCellData() : port(0), full(false) {
		}
	};

	class FriendLobbyChangeDialog : public GeneralDialogBox, public LobbyEventProvider
	{
		FlowLayout* m_container;
		std::vector<FriendLobbyChangeCell> m_cells;
		std::vector<FriendLobbyChangeCellData> m_cellData;
		std::string m_address;
		int m_port;
		bool m_acceptChanges;
		Label* createFriendLobbyChangeLabel(const std::string& text, agui::Widget* parent, float fontScalar = 1.0f);
		void clearCells();
		void addCell(const std::string& lobbyName,
			const std::string& friendCount, 
			const std::string& numUsers, bool full, bool currentLobby);
		void repositionCells();
		virtual void dialogWillAppear();
		void setAcceptChanges(bool accept);
		bool isAcceptingChanges() const;
	public:
		FriendLobbyChangeDialog(GuiFactory* factory);
		void setData(const std::vector<std::string>& names, 
			const std::vector<std::string>& ips,
			const std::vector<int>& ports, 
			const std::vector<int>& capacities, 
			const std::vector<int>& numUsers, const std::vector<std::string>& serverNames
			);
		virtual bool wantCloseButton() const;
		virtual void frameActionReceived(GeneralFrame* frame,const std::string& action);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void onParentSizeChanged(agui::Widget* source, const agui::Dimension &parentInnerSize);
		virtual void receiveServerFull(const std::string& serverName, const std::string& ip, int port, bool full);
		void setCurrentAddressAndPort(const std::string& address, int port);
		virtual ~FriendLobbyChangeDialog(void);
	};
}
#endif
