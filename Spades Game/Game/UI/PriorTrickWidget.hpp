#ifndef CGE_PRIOR_TRICK_WIDGET_HPP
#define CGE_PRIOR_TRICK_WIDGET_HPP
#include "Agui/Widget.hpp"
#include "Game/Card/Card.hpp"
#include "Game/UI/GuiFontManager.hpp"
namespace cge
{
	class PriorTrickWidget : public agui::Widget
	{
		std::vector<Card> m_cards;
		agui::Image* m_heartImg;
		agui::Image* m_diamondImg;
		agui::Image* m_clubImg;
		agui::Image* m_spadeImg;
		agui::Image* m_bg;
		int m_hgap;
		int m_vgap;
		std::string m_priorTrickText;
		agui::Color m_black;
		agui::Color m_red;
		agui::Font* m_rankFont;
		GuiFontManager* m_fontManager;
		float m_scale;
		float m_imgScale;
		float m_shadowOffScale;
	protected:
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
	public:
		PriorTrickWidget(
			agui::Image* heartImg,
			agui::Image* diamondImg,
			agui::Image* clubImg,
			agui::Image* spadeImg,
			agui::Image* bg,
			GuiFontManager* fontManager);
		virtual ~PriorTrickWidget(void);
		void setCards(const std::vector<Card>& cards);
		void setPriorTrickText(std::string priorText);
		void setBlack(const agui::Color& color);
		void setRed(const agui::Color& color);
		void setCardRankFont(agui::Font* font);
		agui::Image* getSuitImage(Card::SuitEnum suit) const;
		void renderCard(agui::AreaAlignmentEnum align, const Card& card,
			int hgap, int vgap,
			const agui::PaintEvent& paintEvent);
		int alignString(const std::string& text, agui::AlignmentEnum align);
		agui::Dimension getSuitImgSize(Card::SuitEnum suit) const;
		virtual void setSize(const agui::Dimension &size);
	};
}

#endif
