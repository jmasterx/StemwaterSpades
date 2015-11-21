#include "Game/UI/PassedCardsWidget.hpp"

namespace cge
{
	PassedCardsWidget::PassedCardsWidget( agui::Image* heartImg, agui::Image* diamondImg, agui::Image* clubImg, agui::Image* spadeImg, agui::Image* bg, GuiFontManager* fontManager )
	: m_heartImg(heartImg), m_diamondImg(diamondImg),
	m_clubImg(clubImg), m_spadeImg(spadeImg),
	m_bg(bg),
	m_hgap(12), m_vgap(6),
	m_fontManager(fontManager), m_rankFont(NULL),
	m_scale(1.0f), m_shadowImgOffs(2.0f)
	{
	}

	PassedCardsWidget::~PassedCardsWidget(void)
	{
	}

	void PassedCardsWidget::setCards( const std::vector<Card>& passed,
		const std::vector<Card>& received )
	{
		m_passedCards = passed;
		m_receivedCards = received;
	}

	void PassedCardsWidget::paintBackground( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawNinePatchImage(m_bg,agui::Point(0,0),getSize());
	}

	void PassedCardsWidget::paintComponent( const agui::PaintEvent &paintEvent )
	{
		std::vector<Card>* cards[] = {&m_passedCards,&m_receivedCards};
		std::string* strings[] = {&m_passedText,&m_receivedText};
		
		float suitScale = 0.27f;
		int h = 0;
		for(int x = 0; x < 2; ++x)
		{
			paintEvent.graphics()->drawText(
				agui::Point(alignString(*strings[x],
				agui::ALIGN_CENTER),h),strings[x]->c_str(),
				getFontColor(),getFont());

			h += getFont()->getLineHeight();

			h += m_vgap * m_scale;

			int w = 0;
			int elemsWidth = 0;

			w += m_hgap * m_scale * 2.35f;
			for(size_t i = 0; i < cards[x]->size(); ++i)
			{
				agui::Image* suitImg = getSuitImage((*cards[x])[i].getSuit());
				agui::Dimension suitDim = getSuitImgSize((*cards[x])[i].getSuit());
				int offset = ((suitDim.getHeight() * m_scale * suitScale) - m_rankFont->getLineHeight()) / 2;

				if (i == 1)
				{
					w = getInnerWidth() - ( 132 * m_scale * suitScale);
				}

				int incr = 0;
				incr += m_rankFont->getTextWidth((*cards[x])[i].getShortRankString());
				incr += 1.0 * m_scale; // between text and image
				
				
				paintEvent.graphics()->drawText(agui::Point(w - incr,h + offset),
					(*cards[x])[i].getShortRankString().c_str(),
					(*cards[x])[i].isRed() ? m_red : m_black,m_rankFont);

				paintEvent.graphics()->drawScaledImage(suitImg,agui::Point(w,h - (4.5f * m_scale * suitScale)),agui::Point(),
					agui::Dimension(suitImg->getWidth(), suitImg->getHeight()),
					agui::Dimension(suitImg->getWidth() * m_scale * suitScale, suitImg->getHeight() * m_scale * suitScale));
				
				w += suitDim.getWidth() * m_scale * suitScale;

				w += m_hgap * m_scale;
			}

			//move back to beginning of line
			w = 0;
			agui::Dimension heartDim = getSuitImgSize(Card::HEARTS);
			h += (heartDim.getHeight() + m_vgap * 5) * m_scale * suitScale;
		}
		
	}

	void PassedCardsWidget::setPassedText( std::string passedText )
	{
		m_passedText = passedText;

	}

	void PassedCardsWidget::setReceivedText( std::string receivedText )
	{
		m_receivedText = receivedText;
	}

	int PassedCardsWidget::alignString( 
		const std::string& text, agui::AlignmentEnum align )
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

	void PassedCardsWidget::setBlack( const agui::Color& color )
	{
		m_black = color;
	}

	void PassedCardsWidget::setRed( const agui::Color& color )
	{
		m_red = color;
	}

	agui::Image* PassedCardsWidget::getSuitImage( Card::SuitEnum suit ) const
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


	void PassedCardsWidget::setCardRankFont( agui::Font* font )
	{
		m_rankFont = font;
	}

	void PassedCardsWidget::setSize( const agui::Dimension &size )
	{
		agui::Widget::setSize(size);
		m_scale = size.getWidth() / 129.0f;
		float textScale = size.getWidth() / 135.0f;
		setFont(m_fontManager->getFont(0.97f * textScale));
		setCardRankFont(m_fontManager->getFont(1.1f * textScale));
	}

	agui::Dimension PassedCardsWidget::getSuitImgSize(Card::SuitEnum suit) const
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
			return agui::Dimension(m_clubImg->getWidth() * 0.7f,m_clubImg->getHeight() * 0.53f);
			break;
		case Card::SPADES:
			return agui::Dimension(m_spadeImg->getWidth() * 0.7f,m_spadeImg->getHeight() * 0.53f);
			break;
		}

		return agui::Dimension(m_diamondImg->getWidth(),m_diamondImg->getHeight());
	}

}

