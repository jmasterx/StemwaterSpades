#include "Game/UI/LobbyTable.hpp"
#include "Game/Handler/LobbyEventListener.hpp"
#include "Game/Utility/StringUtil.hpp"
#include <sstream>
namespace cge
{
	LobbyTable::~LobbyTable(void)
	{
	}

	LobbyTable::LobbyTable(AvatarCache* cache, GuiColorManager* color, GuiFontManager* font, SpriteManager* sprite, int tableId, ClientPlayer* self )
		: m_fontManager(font), m_tableId(tableId), m_type(NOT_SET_TT),
		m_baseWidth(200), m_baseHeight(200),m_table(sprite->getImage("lobby.table"),
		sprite->getImage("lobby.table.shadow")), m_gameInfoSet(false),
		m_colorManager(color),m_locked(false), m_beingSetup(false)
	{
		for(int i = 0; i < getNumChairs(); ++i)
		{
				m_chairs[i] = LobbyChairComponent(
				sprite->getImage("lobby.chair"),
				sprite->getImage("lobby.chair.hover"),cache,i,self);
		}

		for(int i = 0; i < getNumChairs(); ++i)
		{
			m_chairs[i].setBaseScale(0.1f);
		}

		m_table.setBaseScale(0.2f);
		std::stringstream ss;
		ss << m_tableId;
		m_table.setText(ss.str());
		setGameInfoSet(false);
		//m_table.setAngle((m_tableId % 8) * (3.1415f / 4.0f));
	}

	LobbyTable::LobbyTable()
		: m_table(NULL,NULL), m_gameInfoSet(false)
	{

	}

	void LobbyTable::render( GraphicsContext* g )
	{
		agui::Rectangle r = getParent()->getParent()->getAbsoluteRectangle();
		int cx, cy, cw, ch;
		g->getClippingRect(cx,cy,cw,ch);
		g->setClippingRect(r.getX(),r.getY(),r.getWidth(),r.getHeight());

		for(int i = 0; i < getNumChairs(); ++i)
		{
			m_chairs[i].render(g);
		}

		renderTable(g);

		g->setClippingRect(cx,cy,cw,ch);
	}

	void LobbyTable::paintBackground( const agui::PaintEvent &paintEvent )
	{
	}

	void LobbyTable::paintComponent( const agui::PaintEvent &paintEvent )
	{

	}

	void LobbyTable::resize( int w, int h )
	{
	}

	void LobbyTable::rescale( double scale, int numOnRow )
	{
		setScale(scale);
		float os = getObjectScale();
		double x = m_baseHeight * os;
	
		if( (x + 0.5) >= (int(x) + 1) )
			x = (int) x + 1;
		
		int oldH = getHeight();

		setSize(m_baseWidth * os, x);
		
		agui::Font* f = m_fontManager->getFont((float)scale * 1.05f);
		m_table.setFont(f);
		m_table.setSubFont(m_fontManager->getFont(f,0.8f));
		m_table.setRatedFont(m_fontManager->getFont(f,0.75f));
		m_table.setWatchFont(m_fontManager->getFont(f,0.6f));
		m_table.setFeeFont(m_fontManager->getFont(f,0.55f));
		m_table.setPtsFont(m_fontManager->getFont(f,0.53f));
		m_table.setScale(scale);
		m_table.resizeToScale();
		m_table.alignToParent(agui::ALIGN_MIDDLE_CENTER);
		

		for(int i = 0; i < getNumChairs(); ++i)
		{
			m_chairs[i].setScale(scale);
			m_chairs[i].resizeToScale();
			m_chairs[i].setScaledFont(m_fontManager,numOnRow);

			m_chairs[i].relocate(
				m_table.getCenter().getX(),m_table.getCenter().getY());
		}

	}

	bool LobbyTable::operator<( const LobbyTable &rhs )
	{
		return m_tableId < rhs.m_tableId;
	}

	void LobbyTable::addChairActionListeners( agui::ActionListener* listener )
	{
		for(int i = 0; i < getNumChairs(); ++i)
		{
			m_chairs[i].addActionListener(listener);
		}

		m_table.addActionListener(listener);
	}

	void LobbyTable::removeChairActionListeners( agui::ActionListener* listener )
	{
		for(int i = 0; i < getNumChairs(); ++i)
		{
			m_chairs[i].removeActionListener(listener);
		}

		m_table.removeActionListener(listener);
	}

	int LobbyTable::getNumChairs() const
	{
		return 4;
	}

	LobbyChairComponent* LobbyTable::getChairAt( int i )
	{
		return &m_chairs[i];
	}

	int LobbyTable::getTableId() const
	{
		return m_tableId;
	}

	void LobbyTable::addChairMouseListeners( agui::MouseListener* listener )
	{
		for(int i = 0; i < getNumChairs(); ++i)
		{
			m_chairs[i].addMouseListener(listener);
		}
	}

	void LobbyTable::removeChairMouseListeners( agui::MouseListener* listener )
	{
		for(int i = 0; i < getNumChairs(); ++i)
		{
			m_chairs[i].removeMouseListener(listener);
		}
	}

	LobbyTableComponent* LobbyTable::getTable()
	{
		return &m_table;
	}

	void LobbyTable::setChildVisibilities( bool visible )
	{
		if(m_table.isVisible() == visible)
		{
			return;
		}
		else
		{
			m_table.setVisibility(visible);
			for(int i = 0; i < getNumChairs(); ++i)
			{
				m_chairs[i].setVisibility(visible);
			}
		}
	}

	void LobbyTable::addChildrentToSelf()
	{
		for(int i = 0; i < getNumChairs(); ++i)
		{
			add(&m_chairs[i]);
		}

		add(&m_table);
	}

	void LobbyTable::renderTable( GraphicsContext* g )
	{
		m_table.render(g);
	}

	void LobbyTable::renderChairs(GraphicsContext* g)
	{
		for(int i = 0; i < getNumChairs(); ++i)
		{
			m_chairs[i].render(g);
		}
	}

	void LobbyTable::setGameInfoSet( bool set )
	{
		m_gameInfoSet = set;
		setStarted(false);
		if(!set)
		{
			m_table.setFontColor(m_colorManager->getColor("filter.none"));
			changeTableSubText();
		}

		if(set && getGameInfo().getNumPlayers() == 3)
		{
			getChairAt(3)->setOff(true);
		}
		else
		{
			getChairAt(3)->setOff(false);
		}
	}

	void LobbyTable::setGameInfo( const SpadesGameInfo& info )
	{
		m_gameInfo = info;
		setGameInfoSet(true);

		agui::Color theColor;

		if(info.isPassTwo())
		{
			theColor = m_colorManager->getColor("filter.pass");
			m_table.setTypeText(m_pass2Short);
		}
		else if(info.isPartnership())
		{
			theColor = m_colorManager->getColor("filter.regular");
			m_table.setTypeText(m_partnershipShort);
		}
		else if(info.isCutthroat())
		{
			theColor = m_colorManager->getColor("filter.cutthroat");
			m_table.setTypeText(m_cutthroatShort);
		}
		else if(info.isIndividual())
		{
			theColor = m_colorManager->getColor("filter.individual");
			m_table.setTypeText(m_individialShort);
		}

		getTable()->setFontColor(theColor);

		if(info.isSocial())
		{
			theColor = m_colorManager->getColor("host.social");
		}
		else
		{
			theColor = m_colorManager->getColor("host.rated");
		}

		getTable()->setRatedColor(theColor);
		changeTableSubText();
	}

	void LobbyTable::setMirrorSuicideNames( const std::string& mirror, const std::string& suicide )
	{
		m_mirrorShort = mirror;
		m_suicideShort = suicide;
		changeTableSubText();
	}

	void LobbyTable::changeTableSubText()
	{
		if(m_gameInfoSet)
		{
			std::stringstream ssp;
			ssp << m_gameInfo.getEndValue() << " " << m_points;
			m_table.setPtsText(ssp.str());

			if(m_gameInfo.isMirror())
			{
				m_table.setSubText(m_mirrorShort);
			}
			else if(m_gameInfo.isSuicide())
			{
				m_table.setSubText(m_suicideShort);
			}
			if(m_gameInfo.isWatchingAllowed())
			{
				m_table.setWatchText(m_watchSymbol);
			}

			if(m_gameInfo.isRated())
			{
				m_table.setRatedText(m_ratedShort);
			}
			else
			{
				m_table.setRatedText(m_socialShort);
			}

			if(m_gameInfo.getFee() <= 0)
			{
				m_table.setFeeText(m_noFee);
			}
			else
			{
				std::stringstream ss;
				ss << "$" << StringUtil::commaSeparate(StringUtil::toString(m_gameInfo.getFee()));
				m_table.setFeeText(ss.str());
			}
		}
		else
		{
			m_table.setTypeText("");
			m_table.setRatedText("");
			m_table.setSubText("");
			m_table.setWatchText("");
			m_table.setFeeText("");
			m_table.setPtsText("");
		}
	}

	const SpadesGameInfo& LobbyTable::getGameInfo() const
	{
		return m_gameInfo;
	}

	bool LobbyTable::isGameInfoSet() const
	{
		return m_gameInfoSet;
	}

	void LobbyTable::setDarkness( float darkness )
	{
		if(darkness < 0.0f)
			darkness = 0.0f;
		if(darkness > 1.0f)
			darkness = 1.0f;
		m_table.setDarkness(darkness);
		for(int i = 0; i < getNumChairs(); ++i)
		{
			m_chairs[i].setDarkness(darkness);
		}
	}

	void LobbyTable::setEnabled( bool enabled )
	{
		m_table.setEnabled(enabled);
		for(int i = 0; i < getNumChairs(); ++i)
		{
			m_chairs[i].setDark(!enabled);
		}

		if(enabled)
		{
			setDarkness(1.0f);
		}
		else
		{
			setDarkness(0.5f);
		}
	}

	void LobbyTable::unoccupyChair( int chairNo )
	{
		if(chairNo < getNumChairs())
		{
			getChairAt(chairNo)->setOccupied(false);
			//test
			getChairAt(chairNo)->setToolTipText("");
		}
	}

	void LobbyTable::occupyChair( int chairNo, ClientPlayer* occupant )
	{
		if(chairNo < getNumChairs())
		{
			getChairAt(chairNo)->setOccupant(occupant);
			getChairAt(chairNo)->setOccupied(true);

			//test
			getChairAt(chairNo)->setText(occupant->getUsername());
		}
	}

	int LobbyTable::getNumOccupants() const
	{
		int n = 0;
		for(int i = 0; i < getNumChairs(); ++i)
		{
			if(m_chairs[i].isOccupied())
			{
				n++;
			}
		}

		return n;
	}

	bool LobbyTable::isEmpty() const
	{
		return getNumOccupants() == 0;
	}

	bool LobbyTable::isFull() const
	{
		return isGameInfoSet() && 
			(getGameInfo().getNumPlayers() == getNumOccupants() || getGameInfo().isBotGame());
	}

	bool LobbyTable::isLocked() const
	{
		return m_locked;
	}

	void LobbyTable::lock( bool locked )
	{
		m_locked = locked;
		for(int i = 0; i < getNumChairs(); ++i)
		{
			m_chairs[i].setLocked(locked);

			if(i == 3 && isGameInfoSet() && getGameInfo().getNumPlayers() < 4)
			{
				m_chairs[i].setDark(true);
				m_chairs[i].setDarkness(0.5f);
			}

			/*
			if(!m_chairs[i].isOccupied() && isGameInfoSet() && getGameInfo().isBotGame())
			{
				m_chairs[i].setDark(true);
				m_chairs[i].setDarkness(0.5f);
			}
			*/
		}
	}

	void LobbyTable::tableBeingSetup( bool setup )
	{
		if(setup)
		{
			getTable()->setTypeText("...");
			m_beingSetup = true;
		}
		else
		{	m_beingSetup = false;
			getTable()->setTypeText("");
		}
	}

	bool LobbyTable::isBeingSetup() const
	{
		return m_beingSetup;
	}

	void LobbyTable::setWatchSymbol( const std::string & symbol )
	{
		m_watchSymbol = symbol;
	}

	void LobbyTable::setCanWatch( bool canWatch )
	{
		m_table.setCanWatch(canWatch);
	}

	void LobbyTable::showWatchText( bool show )
	{
		if(show)
		{
			m_table.setWatchText(m_watchSymbol);
		}
		else
		{
			m_table.setWatchText("");
		}
	}

	bool LobbyTable::isWatchable() const
	{
		if(!isGameInfoSet())
		{
			return false;
		}
		else
		{
			return getGameInfo().isWatchingAllowed() && getNumOccupants() == getGameInfo().getNumPlayers();
		}
	}

	void LobbyTable::setStarted( bool started )
	{
		m_table.setStarted(started);
	}

	bool LobbyTable::isStarted() const
	{
		return m_table.isStarted();
	}

	void LobbyTable::renderChairTexts( GraphicsContext* g )
	{
		for(int i = 0; i < getNumChairs(); ++i)
		{
			m_chairs[i].renderText(g);
		}
	}

	void LobbyTable::setGameTypeNames( const std::string& pass2, const std::string& partner,const std::string& individual, const std::string& cutthroat )
	{
		m_pass2Short = pass2;
		m_partnershipShort = partner;
		m_individialShort = individual;
		m_cutthroatShort = cutthroat;
	}

	void LobbyTable::renderTableShadow( GraphicsContext*g )
	{
		m_table.renderShadow(g);
	}

	void LobbyTable::renderChairPlayers( GraphicsContext* g )
	{
		for(int i = 0; i < getNumChairs(); ++i)
		{
			m_chairs[i].renderPlayer(g);
		}
	}

	void LobbyTable::renderTableShadowWeak( GraphicsContext*g )
	{
		m_table.renderShadowWeak(g);
	}

	void LobbyTable::setRatedTexts( const std::string&rated, const std::string& social )
	{
		m_ratedShort = rated;
		m_socialShort = social;
	}

	void LobbyTable::setFeeText( const std::string& text )
	{
		m_fee = text;
	}

	void LobbyTable::update()
	{
		for(int i = 0; i < getNumChairs(); ++i)
		{
			m_chairs[i].update();
		}
	}

	void LobbyTable::setPointsText( const std::string& text )
	{
		m_points = text;
	}

	void LobbyTable::setNoFeeText( const std::string& text )
	{
		m_noFee = text;
	}


}
