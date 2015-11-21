#include "Game/UI/GameInformation.hpp"
#include "Game/UI/GuiFactory.hpp"
#include "Game/UI/LobbyCoinDisplay.hpp"
#include "Game/Utility/StringUtil.hpp"
#include <sstream>
namespace cge
{
	GameInformation::~GameInformation(void)
	{
		m_infoButton->removeButtonListener(this);
		delete m_coinDisplay;
	}

	void GameInformation::paintComponent( const agui::PaintEvent &paintEvent )
	{

	}

	void GameInformation::paintBackground( const agui::PaintEvent &paintEvent )
	{
		if(m_bg)
		{
			paintEvent.graphics()->drawNinePatchImage(m_bg,agui::Point(0,0),getSize(),0.87f);
		}
	}

	GameInformation::GameInformation( agui::Image* bg,
		LanguageManager* language, GuiFontManager* font, GuiFactory* factory, bool gameIsTimeLimited )
		:m_bg(bg),m_language(language),m_font(font),m_factory(factory),
		m_coinDisplay(NULL),m_numSecondsGone(0),m_totalSeconds(0),
		m_gameIsTimeLimited(gameIsTimeLimited)
	{
		m_coinDisplay = new LobbyCoinDisplay(factory);

		setVisibility(false);
		setMargins(6,8,6,9);
		setMaxOnRow(1);
		setHorizontallyCentered(true);
		setResizeToParent(false);
		m_tableLabel = factory->createLabel();
		m_tableLabel->setFontColor(agui::Color(
			m_tableLabel->getFontColor().getR() * 0.8f,
			m_tableLabel->getFontColor().getG() * 0.8f,
			m_tableLabel->getFontColor().getB() * 0.8f
			));

		m_infoButton = m_factory->createWhiteButton();
		m_infoButton->setText(m_language->getElement("game.settings"));
        if(!Platf::isRetina())
		m_infoButton->setFont(m_font->getFont(0.8f));
        else
        m_infoButton->setFont(m_font->getFont(0.8f * 2.0f));
        
		m_infoButton->resizeToContents();
		m_infoButton->addButtonListener(this);
		m_infoButton->setToggleButton(true);
		m_infoButton->setFocusable(false);

		m_infoButton->setFontColor(agui::Color(
			m_infoButton->getFontColor().getR() * 0.75f,
			m_infoButton->getFontColor().getG() * 0.75f,
			m_infoButton->getFontColor().getB() * 0.75f
			));

		m_leaveButton = factory->createWhiteButton();
		m_leaveButton->setText(m_language->getElement("game.leave"));
		m_leaveButton->setFont(m_font->getFont(0.9f));
		m_leaveButton->resizeToContents();
		m_leaveButton->setFontColor(agui::Color(
			m_leaveButton->getFontColor().getR() * 0.8f,
			m_leaveButton->getFontColor().getG() * 0.8f,
			m_leaveButton->getFontColor().getB() * 0.8f
			));
		m_leaveButton->setFocusable(false);

		m_grid = m_factory->createGridLayout();
		m_grid->setNumberOfColumns(2);
		m_grid->setNumberOfRows(0);
		m_grid->setHorizontalSpacing(10);
		//m_grid->setver
		m_grid->setMargins(1,1,1,0);

		m_typeLabel = createLabel("game.type");
		m_typeRLabel = createLabel("");
		m_scoreLabel = createLabel(""); //later
		m_scoreRLabel = createLabel("");
		m_nilLabel = createLabel("game.nil");
		m_nilRLabel = createLabel("");
		m_gameLabel = createLabel("game.game");
		m_gameRLabel = createLabel("");
		//m_cutDeckLabel = createLabel("game.deckcut");
		//m_cutDeckRLabel = createLabel("");
		m_watchLabel = createLabel("game.watch");
		m_watchRLabel = createLabel("");
		m_feeLabel = createLabel("table.fee");
		m_feeRLabel = createLabel("");
		if(m_gameIsTimeLimited)
		{
			m_timeLabel = createLabel("game.time");
			m_timeRLabel = createLabel("");
		}
		setGridHeight();
		add(m_tableLabel);
		add(m_grid);
		//add(m_leaveButton);
		add(m_coinDisplay->getWidget());
		m_coinDisplay->getWidget()->setHorizontallyCentered(true);
		m_coinDisplay->setColor(m_tableLabel->getFontColor());
	}

	Label* GameInformation::createLabel(const std::string& elemName)
	{
		Label* l = m_factory->createLabel();
		l->setFont(m_font->getFont(0.85f));
		l->setAutosizing(false);
		l->setMargins(0,0,0,0);

		l->setFontColor(agui::Color(
			l->getFontColor().getR() * 0.9f,
			l->getFontColor().getG() * 0.9f,
			l->getFontColor().getB() * 0.9f
			));

		if(elemName != "")
		{
			l->setText(m_language->getElement(elemName));
		}

		m_grid->add(l);

		return l;
	}

	void GameInformation::loadSettings( ClientShared* shared )
	{
		int tableNo = getTableG();
		SpadesGameInfo info = getGameInfoG();
		setContent(info,tableNo);

		bool opened = shared->getSettingsManager()->getBoolSetting("game.settings.on");
		setVisibility(opened);
		m_infoButton->setToggleState(opened);

		if(m_gameIsTimeLimited)
		{
			m_totalSeconds = 60 * shared->getNewGameData()->getGameInfo().getEndValue();
			refreshTime();
		}
	}

	void GameInformation::setContent( const SpadesGameInfo& info, int tableNo )
	{
		std::string table;
		if(tableNo != -1)
		{
			table = m_language->getElement("game.table") + " " + toString(tableNo);
		}
		else
		{
			table = m_language->getElement("game.single");
		}
		
		m_tableLabel->setText(table);

		std::string gType;
		if(info.isPassTwo())
		{
			gType = m_language->getElement("filter.pass");
		}
		else if(info.isPartnership())
		{
			gType = m_language->getElement("filter.regular");
		}
		else if(info.isCutthroat())
		{
			gType = m_language->getElement("filter.cutthroat");
		}
		else if(info.isIndividual())
		{
			gType = m_language->getElement("filter.individual");
		}

		gType += " ";

		if(info.isMirror())
		{
			gType += m_language->getElement("filter.mirrorbid");
		}
		else if(info.isSuicide())
		{
			gType += m_language->getElement("filter.suicidebid");
		}

		m_typeRLabel->setText(gType);

		std::string nilVal = toString(info.m_nilValue) + " " + m_language->getElement("points");
		m_nilRLabel->setText(nilVal);

		std::string rated = info.isSocial() ?  
			m_language->getElement("filter.social") : m_language->getElement("filter.rated");
		m_gameRLabel->setText(rated);

        /*
		std::string cutDeck = info.isDeckCutEnabled() ? 
			m_language->getElement("yes") : m_language->getElement("no");
		m_cutDeckRLabel->setText(cutDeck);
         */

		std::string watch = info.isWatchingAllowed() ? 
			m_language->getElement("yes") : m_language->getElement("no");
		m_watchRLabel->setText(watch);

		std::string score;
		std::string scoreRT;
		if(info.isScoreEnded())
		{
			score = m_language->getElement("filter.score");
			scoreRT = m_language->getElement("points");
		}
		else if(info.isTimeEnded())
		{
			score = m_language->getElement("filter.time");
			scoreRT = m_language->getElement("minutes");
		}
		else if(info.isHandEnded())
		{
			score = m_language->getElement("filter.hand");
			scoreRT = m_language->getElement("hands");
		}

		m_scoreLabel->setText(score);

		std::string scoreR = toString(info.getEndValue()) + " " + scoreRT;
		m_scoreRLabel->setText(scoreR);

		std::string feeR = StringUtil::commaSeparate(toString(info.getFee()));

		if(info.getFee() == 0)
			m_feeRLabel->setText(m_language->getElement("no.fee"));
		else
			m_feeRLabel->setText(m_language->getElementWithArg("n.coins",feeR));

		resizeContents();
	}

	std::string GameInformation::toString( int i ) const
	{
		std::stringstream ss;
		ss << i;
		return ss.str();
	}

	void GameInformation::setSize( const agui::Dimension &size )
	{
		agui::FlowLayout::setSize(size);
		m_grid->setSize(getInnerWidth(),m_grid->getHeight());
		m_tableLabel->setSize(getInnerWidth(),m_tableLabel->getHeight());
	}

	void GameInformation::setSize( int width, int height )
	{
		agui::FlowLayout::setSize(width,height);
	}

	void GameInformation::resizeContents()
	{
		int longest = getLongestLabel() * 2.0f +
			m_grid->getHorizontalSpacing() + 
			m_grid->getMargin(agui::SIDE_LEFT) +
			m_grid->getMargin(agui::SIDE_RIGHT);

		if(m_coinDisplay->getWidget()->getWidth() > longest)
		{
			longest = m_coinDisplay->getWidget()->getWidth();
		}

		longest += getMargin(agui::SIDE_LEFT) + getMargin(agui::SIDE_RIGHT);
		longest += m_typeLabel->getFont()->getLineHeight() * 0.55f;
		setSize(longest,getContentsHeight());
		setSize(longest,getContentsHeight());
	}

	void GameInformation::resize( int w, int h )
	{
		float ctrl = 0.75f;
        
        if(Platf::isRetina())
            ctrl *= 1.5f;
        
		float scale = getProportions()->getScale();
		if(scale > 0.55f)
		{
			scale = 0.55f;
		}

		m_tableLabel->setFont(m_font->getFont(scale * 2.4f * ctrl));
		m_tableLabel->resizeToContents();
		m_leaveButton->setFont(m_font->getFont(scale * 2.0f * ctrl));
		m_leaveButton->resizeToContents();

		m_coinDisplay->setFontSize((agui::Font*)m_tableLabel->getFont());
		m_coinDisplay->setCoinSize(m_tableLabel->getFont()->getLineHeight());
		m_coinDisplay->reMargin();
		m_coinDisplay->getWidget()->setHorizontalSpacing(2);

		for(agui::WidgetArray::iterator it = m_grid->getChildBegin(); it != m_grid->getChildEnd(); ++it)
		{
			(*it)->setFont(m_font->getFont(scale * 1.9f * ctrl));
		}

		m_grid->setHorizontalSpacing(1.5f * scale);
		setVerticalSpacing(12 * scale);
		m_grid->setVerticalSpacing(2.0f *  scale);
		setGridHeight();
		resizeContents();
		
		int offset = 55;
        if(Platf::isRetina())
            offset *= 2.3f;
        
		int selfX = w - getWidth() - offset;
		int parentX = selfX;

		int centerInfoX = (getWidth() - m_infoButton->getWidth()) / 2;
		parentX += centerInfoX;
		m_infoButton->setLocation(parentX,-5);

		setLocation(selfX,m_infoButton->getLocation().getY() +
			m_infoButton->getHeight() * 0.85f);
	}

	Button* GameInformation::getWidget()
	{
		return m_infoButton;
	}

	void GameInformation::setGridHeight()
	{
		int numRows =  8 + (m_gameIsTimeLimited ? 1 : 0);
		int gridHeight = numRows * (m_typeLabel->getFont()->getLineHeight() + m_grid->getVerticalSpacing());
		m_grid->setSize(getInnerWidth(),gridHeight);
	}

	void GameInformation::toggleStateChanged( agui::Button *source, bool state )
	{
		if(source == m_infoButton)
		{
			setVisibility(state);
			DISPATCH_SCENE_EVENT
				(*it)->setBoolSetting("game.settings.on",state);
		}
	}

	Button* GameInformation::getLeaveButton()
	{
		return m_leaveButton;
	}

	int GameInformation::getLongestLabelA()
	{
		int longest = 0;
		int curLen = 0;

		curLen = m_typeLabel->getFont()->getTextWidth(m_typeLabel->getText());

		if(curLen > longest)
			longest = curLen;

		curLen = m_scoreLabel->getFont()->getTextWidth(m_scoreLabel->getText());

		if(curLen > longest)
			longest = curLen;

		curLen = m_nilLabel->getFont()->getTextWidth(m_nilLabel->getText());

		if(curLen > longest)
			longest = curLen;

		//curLen = m_cutDeckLabel->getFont()->getTextWidth(m_cutDeckLabel->getText());

		if(curLen > longest)
			longest = curLen;

		curLen = m_watchLabel->getFont()->getTextWidth(m_watchLabel->getText());

		if(curLen > longest)
			longest = curLen;

		
		curLen = m_gameLabel->getFont()->getTextWidth(m_gameLabel->getText());

		if(curLen > longest)
			longest = curLen;
		

		return longest;
	}

	int GameInformation::getLongestLabelB()
	{
		int longest = 0;
		int curLen = 0;

		curLen = m_typeRLabel->getFont()->getTextWidth(m_typeRLabel->getText());

		if(curLen > longest)
			longest = curLen;

		curLen = m_scoreRLabel->getFont()->getTextWidth(m_scoreRLabel->getText());

		if(curLen > longest)
			longest = curLen;

		curLen = m_nilRLabel->getFont()->getTextWidth(m_nilRLabel->getText());

		if(curLen > longest)
			longest = curLen;

	//	curLen = m_cutDeckRLabel->getFont()->getTextWidth(m_cutDeckRLabel->getText());

		if(curLen > longest)
			longest = curLen;

		curLen = m_watchRLabel->getFont()->getTextWidth(m_watchRLabel->getText());

		if(curLen > longest)
			longest = curLen;

		
		curLen = m_gameRLabel->getFont()->getTextWidth(m_gameRLabel->getText());

		if(curLen > longest)
			longest = curLen;
			

		return longest;
	}

	int GameInformation::getLongestLabel()
	{
		int longest = 0;
		for(agui::WidgetArray::iterator it = m_grid->getChildBegin(); it != m_grid->getChildEnd(); ++it)
		{
			int tw = (*it)->getFont()->getTextWidth((*it)->getText());
			if(tw > longest)
			{
				longest = tw;
			}
		}
		return longest;
	}

	LobbyCoinDisplay* GameInformation::getCoinDisplay()
	{
		return m_coinDisplay;
	}

	void GameInformation::coinAmountChangedG( int64 amount, bool update, bool fromPurchase, bool spending )
	{
		m_coinDisplay->coinAmountChanged(amount,update,fromPurchase,spending);
		resize(getProportions()->getScreenWidth(),getProportions()->getScreenHeight());
	}

	void GameInformation::refreshTime()
	{
		int diff = m_totalSeconds - m_numSecondsGone;
		if(diff < 0)
			diff = 0;

		int minutes = (diff % 3600) / 60;
		int seconds = diff % 60;

		std::stringstream ss;
		ss << minutes << ":" <<(seconds < 10 ? "0" : "") << seconds;
		m_timeRLabel->setText(ss.str());
	}

	void GameInformation::gameTimeChanged( int secondsElapsed )
	{
		if(m_gameIsTimeLimited)
		{
			m_numSecondsGone = secondsElapsed;
			refreshTime();
		}
	}

}

