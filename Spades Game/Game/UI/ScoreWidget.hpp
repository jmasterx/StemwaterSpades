#ifndef CGE_SCORE_WIDGET_HPP
#define CGE_SCORE_WIDGET_HPP
#include "Agui/Widget.hpp"
#include "Game/UI/GuiFontManager.hpp"
namespace cge
{
	class ScoreWidget : public agui::Widget
	{
		std::string m_scoreText;
		std::string m_bagText;

		std::vector<agui::Image*> m_partnershipImgs;
		std::vector<agui::Image*> m_individualImgs;
		agui::Image* m_textshadow;
		agui::Image* m_bg;
		std::vector<std::string> m_scores;
		std::vector<std::string> m_bags;
		int m_vgap;
		int m_hgap;
		agui::Font* m_textFont;
		agui::Color m_textColor;
		GuiFontManager* m_fontManager;
		float m_scale;
	protected:
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
	public:
		ScoreWidget(agui::Image* partnerImg1,
					agui::Image* partnerImg2,
					agui::Image* indivImg1,
					agui::Image* indivImg2,
					agui::Image* indivImg3,
					agui::Image* indivImg4,
					agui::Image* textShadow,
					agui::Image* bg,
					GuiFontManager* fontManager);
		void setScoreTextColor(const agui::Color& color);
		void setScoreTextFont(agui::Font* font);
		void setScoreText(const std::string& text);
		void setBagsText(const std::string& text);
		void setScores(const std::vector<int>& scores, const std::vector<int>& bags);
		int alignString(const std::string& text, agui::AlignmentEnum align);
		virtual void setSize(const agui::Dimension &size);
		virtual ~ScoreWidget(void);
	};	
}

#endif
