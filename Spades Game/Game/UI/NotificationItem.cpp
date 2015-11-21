#include "Game/UI/NotificationItem.hpp"
namespace cge
{
	NotificationItem::NotificationItem(void)
		: m_notify(NOTIFY_NOT_SET),m_tableNo(-1)
	{
		setUsernameColor(agui::Color(0,0,0));
	}

	NotificationItem::~NotificationItem(void)
	{
	}

	const std::string& NotificationItem::getFirstText() const
	{
		return m_firstText;
	}

	const std::string& NotificationItem::getSecondText() const
	{
		return m_secondText;
	}

	void NotificationItem::setFirstText( const std::string& text )
	{
		m_firstText = text;
	}

	void NotificationItem::setSecondText( const std::string& text )
	{
		m_secondText = text;
	}

	void NotificationItem::setUsername( const std::string& username )
	{
		m_username = username;
	}

	const std::string& NotificationItem::getUsername() const
	{
		return m_username;
	}

	void NotificationItem::setUsernameColor( const agui::Color& color )
	{
		m_usernameColor = color;
	}

	const agui::Color& NotificationItem::getUsernameColor() const
	{
		return m_usernameColor;
	}

	void NotificationItem::setTableNo( int tableNo )
	{
		m_tableNo = tableNo;
	}

	int NotificationItem::getTableNo() const
	{
		return m_tableNo;
	}

	void NotificationItem::setThirdText( const std::string& text )
	{
		m_thirdText = text;
	}

	const std::string& NotificationItem::getThirdText() const
	{
		return m_thirdText;
	}

	void NotificationItem::setType( NotificationEnum notify )
	{
		m_notify = notify;
	}

	cge::NotificationItem::NotificationEnum NotificationItem::getType() const
	{
		return m_notify;
	}

}
