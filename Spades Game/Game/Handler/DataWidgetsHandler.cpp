#include "Game/Handler/DataWidgetsHandler.hpp"
namespace cge
{
	DataWidgetsHandler::~DataWidgetsHandler(void)
	{
	}

	DataWidgetsHandler::DataWidgetsHandler(
		GuiFactory* factory, agui::Gui* gui, int numPlayers, int numTeams )
		: m_passingCards(false), m_widgetSize(175), m_numTeams(numTeams),
		m_width(0),m_height(0)
	{
		m_borderLayout = factory->createBorderLayout();
		m_borderLayout->setResizable(false);
		m_frame = factory->createFrame(m_borderLayout);
		m_frame->setResizable(false);
		m_frame->setMovable(false);
		m_frame->setMinSize(agui::Dimension(0,0));

		int frameSz = 3 * m_widgetSize;

		m_frame->setSize(frameSz * 1.17f,(frameSz * 1.43) / 3);
		m_borderLayout->setBorderMargins(0,0,m_widgetSize,m_widgetSize);

		m_scoreWidget = factory->createScoreWidget();
		m_borderLayout->add(m_scoreWidget,agui::BorderLayout::WEST);

		m_priorTrick = factory->createPriorTrickWidget();
		m_borderLayout->add(m_priorTrick,agui::BorderLayout::CENTER);

		
			m_passedCards = factory->createPassedCardsWidget();
			m_borderLayout->add(m_passedCards,agui::BorderLayout::EAST);

		gui->add(m_frame);
	}

	void DataWidgetsHandler::roundScoresChanged(
		const std::vector<int>& scores, const std::vector<int> bags )
	{
		m_scoreWidget->setScores(scores,bags);
	}

	void DataWidgetsHandler::passTwoCardsChanged(
		const std::vector<Card>& received, const std::vector<Card>& given )
	{
		if(m_passedCards->isVisible())
		{
			m_passedCards->setCards(given,received);
		}
	}

	void DataWidgetsHandler::lastHandChanged( const std::vector<Card>& lastCards )
	{
		m_priorTrick->setCards(lastCards);
	}

	void DataWidgetsHandler::gameBegin()
	{

		m_scoreWidget->setScores(
			std::vector<int>(m_numTeams,0),std::vector<int>(m_numTeams,0));

		m_priorTrick->setCards(std::vector<Card>());

		if(m_passedCards->isVisible())
		{
			m_passedCards->setCards(std::vector<Card>(),std::vector<Card>());
		}

	}

	void DataWidgetsHandler::roundBegan()
	{
		m_priorTrick->setCards(std::vector<Card>());

		if(m_passedCards->isVisible())
		{
			m_passedCards->setCards(std::vector<Card>(),std::vector<Card>());
		}
	}

	void DataWidgetsHandler::resize( int w, int h )
	{
		m_width = w;
		m_height = h;
		if(!getProportions())
		{
			return;
		}
		int tabSz = getProportions()->getTableSize() * getProportions()->getScale();
		int r = tabSz / 2;
		agui::Point tabCenter = agui::Point(
			w * getProportions()->getTableOffset().getX(),
			h * getProportions()->getTableOffset().getY()); 

		float widthRatio = solveWidthRatio(m_passedCards->isVisible() ? 3 : 2);
		int theOffset = getProportions()->isInCompactMode() ? 105 : 140;
		int offset = theOffset * getProportions()->getScale();
		int offset2 = getProportions()->isInCompactMode() ? offset *  0.45f : offset * 0.65f;
		int height = solveHeight(offset,widthRatio,tabCenter.getX(),h - tabCenter.getY(),r );
		if(height > getProportions()->getScreenHeight() * 0.25f)
			height = getProportions()->getScreenHeight() * 0.25f;
		
        if(Platf::isMobile() && m_passedCards->isVisible())
            height *= 0.95f;
        
        if(!m_passedCards->isVisible())
			height *= 0.85f;

		height *= 0.93f;

		if(!Platf::isHD() && m_passedCards->isVisible())
			height *= 0.92;

		if(Platf::isHD())
			height *= 0.85;
		int width = height * widthRatio;

		agui::Point location = agui::Point(offset2 - m_frame->getMargin(agui::SIDE_LEFT),
			h - height - offset2 - m_frame->getMargin(agui::SIDE_TOP));
		
		width += m_frame->getMargin(agui::SIDE_LEFT);
		width += m_frame->getMargin(agui::SIDE_RIGHT);
		height += m_frame->getMargin(agui::SIDE_TOP);
		height += m_frame->getMargin(agui::SIDE_BOTTOM);

		int borderMargin = calcObjSz(width);

		m_frame->setSize(width,height);
		m_frame->setLocation(location);

		m_borderLayout->setBorderMargins(0,0,borderMargin,borderMargin);
	}

	int DataWidgetsHandler::calcObjSz( int w )
	{
		int numWnd = 3;
		if(!m_passedCards->isVisible())
		{
			numWnd--;
		}

		w -= (m_frame->getMargin(agui::SIDE_RIGHT) + m_frame->getRightMargin());
		w -= m_frame->getMargin(agui::SIDE_LEFT);
		w -= m_borderLayout->getHorizontalSpacing() * (numWnd - 1);

		w /= numWnd;
		return w;
	}

	int DataWidgetsHandler::calcHeight( int objSz )
	{
		int height = objSz;
		height += m_frame->getMargin(agui::SIDE_BOTTOM) + m_frame->getBottomMargin();
		height += m_frame->getMargin(agui::SIDE_TOP);

		return height;
	}

	int DataWidgetsHandler::solveHeight( int offset, float widthRatio, float h, float k, float r ) const
	{
		float denom = (widthRatio * widthRatio) + 1.0f;
		float negVal = -offset - (widthRatio * 2 * offset);
		if(denom == 0)
		{
			//error
			return -1.0f;
		}
		float oneOver = 1.0f / denom;
		float widH = widthRatio * h;

		float firstPart = widH + k + negVal;
		float eq = (widthRatio * offset) - (2 * offset) + h - (widthRatio * k);
		float eqSq = eq * eq;
		float wRadius = denom * r * r;

		return oneOver * (firstPart - sqrt(wRadius - eqSq));

	}

	float DataWidgetsHandler::solveWidthRatio( int numObj ) const
	{
		int numSide = numObj - 1;
		float val = numObj;
		val += (m_frame->getLeftMargin() +  m_frame->getLeftMargin() + m_frame->getRightMargin() + (numSide * m_borderLayout->getHorizontalSpacing())) 
			/ (m_frame->getTopMargin() +  m_frame->getTopMargin() + m_frame->getBottomMargin());

		return val;
	}

	void DataWidgetsHandler::setPassingCards( bool passing )
	{
		m_passingCards = passing;
		m_passedCards->setVisibility(passing);
		resize(m_width,m_height);
	}

}


