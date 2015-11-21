#ifndef CGE_PASSED_CARDS_WIDGET_HPP
#define CGE_PASSED_CARDS_WIDGET_HPP
#include "Agui/Widget.hpp"
#include "Game/Card/Card.hpp"
#include "Game/UI/GuiFontManager.hpp"
namespace cge
{
	class PassedCardsWidget : public agui::Widget
	{
		std::vector<Card> m_passedCards;
		std::vector<Card> m_receivedCards;
		agui::Image* m_heartImg;
		agui::Image* m_diamondImg;
		agui::Image* m_clubImg;
		agui::Image* m_spadeImg;
		agui::Image* m_bg;
		int m_hgap;
		int m_vgap;
		std::string m_passedText;
		std::string m_receivedText;
		agui::Color m_black;
		agui::Color m_red;
		agui::Font* m_rankFont;
		GuiFontManager* m_fontManager;
		float m_scale;
		float m_shadowImgOffs;

	protected:
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
	public:
		PassedCardsWidget(
		agui::Image* heartImg,
		agui::Image* diamondImg,
		agui::Image* clubImg,
		agui::Image* spadeImg,
		agui::Image* bg,
		GuiFontManager* fontManager);
		void setCards(const std::vector<Card>& passed,const std::vector<Card>& received);
		void setPassedText(std::string passedText);
		void setReceivedText(std::string receivedText);
		void setBlack(const agui::Color& color);
		void setRed(const agui::Color& color);
		void setCardRankFont(agui::Font* font);
		agui::Dimension getSuitImgSize(Card::SuitEnum suit) const;
		agui::Image* getSuitImage(Card::SuitEnum suit) const;
		int alignString(const std::string& text, agui::AlignmentEnum align);
		virtual void setSize(const agui::Dimension &size);
		virtual ~PassedCardsWidget(void);
	};
}

#endif
