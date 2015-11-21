#ifndef CGE_NOTIFICATION_ITEM_HPP
#define CGE_NOTIFICATION_ITEM_HPP
#include "Agui/Agui.hpp"
namespace cge
{
	class NotificationItem
	{
	public:
		enum NotificationEnum
		{
			NOTIFY_NOT_SET,
			NOTIFY_INVITE
		};
	private:
		std::string m_firstText;
		std::string m_secondText;
		std::string m_thirdText;
		std::string m_username;
		agui::Color m_usernameColor;

		int m_tableNo;

		NotificationEnum m_notify;
	public:
		const std::string& getFirstText() const;
		const std::string& getSecondText() const;
		void setFirstText(const std::string& text);
		void setSecondText(const std::string& text);
		void setThirdText(const std::string& text);
		const std::string& getThirdText() const;

		void setUsername(const std::string& username);
		const std::string& getUsername() const;
		void setUsernameColor(const agui::Color& color);
		const agui::Color& getUsernameColor() const;
		void setTableNo(int tableNo);
		int getTableNo() const;
		void setType(NotificationEnum notify);
		NotificationEnum getType() const;
		NotificationItem(void);
		~NotificationItem(void);
	};
}

#endif
