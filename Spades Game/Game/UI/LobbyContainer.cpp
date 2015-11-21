#include "Game/UI/LobbyContainer.hpp"
namespace cge
{
	LobbyContainer::~LobbyContainer(void)
	{
		getInner()->removeWidgetListener(this);
		getOuter()->removeWidgetListener(this);
	}

	void LobbyContainer::addOuter( agui::Widget* source,agui::BorderLayout::BorderLayoutEnum which )
	{
		m_outerBorder->add(source,which);
	}

	void LobbyContainer::addInner( agui::Widget* source,agui::BorderLayout::BorderLayoutEnum which )
	{
		m_innerBorder->add(source,which);
	}

	LobbyContainer::LobbyContainer( GuiFactory* factory, LanguageManager* language )
		: m_filterManager(NULL), m_filterOn(false)
	{
		m_innerBorder = factory->createBorderLayout();
		m_outerBorder = factory->createBorderLayout();
		m_innerBorder->setMargins(0,0,0,0);
		m_outerBorder->setMargins(0,0,0,0);
		m_innerBorder->setResizable(false);
		m_outerBorder->setResizable(false);
		addOuter(m_innerBorder,agui::BorderLayout::CENTER);
		m_innerBorder->addWidgetListener(this);
		m_outerBorder->addWidgetListener(this);

	}

	agui::Widget* LobbyContainer::getWidget()
	{
		return m_outerBorder;
	}

	void LobbyContainer::toggleFilterMenu( bool on )
	{
		m_filterOn = on;
		if(on && m_filterManager)
		{
			for(int i = 0; i < 2; ++i)
			{
				getInner()->setBorderMargin(
					agui::BorderLayout::SOUTH,m_filterManager->getHeight());
				m_filterManager->resize();
				getInner()->setBorderMargin(
					agui::BorderLayout::SOUTH,m_filterManager->getHeight());
			}
		}
		else
		{
			getInner()->setBorderMargin(
				agui::BorderLayout::SOUTH,0);
		}
	}

	void LobbyContainer::setFilterManager(LobbyFilterManager* filter)
	{
		m_filterManager = filter;
	}

	BorderLayout* LobbyContainer::getInner()
	{
		return m_innerBorder;
	}

	BorderLayout* LobbyContainer::getOuter()
	{
		return m_outerBorder;
	}

	void LobbyContainer::sizeChanged( 
		agui::Widget* source, const agui::Dimension &size )
	{
		if(source == getInner())
		if(m_filterOn && m_filterManager)
		{
			
			for(int i = 0; i < 2; ++i)
			{
				getInner()->setBorderMargin(
					agui::BorderLayout::SOUTH,m_filterManager->getHeight());
				m_filterManager->resize();
				getInner()->setBorderMargin(
					agui::BorderLayout::SOUTH,m_filterManager->getHeight());
			}
		}
		else
		{
			getInner()->setBorderMargin(
				agui::BorderLayout::SOUTH,0);
		}

		if(source == getOuter())
		{
			int margin = size.getWidth() * 0.18f;

			int minMar = 190;
			int maxMar = 220;
            
            if(Platf::isRetina())
            {
                minMar *= 1.3f;
                maxMar *= 1.3f;
            }

			if((float)size.getWidth() / (float)size.getHeight() < 1.55f)
			{
			   margin *= 1.25f;
			}

			if(margin < minMar)
			{
				margin = minMar;
			}


			int maxMargin = maxMar;
			if(margin > maxMargin)
			{
				float whatItTakes = maxMargin / 0.18f;
				float newAddition = size.getWidth() - whatItTakes;
				margin = maxMargin;
				margin += newAddition * 0.14f;
			}

			getOuter()->setBorderMargin(agui::BorderLayout::EAST,margin);
			getOuter()->setBorderMargin(agui::BorderLayout::WEST,margin);
		}
	}

	void LobbyContainer::resize( int w, int h )
	{
		sizeChanged(getInner(),getInner()->getSize());
		sizeChanged(getOuter(),getOuter()->getSize());
	}

}

