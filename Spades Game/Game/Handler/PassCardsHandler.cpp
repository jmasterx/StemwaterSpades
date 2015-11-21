#include "Game/Handler/PassCardsHandler.hpp"
namespace cge
{
	PassCardsHandler::~PassCardsHandler(void)
	{
	}

	PassCardsHandler::PassCardsHandler( 
		GuiFontManager* fontManager,LanguageManager* languageManager, GuiFactory* factory, agui::Gui* gui )
		: m_fontManager(fontManager)
	{
		m_button = factory->createOrbButton();
		m_button->setText(languageManager->getElement("pass.across"));
		gui->add(m_button);
		m_button->setFocusable(false);
		m_button->setMultiline(true);
		m_button->setVisibility(false);
		m_button->addActionListener(this);
	}

	void PassCardsHandler::resize( int w, int h )
	{
		int width = 300.0f * getProportions()->getScale();
		m_button->setSize(width,width);
		m_button->setMargins(59 * getProportions()->getScale(),
			75 * getProportions()->getScale(),
			70 * getProportions()->getScale(),70 * getProportions()->getScale());
			int fontSz = (int)((float)m_button->getInnerWidth() * 0.17f);
		
		m_button->setFont(m_fontManager->getFont(fontSz));
		
		
		float tw = (float)getProportions()->getTableSize() * getProportions()->getScale();
		int startX = (float)getProportions()->getScreenWidth() * 
			getProportions()->getTableOffset().getX();
		startX -= (width / 2);

		int startY = (float)getProportions()->getScreenHeight() * 
			getProportions()->getTableOffset().getY();
		startY -= (width / 2);

		m_button->setLocation(startX,startY);
	}

	void PassCardsHandler::actionPerformed(const agui::ActionEvent& evt )
	{
		for(std::vector<ClientEventListener*>::iterator it = 
			m_listeners.begin();
			it != m_listeners.end(); ++it)
		{
			(*it)->endPassCards();
		}
		m_button->setVisibility(false);
	}

	void PassCardsHandler::addListener( ClientEventListener* listener )
	{
		m_listeners.push_back(listener);
	}

	void PassCardsHandler::removeListener( ClientEventListener* listener )
	{
		m_listeners.erase(
			std::remove(m_listeners.begin(),
			m_listeners.end(), listener),
			m_listeners.end());
	}

	void PassCardsHandler::passCardsBegin()
	{
		m_button->setVisibility(true);
		m_button->setEnabled(false);
		resize(getProportions()->getScreenWidth(),getProportions()->getScreenHeight());
	}

	void PassCardsHandler::passButtonEnabledChanged( bool enabled )
	{
		m_button->setEnabled(enabled);
	}

	void PassCardsHandler::passButtonVisibilityChanged( bool visible )
	{
		m_button->setVisibility(visible);
	}

}
