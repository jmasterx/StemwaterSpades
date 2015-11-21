#include "Game/UI/PriorTrickWidget.hpp"

namespace cge
{
	PriorTrickWidget::PriorTrickWidget(
		agui::Image* heartImg, agui::Image* diamondImg, agui::Image* clubImg, agui::Image* spadeImg, agui::Image* bg, GuiFontManager* fontManager )
		: m_heartImg(heartImg), m_diamondImg(diamondImg),
		m_clubImg(clubImg), m_spadeImg(spadeImg),
		m_bg(bg),
		m_hgap(12), m_vgap(10), m_rankFont(NULL),
		m_fontManager(fontManager),m_scale(1.0f), m_imgScale(0.27f),
		m_shadowOffScale(5.0f)
	{

	}

	PriorTrickWidget::~PriorTrickWidget(void)
	{
	}

	void PriorTrickWidget::setCards( const std::vector<Card>& cards )
	{
		m_cards = cards;
	}

	void PriorTrickWidget::setPriorTrickText( std::string priorText )
	{
		m_priorTrickText = priorText;
	}

	void PriorTrickWidget::setBlack( const agui::Color& color )
	{
		m_black = color;
	}

	void PriorTrickWidget::setRed( const agui::Color& color )
	{
		m_red = color;
	}

	void PriorTrickWidget::setCardRankFont( agui::Font* font )
	{
		m_rankFont = font;
	}

	agui::Image* PriorTrickWidget::getSuitImage( Card::SuitEnum suit ) const
	{
		switch(suit)
		{
		case Card::DIAMONDS:
			return m_diamondImg;
			break;
		case Card::HEARTS:
			return m_heartImg;
			break;
		case Card::CLUBS:
			return m_clubImg;
			break;
		case Card::SPADES:
			return m_spadeImg;
			break;
		}

		return NULL;
	}

	int PriorTrickWidget::alignString( const std::string& text, agui::AlignmentEnum align )
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

	void PriorTrickWidget::paintBackground( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawNinePatchImage(m_bg,agui::Point(0,0),getSize());
	}

	void PriorTrickWidget::paintComponent( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawText(
			agui::Point(alignString(m_priorTrickText,
			agui::ALIGN_CENTER),0),m_priorTrickText.c_str(),
			getFontColor(),getFont());

		//not interested if too few cards
		if(m_cards.size() < 3)
			return;

		int cutThroatYOffset = 0;
		if(m_cards.size() == 3)
		{
			agui::Dimension heartSz = getSuitImgSize(Card::HEARTS);
			cutThroatYOffset = (heartSz.getHeight() * m_scale * m_imgScale) / 2;
		}

		int newVGap = m_vgap * m_scale;
		int newHGap = m_hgap * m_scale;

		renderCard(agui::ALIGN_BOTTOM_CENTER,m_cards[0],0,-newVGap - cutThroatYOffset,
			paintEvent);
		renderCard(agui::ALIGN_MIDDLE_LEFT,m_cards[1],newHGap,
			(newVGap / 2) - cutThroatYOffset,paintEvent);

		if(m_cards.size() > 3)
		{
			renderCard(agui::ALIGN_TOP_CENTER,m_cards[2],0,
				getFont()->getLineHeight() + (newVGap / 2) - cutThroatYOffset,paintEvent);
		}

		renderCard(agui::ALIGN_MIDDLE_RIGHT,m_cards.back(),-newHGap,
			(newVGap / 2) - cutThroatYOffset,paintEvent);
	}

	void PriorTrickWidget::renderCard(
		agui::AreaAlignmentEnum align, const Card& card, 
		int hgap, int vgap, const agui::PaintEvent& paintEvent )
	{
		int textImageOffset = 1.0f * m_scale;
		agui::Dimension imgSz = getSuitImgSize(card.getSuit());

		int cw = m_rankFont->getTextWidth(card.getShortRankString());
		int ch = m_rankFont->getLineHeight();
		agui::Image* suitImg = getSuitImage(card.getSuit());

		int itemWidth = (imgSz.getWidth() * m_imgScale * m_scale);
		int itemHeight = (imgSz.getHeight() * m_scale * m_imgScale) > ch ? imgSz.getHeight() * m_scale * m_imgScale : ch;

		agui::Point startPoint = createAlignedPosition(
			align,getInnerRectangle(),agui::Dimension(itemWidth,itemHeight));

		if(vgap != 0)
		{
			startPoint.setX(startPoint.getX() + hgap);
		}

		if(hgap > 0)
		{
			startPoint.setX(startPoint.getX() + (12.3f * m_scale) + textImageOffset);
		}

		int textDiff = ((imgSz.getHeight() * m_scale * m_imgScale) - ch) / 2;

		paintEvent.graphics()->drawText(agui::Point(
			startPoint.getX() - cw - textImageOffset, startPoint.getY() + vgap + textDiff),
			card.getShortRankString().c_str(),
			card.isRed() ? m_red : m_black,m_rankFont);

		int imgX = startPoint.getX();
		int imgY = startPoint.getY() + vgap;

		paintEvent.graphics()->drawScaledImage(suitImg,agui::Point(imgX,imgY - (4 * m_imgScale * m_scale)),
			agui::Point(),agui::Dimension(suitImg->getWidth(),suitImg->getHeight()),
			agui::Dimension(suitImg->getWidth() * m_scale * m_imgScale, suitImg->getHeight() * m_scale *m_imgScale));
	}

	void PriorTrickWidget::setSize( const agui::Dimension &size )
	{
		agui::Widget::setSize(size);
		m_scale = size.getWidth() / 135.0f;
		setFont(m_fontManager->getFont(0.97f * m_scale));
		setCardRankFont(m_fontManager->getFont(1.1f * m_scale));
	}

	agui::Dimension PriorTrickWidget::getSuitImgSize(Card::SuitEnum suit) const
	{
		switch(suit)
		{
		case Card::DIAMONDS:
			return agui::Dimension(m_diamondImg->getWidth() * 0.65f,m_diamondImg->getHeight() * 0.54f);
			break;
		case Card::HEARTS:
			return agui::Dimension(m_heartImg->getWidth() * 0.65f,m_heartImg->getHeight() * 0.54f);
			break;
		case Card::CLUBS:
			return agui::Dimension(m_clubImg->getWidth() * 0.7f,m_clubImg->getHeight() * 0.5f);
			break;
		case Card::SPADES:
			return agui::Dimension(m_spadeImg->getWidth() * 0.7f,m_spadeImg->getHeight() * 0.53f);
			break;
		}

		return agui::Dimension(m_diamondImg->getWidth(),m_diamondImg->getHeight());
	}

}
