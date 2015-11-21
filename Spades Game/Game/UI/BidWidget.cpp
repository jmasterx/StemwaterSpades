#include "Game/UI/BidWidget.hpp"
#include "Game/Utility/Vec2.hpp"
#include "Game/Utility/Math.hpp"
#include <sstream>
namespace cge
{
	BidWidget::~BidWidget(void)
	{
	}



	void BidWidget::resize( int w, int h )
	{
		float tw = (float)getProportions()->getTableSize() * getProportions()->getScale();
		int startX = (float)getProportions()->getScreenWidth() * 
			getProportions()->getTableOffset().getX();
		startX -= tw * 0.3f;

		int startY = (float)getProportions()->getScreenHeight() * 
			getProportions()->getTableOffset().getY();

		startY += (float)getProportions()->getScreenHeight() * 
			0.034f;
        //if(Platf::isRetina())
          //  startY -= 50;
        
        //if(Platf::isMini())
          //  startY -= 25;

		int width = tw * 0.6f;
		int height = tw * 0.5f;

		setSize(width,height);
		resizeButtons();

		int csz = getProportions()->getScale() *(m_numButtonsShowing <= 13 ? 320 : 335);
		setLocation(startX,startY - csz);

	}

	BidWidget::BidWidget( 
		const std::vector<Button*>& buttons, Button* nilButton, 
		Button* bidButton, GuiFontManager* fontManager, const std::string& bidText,
		const std::string& nilText )
		: m_bid(-1), m_nilShowing(false),m_numButtonsShowing(1)
	{
		setFocusable(false);
		setVisibility(false);
		m_buttons = buttons;
		m_nilButton = nilButton;
		m_bidButton = bidButton;
		m_fontManager = fontManager;

		m_bidButton->setFocusable(false);
		m_bidButton->setEnabled(false);
		m_bidButton->setText(bidText);
		add(m_bidButton);
		m_bidButton->addActionListener(this);
		
		m_nilButton->setFocusable(false);
		m_nilButton->setToggleButton(true);
		m_nilButton->setAutoUntoggle(false);
		m_nilButton->setText(nilText);
		m_nilButton->addActionListener(this);
		add(m_nilButton);

		for (size_t i = 0; i < m_buttons.size(); ++i)
		{
			m_buttons[i]->setToggleButton(true);
			m_buttons[i]->setAutoUntoggle(false);
			std::stringstream ss;
			ss << (i+1);
			std::string val;
			ss >> val;
			m_buttons[i]->setFocusable(false);
			m_buttons[i]->setText(val);
			add(m_buttons[i]);
			m_buttons[i]->addActionListener(this);
		}
	}

	void BidWidget::actionPerformed( const agui::ActionEvent &evt )
	{
		if(!m_bidButton->isEnabled())
		m_bidButton->setEnabled(true);

	

		if(evt.getSource() == m_bidButton)
		{
			DISPATCH_SCENE_EVENT
				(*it)->playSound("bid.click2");
			finishedBidding();
			return;
		}
		else
		{
			DISPATCH_SCENE_EVENT
				(*it)->playSound("bid.click");
		}

		for(size_t i = 0; i < m_buttons.size(); ++i)
		{
			if(evt.getSource() != m_buttons[i])
			{
				m_buttons[i]->setToggleState(false);
			}
			else
			{
				m_bid = i + 1;
			}
		}

		if(evt.getSource() != m_nilButton)
		{
			m_nilButton->setToggleState(false);
		}
		else
		{
			m_bid = 0;
		}

	}

	void BidWidget::finishedBidding()
	{
		setVisibility(false);
		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->trickCountChanged(m_bid);
		}
	}

	void BidWidget::paintBackground( const agui::PaintEvent &paintEvent )
	{

	}

	void BidWidget::paintComponent( const agui::PaintEvent &paintEvent )
	{

	}

	void BidWidget::startBidding( int numButtons,bool niled, bool moonshot )
	{
		m_bidButton->setEnabled(false);

		m_bid = -1;

		m_nilShowing = niled;
		m_numButtonsShowing = numButtons;
		for(size_t i = 0; i < m_buttons.size(); ++i)
		{
			m_buttons[i]->setVisibility(false);
		}
		for(int i = 0; i < m_numButtonsShowing; ++i)
		{
			m_buttons[i]->setVisibility(true);
			m_buttons[i]->setToggleState(false);
		}

		m_nilButton->setVisibility(m_nilShowing);
		m_nilButton->setToggleState(false);
		resize(getProportions()->getScreenWidth(),
			getProportions()->getScreenHeight());
		setVisibility(true);

		if(!moonshot && !m_buttons.empty())
		{
			m_buttons[m_buttons.size() - 1]->setEnabled(false);
		}
	}

	void BidWidget::resizeButtons()
	{
		float scale = 0.85f;
       // if(Platf::isRetina() || Platf::isMini())
            scale = 1.08f;
		float yOffset =  m_numButtonsShowing > 13 ? 10.0f : 45.0f;
        
		float y = yOffset;
        

		y += 20;
		layoutRow(y * scale * getProportions()->getScale(),0,7,128 * 
			scale * getProportions()->getScale());

		y += 75;
		layoutRow(y * scale * getProportions()->getScale(),7,6,126 * 
			scale * getProportions()->getScale());
	
		y += 125;

		if(m_numButtonsShowing > 13)
		{
			y = yOffset + 171;

			layoutRow(y * scale * 
				getProportions()->getScale(),13,4,125 * scale *
				getProportions()->getScale(), 1.0f, 0.65f);

			y += 125;
		}

		y *= getProportions()->getScale() * scale;
	
			float nilSz = 185 * scale * 
				getProportions()->getScale();
			y -= nilSz * 0.28f;
			float nilFontSz = nilSz * 0.18f;
			if(m_bidButton->getTextLength() > 3)
			{
				nilFontSz *= 0.85f;
			}
			m_nilButton->setSize(nilSz,nilSz);
			m_nilButton->setFont(m_fontManager->getFont((int)nilFontSz));

			int padding = nilSz * 0.35f;
			int totalSz = (2 * nilSz) - padding;
			int centerOffset = (getWidth() - totalSz) / 2;
			m_bidButton->setSize(nilSz,nilSz);
		
			m_bidButton->setFont(m_fontManager->getFont((int)nilFontSz));

			m_nilButton->setLocation(centerOffset,y);
			m_bidButton->setLocation(centerOffset + nilSz - padding,y);
	
	}

	void BidWidget::layoutRow( int yStart, int indexStart, int numOnRow, int width, float spread, float angStrength )
	{
		int fontSize = width * 0.25f;
		agui::Font* f = m_fontManager->getFont(fontSize);
		int padding = width * 0.4f * spread;

		int centerOffset = ((float)getWidth() - 
			(numOnRow * (width - padding)) + padding ) / 2.0f;

		int count = 0;
		for(int i = indexStart; i < indexStart + numOnRow && 
			i < (int)m_buttons.size() && i < m_numButtonsShowing; ++i)
		{
			Vec2 a = Vec2(centerOffset,(float)yStart - (width * 0.22f * angStrength * ((float)numOnRow / 7.0f) ));
			Vec2 b = Vec2(getWidth() / 2,(float)yStart);
			Vec2 c = Vec2(getWidth() - centerOffset, (float)yStart - (width * 0.24f * angStrength * ((float)numOnRow / 7.0f) ));

			Vec2 pos = Math::quadraticBezier(a,b,c, (float)count /(float)(numOnRow - 1));


			m_buttons[i]->setLocation(pos.getX() - (width / 2),pos.getY());
			m_buttons[i]->setSize(width,width);
			m_buttons[i]->setFont(f);
			count++;
		}
	}

	void BidWidget::bidBegin( int numButtons, bool nilActive )
	{
		startBidding(numButtons,nilActive,true);
	}

	void BidWidget::addListener( ClientEventListener* listener )
	{
		m_listeners.push_back(listener);
	}

	void BidWidget::removeListener( ClientEventListener* listener )
	{
		m_listeners.erase(
			std::remove(m_listeners.begin(),
			m_listeners.end(), listener),
			m_listeners.end());
	}

	int BidWidget::getContentsHeight() const
	{
		int lowestVal = 0;
		int highestVal = 0;
		for(agui::WidgetArray::const_iterator it = getChildBegin(); it != getChildEnd(); ++it)
		{
			if((*it)->isVisible())
			{
				if((*it)->getLocation().getY() < lowestVal)
				{
					lowestVal = (*it)->getLocation().getY();
				}

				if((*it)->getLocation().getY() + (*it)->getHeight() > highestVal)
				{
					highestVal = (*it)->getLocation().getY() + (*it)->getHeight();
				}
			}
		}

		return highestVal - lowestVal;
	}

}

