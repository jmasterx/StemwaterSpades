#ifndef CGE_NOTIFICATION_HANDLER_HPP
#define CGE_NOTIFICATION_HANDLER_HPP
#include "Game/Handler/LobbyEventProvider.hpp"
#include "Game/UI/Button.hpp"
#include "Game/UI/NotificationListBox.hpp"
#include "Game/UI/GuiFactory.hpp"
#include "Agui/SelectionListener.hpp"
#include "Agui/MouseListener.hpp"
namespace cge
{
	class NotificationHandler : public LobbyEventProvider,
		public agui::ActionListener, public agui::SelectionListener,
		public agui::MouseListener
	{
		NotificationListBox* m_list;
		bool m_handlingInvites;
		Button* m_button;
		GuiFactory* m_factory;
		int m_numNewNotifications;
		agui::Gui* m_gui;
		LanguageManager* m_language;
		void showNotifications();
		void hideNotifications();
		std::string toString(int i) const;
	public:
		NotificationHandler(GuiFactory* factory,Button* button, agui::Gui* gui);
		NotificationListBox* getWidget();
		std::string generateTableInfo(const SpadesGameInfo& info);
		void addNotification(const NotificationItem& item);
		void removeNotification(NotificationItem* item);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		void setNumNewNotifications(int num);
		void increaseNumNewNotifications(int q = 1);
		virtual void selectionChanged(agui::Widget *source, const std::string &item, 
			int index, bool selected);
		void setHandlingInvites(bool handling);
		virtual void mouseDownCB(agui::MouseEvent &mouseEvent);
		virtual void notificationDispatched(NotificationItem* item);
		virtual void pushNotification(const NotificationItem& item);
		virtual void popNotification(NotificationItem* item);
		void cleanTable(int tableNo);
		virtual void tableIsEmpty(int tableId);
		virtual void tableIsFull(int tableId);
		virtual void sitPlayerAtTable(int tableNumber, int seatNumber, const std::string& playerName);
		virtual void invitationReceived(int tableNo,const std::string& player);
		virtual ~NotificationHandler(void);
	};
}

#endif
