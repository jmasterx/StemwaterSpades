#ifndef CGE_LOBBY_CHANGE_DIALOG_HPP
#define CGE_LOBBY_CHANGE_DIALOG_HPP
#include "Game/UI/GeneralDialogBox.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
#include <vector>
namespace cge
{
	struct LobbyChangeCell
	{
		Label* lblLobbyName;
		Label* lblFriendCount;
		Label* lblNumUsers;
		Button* btnGo;
		agui::Widget* container;
		LobbyChangeCell()
			: lblLobbyName(NULL),
			lblFriendCount(NULL),
			lblNumUsers(NULL),
			btnGo(NULL),
			container(NULL) {
		}
	};

	struct LobbyChangeCellData
	{
		std::string lobbyName;
		std::string friendCount;
		std::string ip;
		int port;
		std::string numUsers;
		bool full;

		LobbyChangeCellData() : port(0), full(false) {
		}
	};

	class LobbyChangeDialog : public GeneralDialogBox, public LobbyEventProvider
	{
		FlowLayout* m_container;
		std::vector<LobbyChangeCell> m_cells;
		std::vector<LobbyChangeCellData> m_cellData;
		std::string m_address;
		int m_port;
		bool m_acceptChanges;
		Label* createLobbyChangeLabel(const std::string& text, agui::Widget* parent, float fontScalar = 1.0f);
		void clearCells();
		void addCell(const std::string& lobbyName,
			const std::string& friendCount, 
			const std::string& numUsers, bool full, bool currentLobby);
		void repositionCells();
		virtual void dialogWillAppear();
		void setAcceptChanges(bool accept);
		bool isAcceptingChanges() const;
	public:
		LobbyChangeDialog(GuiFactory* factory);
		void setData(const std::vector<std::string>& names, 
			const std::vector<std::string>& ips,
			const std::vector<int>& ports, 
			const std::vector<int>& capacities, 
			const std::vector<int>& numUsers, const std::vector<int>& numFriends
			);
		virtual bool wantCloseButton() const;
		virtual void frameActionReceived(GeneralFrame* frame,const std::string& action);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void onParentSizeChanged(agui::Widget* source, const agui::Dimension &parentInnerSize);
		virtual void receiveServerFull(const std::string& serverName, const std::string& ip, int port, bool full);
		void setCurrentAddressAndPort(const std::string& address, int port);
		virtual ~LobbyChangeDialog(void);
	};
}
#endif
