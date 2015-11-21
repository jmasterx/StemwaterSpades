#include "Game/UI/ScoreWidget.hpp"
#include <sstream>
namespace cge
{
	ScoreWidget::~ScoreWidget(void)
	{
	}

	void ScoreWidget::setScores( const std::vector<int>& scores, const std::vector<int>& bags )
	{
		m_scores.clear();
		m_bags.clear();
		for(size_t i = 0; i < scores.size(); ++i)
		{
			std::stringstream ss;
			ss << scores[i];
			m_scores.push_back(ss.str());
		}

		for(size_t i = 0; i < bags.size(); ++i)
		{
			std::stringstream ss;
			ss << bags[i];
			m_bags.push_back(ss.str());
		}

	}

	void ScoreWidget::setScoreTextFont( agui::Font* font )
	{
		m_textFont = font;
	}

	void ScoreWidget::setScoreTextColor( const agui::Color& color )
	{
		m_textColor = color;
	}

	void ScoreWidget::setScoreText( const std::string& text )
	{
		m_scoreText = text;
	}

	void ScoreWidget::setBagsText( const std::string& text )
	{
		m_bagText = text;
	}

	void ScoreWidget::paintBackground( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawNinePatchImage(m_bg,agui::Point(0,0),getSize());
	}


	ScoreWidget::ScoreWidget( agui::Image* partnerImg1, agui::Image* partnerImg2, agui::Image* indivImg1, agui::Image* indivImg2, agui::Image* indivImg3, agui::Image* indivImg4, agui::Image* textShadow, agui::Image* bg, GuiFontManager* fontManager )
		: m_bg(bg), m_vgap(6), m_hgap(4), m_textFont(NULL) , m_textshadow(textShadow),
		m_fontManager(fontManager), m_scale(0.0f)
	{
		m_partnershipImgs.push_back(partnerImg1);
		m_partnershipImgs.push_back(partnerImg2);

		m_individualImgs.push_back(indivImg1);
		m_individualImgs.push_back(indivImg2);
		m_individualImgs.push_back(indivImg3);
		m_individualImgs.push_back(indivImg4);
	}


	void ScoreWidget::paintComponent( const agui::PaintEvent &paintEvent )
	{
		std::vector<agui::Image*>* imgs = 
			m_scores.size() == 2 ? &m_partnershipImgs : &m_individualImgs;

		int h = 0;
		paintEvent.graphics()->drawText(
			agui::Point(m_hgap,0),m_scoreText.c_str(),
			getFontColor(),getFont());

		paintEvent.graphics()->drawText(
			agui::Point(alignString(m_bagText,
			agui::ALIGN_RIGHT) - m_hgap,0),m_bagText.c_str(),
			getFontColor(),getFont());

		h += m_vgap;
		h += getFont()->getLineHeight();

		for (size_t i = 0; i < m_scores.size(); ++i)
		{
			int lh = m_textFont->getLineHeight();

			paintEvent.graphics()->drawNinePatchImage((*imgs)[i],agui::Point(0,h),
				agui::Dimension(getInnerWidth(),lh));

			int scoreTw = m_textFont->getTextWidth(m_scores[i]);
			int bagsTw = m_textFont->getTextWidth(m_bags[i]);

			paintEvent.graphics()->drawNinePatchImage(m_textshadow,
				agui::Point(m_hgap - scoreTw,h -  (lh / 3)),
				agui::Dimension(scoreTw * 3,lh * 1.5f),0.38f);

			paintEvent.graphics()->drawNinePatchImage(m_textshadow,
				agui::Point(getInnerWidth() - (m_hgap + bagsTw + bagsTw),h -  (lh / 3)),
				agui::Dimension(bagsTw * 3,lh * 1.5f),0.38f);

			
			paintEvent.graphics()->drawText(
				agui::Point(m_hgap,h - 1),m_scores[i].c_str(),
				m_textColor,m_textFont);
			

			paintEvent.graphics()->drawText(
				agui::Point(alignString(m_bags[i],
				agui::ALIGN_RIGHT) - m_hgap,h - 1),m_bags[i].c_str(),
				m_textColor,m_textFont);

			h += m_textFont->getLineHeight();
			h += 4 * m_scale;
		}

	}

	int ScoreWidget::alignString( const std::string& text, agui::AlignmentEnum align )
	{
		int w = getFont()->getTextWidth(text);
		int h = getFont()->getLineHeight();
		agui::AreaAlignmentEnum a;
		if(align == agui::ALIGN_LEFT)
			a = agui::ALIGN_TOP_LEFT;
		else if(align == agui::ALIGN_CENTER)
			a = agui::ALIGN_TOP_CENTER;
		else
			a = agui::ALIGN_TOP_RIGHT;

		return createAlignedPosition(a,getInnerRectangle(),
			agui::Dimension(w,h)).getX();
	}

	void ScoreWidget::setSize( const agui::Dimension &size )
	{
		agui::Widget::setSize(size);
		m_scale = getWidth() / 135.0f;

		setFont(m_fontManager->getFont(0.97f * m_scale));
		setScoreTextFont(m_fontManager->getFont(0.93f * m_scale));
	}

}

