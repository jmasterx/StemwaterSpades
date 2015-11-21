#ifndef CGE_NOTIFICATION_LIST_BOX_HPP
#define CGE_NOTIFICATION_LIST_BOX_HPP
#include "Agui/Widgets/ListBox/ListBox.hpp"
#include "Game/UI/NotificationItem.hpp"
#include <vector>
namespace cge
{
	class NotificationListBox : public agui::ListBox
	{
		agui::Image* m_bg;
		agui::HScrollBar* m_hScroll;
		agui::VScrollBar* m_vScroll;
		agui::Widget* m_inset;
		std::vector<NotificationItem*> m_notifications;
		agui::Font* m_secondFont;
		agui::Font* m_thirdFont;
		agui::Color m_secondFontColor;
		agui::Color m_thirdFontColor;
        std::string m_noNotifications;
	public:
		NotificationListBox(agui::Image* bg,agui::HScrollBar* hscroll,
			agui::VScrollBar* vscroll, agui::Widget* inset);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		NotificationItem* addNotification(const NotificationItem& item);
		void removeNotification(NotificationItem* item);

		void setSecondFont(const agui::Font* font);
		void setThirdFont(const agui::Font* font);
		void setSecondFontColor(const agui::Color& color);
		void setThirdFontColor(const agui::Color& color);
		const agui::Color& getSecondFontColor() const;
		const agui::Color& getThirdFontColor() const;
		const agui::Font* getSecondFont() const;
		const agui::Font* getThirdFont() const;
		virtual void setFont(const agui::Font *font);
		NotificationItem* getSelectedNotification();

		void recalcItemHeight();
		virtual void setWidestItem();
		virtual void resizeWidthToContents();
		virtual void resizeHeightToContents();
		void setNoNotificationsText(const std::string& text);


		virtual ~NotificationListBox(void);
	};
}

#endif