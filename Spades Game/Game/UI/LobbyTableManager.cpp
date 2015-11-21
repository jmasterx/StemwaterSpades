#include "Game/UI/LobbyTableManager.hpp"
#include "Game/UI/GuiFactory.hpp"
#include <iostream>
#include <sstream>
#include "Game/Utility/StringUtil.hpp"
#define MSG_WANT_QUIT 1
namespace cge
{
	LobbyTableManager::LobbyTableManager( AvatarCache* cache, GuiFactory* factory, 
		FlowLayout* flow, Button* filterButton, Button* hostButton, Button* watch, ToolTip* toolTip, ImageIcon* theme, GuiColorManager* color,
		LanguageManager* language, VScrollBar* vscroll, SpriteManager* sprite, GuiFontManager* font, BackgroundManager* bgMan,
		ThemeManager* themeMan,
		agui::Gui* gui)
		: m_fontMan(font),m_spriteMan(sprite),m_bgSprite(sprite->getImage("lobby.bg")),
		m_bgData(*(bgMan->getBackgroundAt(0))), m_numTablesPerRow(0),
		m_heightRatioForOne(1.0), m_lastFocusedTable(NULL), m_ignoreScroll(false),
		m_filterButton(filterButton), m_colorMan(color), m_hostButton(hostButton),
		m_factory(factory),m_hostButtonPressed(false),m_lockTableNo(-1),m_gui(gui),
		m_cache(cache),m_themeMan(themeMan),m_glowingTable(-1),m_wantWatchTable(-1),
        m_tooltip(toolTip),m_themeIcon(theme)
	{
		gui->addMousePreviewListener(this);
		setMargins(1,1,4,3);
		m_vScroll = vscroll;
		setVScrollBar(m_vScroll);
		m_flow = flow;
		m_flow->setUpdateOnChildAddRemove(false);
		m_flow->setHandleChildlogic(true);
		m_flow->setFilterVisibility(false);
		m_flow->setUpdateOnChildResize(false);
		m_flow->setUpdateOnChildRelocate(false);
        
        m_watchButton = watch;
        m_gui->add(m_watchButton);
        m_watchButton->setVisibility(false);
        m_watchButton->setText(m_factory->getLanguageManager()->getElement("watch.this.table"));
        m_watchButton->addMouseListener(this);

		m_pointsText = language->getElement("points");

		for(int i = 0; i < 3; i++)
		{
			m_bots[i].setAvatar(i + 1);
			m_bots[i].setUserName(language->getElement("cyborg"));
		}


		std::string open = language->getElement("filter.button");
		std::string close = language->getElement("filter.button.close");

        agui::Font* filterFont = Platf::isRetina() ? font->getFont(2.0f * 0.8f) : font->getFont(0.8f);
		
		int openLength = filterFont->getTextWidth(open);
		int closeLength = filterFont->getTextWidth(close);

        if(Platf::isRetina())
        m_filterButton->setFont(font->getFont(2.0f));
        
		m_filterButton->setText(openLength > closeLength ? open : close);
		m_filterButton->setContentResizeScale(0.9f,1.0f);
		m_filterButton->resizeToContents();
		m_filterButton->setFont(filterFont);
		m_filterButton->setToggleButton(true);
		m_filterButton->addActionListener(this);
		m_filterButton->setText(open);
		
		m_hostButton->setContentResizeScale(0.9f,1.0f);
		setHostButtonState(true);
		m_hostButton->addActionListener(this);

		addPrivateChild(m_flow);
		addPrivateChild(m_filterButton);
		addPrivateChild(m_hostButton);
		addPrivateChild(m_vScroll);
		m_vScroll->addVScrollBarListener(this);
        if(!Platf::isMobile())
		m_vScroll->setMouseWheelAmount(25);
        else
        m_vScroll->setMouseWheelAmount(1);
		m_flow->addMouseListener(this);

		m_flow->setResizeToParent(false);
		m_flow->setHorizontallyCentered(true);
		m_flow->setAlignLastRow(true);
		updateScrollBars();

		setTableScale(2);

		//set table M and S
		std::string mirror = language->getElement("filter.mirrorbid");
		std::string suicide = language->getElement("filter.suicidebid");

		if(mirror.length() > 0)
		{
			m_mirrorShort = mirror[0];
		}

		if(suicide.length() > 0)
		{
			m_suicideShort = suicide[0];
		}

		//set social and rated text
		m_ratedText = language->getElement("filter.rated");
		m_socialText = language->getElement("filter.social");

		m_feeText = language->getElement("fee");
		m_noFeeText = language->getElement("no.fee");

		m_pass2Text = language->getElement("short.pass2");
		m_partnershipText = language->getElement("short.partnership");
		m_individualText = language->getElement("short.individual");
		m_cutthroatText = language->getElement("short.cutthroat");

		m_magnifyButton = factory->createWhiteButton("magnify");
        if(Platf::isRetina())
        {
            m_magnifyButton->setScaleIcon(true);
            m_magnifyButton->setImageScale(0.4f);
        }
		m_magnifyButton->setToggleButton(true);
		m_magnifyButton->addActionListener(this);
        if(!Platf::isRetina())
		m_magnifyButton->setSize(35,35);
        else
         m_magnifyButton->setSize(35 * 2.5f,35 * 2.5f);
		addPrivateChild(m_magnifyButton);
		m_magnifyButton->setLocation(5,5);

		m_magnifySlider = factory->createSlider();
		m_magnifySlider->setOrientation(agui::VERTICAL);
        if(!Platf::isRetina())
		m_magnifySlider->setSize(m_magnifySlider->getWidth(),100);
        else
        m_magnifySlider->setSize(m_magnifySlider->getWidth(),220);
		m_magnifySlider->setVisibility(false);
		m_magnifySlider->addSliderListener(this);
		m_magnifySlider->setMaxValue(2);
		m_magnifySlider->setMinValue(1);
		m_magnifySlider->setValue(1);
		m_magnifySlider->setStepLength(1);
		m_magnifySlider->setFocusable(true);
		gui->add(m_magnifySlider);
        m_tooltip->addMouseListener(this);

		for(int i = 0; i < 3; i++)
		{
			m_bots[i].setIsBot(true);
		}
        
        m_gui->add(m_themeIcon);
        m_themeIcon->setVisibility(false);
        m_themeIcon->addMouseListener(this);
	}

	LobbyTableManager::~LobbyTableManager(void)
	{
		m_vScroll->removeVScrollBarListener(this);
		setVScrollBar(NULL);
		m_flow->clear();
		m_magnifySlider->removeSliderListener(this);
		m_gui->removeMousePreviewListener(this);
        m_watchButton->removeMouseListener(this);

	}

    void LobbyTableManager::showToolTip(agui::Widget* widget, const std::string& text, int x, int y) {
        int w = m_gui->getMaxToolTipWidth();
        
        if(m_tooltip && text.length() > 0)
        {
            if(m_tooltip->getParent() == NULL)
            {
                m_gui->add(m_tooltip);
            }
            m_tooltip->bringToFront();
            m_tooltip->showToolTip(
                                 text,
                                 w,
                                 x + m_tooltip->getPreferredOffset().getX(),
                                 y + m_tooltip->getPreferredOffset().getY(),
                                 widget);
        }
    }
    
    void LobbyTableManager::hideToolTip() {
        m_tooltip->hideToolTip();
    }
    
	void LobbyTableManager::render( GraphicsContext* g )
	{
		agui::Rectangle absRect = getAbsoluteRectangle();
		agui::Rectangle parentAbsRect = m_flow->getAbsoluteRectangle();
		agui::Rectangle r = absRect;
		int cx, cy, cw, ch;
		g->getClippingRect(cx,cy,cw,ch);
		g->setClippingRect(r.getX(),r.getY(),r.getWidth(),r.getHeight());

		renderBG(g,absRect,parentAbsRect);

		g->holdDrawing();
		for(size_t i = 0; i < m_activeTables.size(); ++i)
		{
			bool visible = isTableVisible(m_activeTables[i],absRect,parentAbsRect);
			m_activeTables[i]->setVisibility(visible);

			if(m_activeTables[i]->isVisible())
			m_activeTables[i]->renderChairs(g);
		}

		for(size_t i = 0; i < m_activeTables.size(); ++i)
		{
			if(m_activeTables[i]->isVisible())
				m_activeTables[i]->renderTableShadow(g);
		}

		for(size_t i = 0; i < m_activeTables.size(); ++i)
		{
			bool visible = isTableVisible(m_activeTables[i],absRect,parentAbsRect);

			if(m_activeTables[i]->isVisible())
				m_activeTables[i]->renderChairPlayers(g);
			m_activeTables[i]->update();
		}

		for(size_t i = 0; i < m_activeTables.size(); ++i)
		{
			if(m_activeTables[i]->isVisible())
				m_activeTables[i]->renderTableShadowWeak(g);
		}


		for(size_t i = 0; i < m_activeTables.size(); ++i)
		{
			if(m_activeTables[i]->isVisible())
				m_activeTables[i]->renderTable(g);
		}

		for(size_t i = 0; i < m_activeTables.size(); ++i)
		{
			bool visible = isTableVisible(m_activeTables[i],absRect,parentAbsRect);

			if(m_activeTables[i]->isVisible())
				m_activeTables[i]->renderChairTexts(g);
		}

		g->unholdDrawing();	
		g->setClippingRect(cx,cy,cw,ch);
	}

	void LobbyTableManager::createTable( int tableId )
	{
		if(getGui())
		{
			getGui()->toggleWidgetLocationChanged(false);
		}
		ClientPlayer* self = getPlayerByName("");
		m_tables[tableId] = LobbyTable(m_cache,m_colorMan,m_fontMan,m_spriteMan,tableId,self);
		LobbyTable* t = &m_tables[tableId];

		t->setMirrorSuicideNames(m_mirrorShort,m_suicideShort);
		t->setRatedTexts(m_ratedText,m_socialText);
		t->setFeeText(m_feeText);
		t->setNoFeeText(m_noFeeText);
		t->setPointsText(m_pointsText);
		t->setGameTypeNames(m_pass2Text,m_partnershipText,m_individualText,m_cutthroatText);
		t->setWatchSymbol(m_factory->getLanguageManager()->getElement("table.watch"));
		t->addChairActionListeners(this);
		t->addMouseListener(this);
		t->addChairMouseListeners(this);
		t->getTable()->addMouseListener(this);
		t->rescale(1.0f, m_numTablesPerRow);
		m_createdTables.push_back(t);
		
		//m_flow->add(t);
		t->addChildrentToSelf();
		if(getGui())
		{
			getGui()->toggleWidgetLocationChanged(true);
		}
	}

	void LobbyTableManager::destroyTable( int tableId )
	{
		resizeFlow();
		updateScrollBars();
	}

	void LobbyTableManager::actionPerformed( const agui::ActionEvent &evt )
	{
		if(evt.getSource() == m_filterButton)
		{
			std::string open = m_factory->getLanguageManager()->getElement("filter.button");
			std::string close = m_factory->getLanguageManager()->getElement("filter.button.close");

			m_filterButton->setText(m_filterButton->isToggled() ? close : open);

			DISPATCH_LOBBY_EVENT
			{
				(*it)->toggleFilterMenu(m_filterButton->isToggled());
			}

			DISPATCH_SCENE_EVENT
				(*it)->setBoolSetting("toggle.filter.menu.on",m_filterButton->isToggled());
		}
		else if(evt.getSource() == m_magnifyButton)
		{
			if(m_magnifyButton->isToggled())
			{
				showMagnifySlider();
			}
			else
			{
				hideMagnifySlider();
			}

		}
		else if(evt.getSource() == m_hostButton && !m_hostButtonPressed)
		{
			//dummy value for now
			DISPATCH_LOBBY_EVENT
				(*it)->requestHostTable(-1,0);
			m_hostButtonPressed = true;
		}
		else if(evt.getSource() == m_hostButton && m_hostButtonPressed)
		{
			ClientPlayer* self = getPlayerByName("");
			if(self && self->getSeat() != -1 && self->getTable() != -1)
			{
				LobbyTable* t = NULL;
				for(int k = 0; k < m_createdTables.size(); k++)
				{
					if(m_createdTables[k]->getTableId() == self->getTable())
					{
						t = m_createdTables[k];
					}
				}

				leaveRequest(t);
			}
		}
		else
		{
			for (size_t i = 0; i < m_createdTables.size(); ++i)
			{
				if(evt.getSource() == m_createdTables[i]->getTable())
				{
                   if(m_createdTables[i]->getTable()->getInfoText().length() > 0)
                    {
                        agui::Point absPos = m_createdTables[i]->getTable()->getAbsolutePosition();
                        
                        showToolTip(m_createdTables[i]->getTable(), m_createdTables[i]->getTable()->getInfoText(), absPos.getX(), absPos.getY());
                        m_tooltip->setLocation(absPos.getX() + (m_createdTables[i]->getTable()->getWidth() / 2) - (m_tooltip->getWidth() / 2),m_tooltip->getLocation().getY());
                        int theme = m_createdTables[i]->getGameInfo().getThemeID();
                        Sprite* themeImg = m_themeMan->getTheme(theme)->m_thumbnail;
                        m_themeImg.setBitmap(themeImg->getBitmap());
                        m_themeIcon->setScaleToFit(true);
                        m_themeIcon->setIcon(&m_themeImg);
                        m_themeIcon->setVisibility(true);
                        float aspect = (float)themeImg->getHeight() / (float)themeImg->getWidth();
                        m_themeIcon->setSize(m_tooltip->getWidth(), m_tooltip->getWidth() * aspect);
                        m_wantWatchTable = -1;
                        
                        m_themeIcon->setLocation(m_tooltip->getLocation().getX(),m_tooltip->getLocation().getY() + m_tooltip->getHeight());
                        
                        if(m_createdTables[i]->getTable()->canWatch() && m_createdTables[i]->isStarted())
                        {
                            m_wantWatchTable = i;
                            
                            agui::Point pos = m_themeIcon->getAbsolutePosition();
                            pos.setY(pos.getY() + m_themeIcon->getHeight());
                            m_watchButton->resizeToContents();
                            m_watchButton->setSize(m_tooltip->getWidth(),m_watchButton->getHeight() * 2.0f);
                            m_watchButton->setLocation(pos);
                            m_watchButton->bringToFront();
                            m_watchButton->setVisibility(true);
                        }
                        
                        if(m_watchButton->isVisible())
                        {
                            if(m_watchButton->getLocation().getY() + m_watchButton->getHeight() > m_gui->getTop()->getHeight()) {
                                
                                int amount = m_watchButton->getLocation().getY() + m_watchButton->getHeight() - m_gui->getTop()->getHeight();
                                m_tooltip->setLocation(m_tooltip->getLocation().getX(), m_tooltip->getLocation().getY() - amount);
                                m_themeIcon->setLocation(m_themeIcon->getLocation().getX(), m_themeIcon->getLocation().getY() - amount);
                                m_watchButton->setLocation(m_watchButton->getLocation().getX(), m_watchButton->getLocation().getY() - amount);
                            }
                        }
                        else
                        {
                            if(m_themeIcon->getLocation().getY() + m_themeIcon->getHeight() > m_gui->getTop()->getHeight()) {
                                int amount = m_themeIcon->getLocation().getY() + m_themeIcon->getHeight() - m_gui->getTop()->getHeight();
                                m_tooltip->setLocation(m_tooltip->getLocation().getX(), m_tooltip->getLocation().getY() - amount);
                                m_themeIcon->setLocation(m_themeIcon->getLocation().getX(), m_themeIcon->getLocation().getY() - amount);
                            }
                        }
                        
                    }
					break;
				}
				for(int j = 0; j < m_createdTables[i]->getNumChairs(); ++j)
				{
					LobbyChairComponent* c = m_createdTables[i]->getChairAt(j);

					//request to host if it is empty
					ClientPlayer* self = getPlayerByName("");
					bool isMySeat = m_createdTables[i]->getTableId() == self->getTable() && j == self->getSeat();
					if(c == evt.getSource() && self && c->isOccupied() && !isMySeat)
					{
						//show context in other event
						break;
					}
					else if(c == evt.getSource() && m_createdTables[i]->isEmpty())
					{
						if(self && self->getSeat() == j && 
							self->getTable() == m_createdTables[i]->getTableId())
						{
							/*
							DISPATCH_LOBBY_EVENT
								(*it)->requestLeaveTable();
								*/
							//now this shows a context menu
						}
						else
						{
							DISPATCH_LOBBY_EVENT
								(*it)->requestHostTable(m_createdTables[i]->getTableId(),j);

						}
						break;
					}
					//just ask to sit here, but stand up first
					else if(c == evt.getSource())
					{
						ClientPlayer* self = getPlayerByName("");
						if(self && self->getSeat() == j && self->getTable()
							== m_createdTables[i]->getTableId())
						{
							leaveRequest(m_createdTables[i]);
						}
						else
						{
							if(m_createdTables[i]->getChairAt(j)->isSubSeat())
							{
								DISPATCH_LOBBY_EVENT
									(*it)->requestSubSeat(m_createdTables[i],j);
							}
							else
							{
								DISPATCH_LOBBY_EVENT
									(*it)->requestSitAtTable(m_createdTables[i]->getTableId(),j);
							}
						}

						break;
					}
				}
			}

			//reenable widget location changes
			if(getGui())
			{
				getGui()->toggleWidgetLocationChanged(true);
			}
		}
		
	}

	int LobbyTableManager::getContentHeight() const
	{
		return m_flow->getContentsHeight() + m_filterButton->getHeight();
	}

	int LobbyTableManager::getMaxWidth() const
	{
		return getInnerWidth();
	}

	int LobbyTableManager::getMaxHeight() const
	{
		return getInnerHeight();
	}

	void LobbyTableManager::valueChanged( agui::VScrollBar* source,int val )
	{
		if(getGui())
		{
			getGui()->toggleWidgetLocationChanged(false);
		}
		m_flow->setLocation(0,-val);
		if(getGui())
		{
			getGui()->toggleWidgetLocationChanged(true);
			getGui()->_widgetLocationChanged();
		}

		if(!m_ignoreScroll)
		m_lastFocusedTable = NULL;
	}

	void LobbyTableManager::valueChanged( agui::Slider* source,int val )
	{
		if(source == m_magnifySlider)
		{
			setTableScale(val + 1);

			DISPATCH_SCENE_EVENT
				(*it)->setNumericSetting("lobby.tables",val);
		}
	}

	void LobbyTableManager::setSize( int width, int height )
	{
		agui::Widget::setSize(width,height);
	}

	void LobbyTableManager::setSize( const agui::Dimension& size )
	{
		if(!m_lastFocusedTable)
		{
			m_lastFocusedTable = getTopTable();
		}

		m_ignoreScroll = true;

		LobbyTable* tableToUse = m_lastFocusedTable;
		
		agui::Widget::setSize(size);
		recomputeScale();
		rescaleTables();
		resizeFlow();
		updateScrollBars();

		if(m_numTablesPerRow == 1)
		{
			recomputeScale();
			rescaleTables();
			resizeFlow();
			updateScrollBars();
		}

		m_lastFocusedTable = tableToUse;
		focusOnTable(m_lastFocusedTable,false);
		m_ignoreScroll = false;

		m_filterButton->setLocation(0,getInnerHeight() - 
			m_filterButton->getHeight() + 3);

		m_hostButton->setLocation(m_filterButton->getWidth(),getInnerHeight() - 
			m_hostButton->getHeight() + 3);

		if(getGui())
			getGui()->_widgetLocationChanged();
	}

	void LobbyTableManager::resizeFlow()
	{
			int extra = isVScrollNeeded() ?  m_vScroll->getWidth() : 0;
			m_flow->setSize(getInnerWidth() - extra,
				getContentHeight()
				);

			extra = isVScrollNeeded() ?  m_vScroll->getWidth() : 0;

			m_flow->setSize(getInnerWidth() - extra,
				getContentHeight()
				);
	}

	void LobbyTableManager::mouseWheelDownCB( agui::MouseEvent &mouseEvent )
	{
        if(getGui()->getWidgetUnderMouse() != m_watchButton)
        {
            hideToolTip();
            m_watchButton->setVisibility(false);
            m_themeIcon->setVisibility(false);
            m_wantWatchTable = -1;
        }
        else if(getGui()->getWidgetUnderMouse() == m_watchButton) {
            return;
        }
    
		if(m_gui->getWidgetUnderMouse() == m_magnifySlider ||
			m_gui->getWidgetUnderMouse() == m_magnifySlider->getMarker() || mouseEvent.getSourceWidget() == NULL)
			return;
		m_vScroll->wheelScrollDown(mouseEvent.getMouseWheelChange());
	}

	void LobbyTableManager::mouseWheelUpCB( agui::MouseEvent &mouseEvent )
	{
        if(getGui()->getWidgetUnderMouse() != m_watchButton)
        {
            hideToolTip();
            m_watchButton->setVisibility(false);
            m_themeIcon->setVisibility(false);
            m_wantWatchTable = -1;
        }
        else if(getGui()->getWidgetUnderMouse() == m_watchButton) {
            return;
        }
        
		if(m_gui->getWidgetUnderMouse() == m_magnifySlider ||
			m_gui->getWidgetUnderMouse() == m_magnifySlider->getMarker() || mouseEvent.getSourceWidget() == NULL)
			return;
		m_vScroll->wheelScrollUp(mouseEvent.getMouseWheelChange());
	}

	void LobbyTableManager::mouseWheelDown( agui::MouseEvent &mouseEvent )
	{
		if(m_gui->getWidgetUnderMouse() == m_magnifySlider ||
			m_gui->getWidgetUnderMouse() == m_magnifySlider->getMarker() || mouseEvent.getSourceWidget() == NULL)
			return;
		m_vScroll->wheelScrollDown(mouseEvent.getMouseWheelChange());
	}

	void LobbyTableManager::mouseWheelUp( agui::MouseEvent &mouseEvent )
	{
		if(m_gui->getWidgetUnderMouse() == m_magnifySlider ||
			m_gui->getWidgetUnderMouse() == m_magnifySlider->getMarker() || mouseEvent.getSourceWidget() == NULL)
			return;
		m_vScroll->wheelScrollUp(mouseEvent.getMouseWheelChange());
	}

	void LobbyTableManager::rescaleTables()
	{
		if(getGui())
		{
			getGui()->toggleWidgetLocationChanged(false);
		}
		float s = getTableScale();
		for(size_t i = 0; i < m_activeTables.size(); ++i)
		{
			m_activeTables[i]->rescale(s, m_numTablesPerRow);
		}

		if(getGui())
			getGui()->toggleWidgetLocationChanged(true);

		if(getGui())
			getGui()->_widgetLocationChanged();

        if(!Platf::isMobile())
		m_vScroll->setMouseWheelAmount(25 * s);
	}

	double LobbyTableManager::getTableScale() const
	{
		return getScale();
	}


	bool LobbyTableManager::isTableVisible(
		LobbyTable* table,const agui::Rectangle& absRect, 
		const agui::Rectangle& absParentRect ) const
	{
		agui::Point p1 = agui::Point(
			absParentRect.getX() + table->getLocation().getX(),
			absParentRect.getY() + table->getLocation().getY());

		agui::Point p2 = agui::Point(
			p1.getX(),
			p1.getY() + table->getHeight());

		return absRect.pointInside(p1) ||
			absRect.pointInside(p2);
	}

	void LobbyTableManager::endAddTables()
	{
		m_flow->clear();
		for(size_t i = 0; i < m_createdTables.size(); ++i)
		{
			m_flow->add(m_createdTables[i]);
		}
		filterTables();
		recomputeScale();
		rescaleTables();
		resizeFlow();
		updateScrollBars();
	}

	void LobbyTableManager::renderBG( GraphicsContext* g, agui::Rectangle& absRect, agui::Rectangle& childRect )
	{
		double scale = 0.25;
		float w = 846 * getTableScale() * scale;
		double h = (double)846 * getTableScale() * scale;

		int numX = ceil(absRect.getWidth() / (float)w) + 2;
		int numY = ceil(absRect.getHeight() / (float)h) + 2;

		float offsetX = 0;
		float offsetY = 0 - h;

		if(m_activeTables.size() > 0)
		{
			float offsetX = m_activeTables[0]->getAbsolutePosition().getX();
			float offsetY = m_activeTables[0]->getLocation().getY() - h;
		}
		double startY = childRect.getY() - 1;

		int numAttempts = 0;
		while(startY + h < absRect.getY() && numAttempts < 1000)
		{
			startY += h;
			numAttempts++;
		}
		numAttempts = 0;
		while(offsetX + w > absRect.getX() && numAttempts < 1000)
		{
			offsetX -= w;
			numAttempts++;
		}

		g->holdDrawing();
		for(int i = 0; i < numX; ++i)
		{
			for(int j = 0; j < numY; ++j)
			{
				g->drawScaledSprite(m_bgSprite,0,0,m_bgSprite->getWidth(),m_bgSprite->getHeight(),
					(i * w) + (offsetX),absRect.getY() + (j * h) + startY - h,w + 0.5f,h + 0.5f,0);
			}
		}
		g->unholdDrawing();
	}

	void LobbyTableManager::changeTableScale( double scale )
	{
		m_magnifySlider->setValue((int)scale - 1);
	}

	void LobbyTableManager::setTableScale( double scale )
	{

		if(!m_lastFocusedTable)
		{
			m_lastFocusedTable = getTopTable();
		}

		//dont want to NULL last focused
		m_ignoreScroll = true;

		LobbyTable* tableToUse = m_lastFocusedTable;
		m_numTablesPerRow = scale;
		m_flow->setMaxOnRow(m_numTablesPerRow);

		agui::Gui* gotGui = getGui();
		recomputeScale();
		rescaleTables();
		resizeFlow();

		if(gotGui)
		{
			gotGui->toggleWidgetLocationChanged(false);
		}
		updateScrollBars();

		if(gotGui)
		{
			gotGui->toggleWidgetLocationChanged(true);
		}

		if(gotGui)
		{
			gotGui->_widgetLocationChanged();
		}

		

		m_lastFocusedTable = tableToUse;
			focusOnTable(m_lastFocusedTable,false);
			m_ignoreScroll = false;
	}

	void LobbyTableManager::recomputeScale()
	{
		if(m_numTablesPerRow == 0)
		{
			return;
		}

		int width = getInnerWidth() - m_vScroll->getWidth();
		float w = 0.9f;
		width -= (m_numTablesPerRow - 1) * m_flow->getHorizontalSpacing();
		width /= m_numTablesPerRow;

		if(m_numTablesPerRow == 1)
		{
			width = getInnerHeight() * m_heightRatioForOne;

			if(width > m_flow->getInnerWidth())
			{
				width = getInnerWidth() - m_vScroll->getWidth();

				width -= (m_numTablesPerRow - 1) * m_flow->getHorizontalSpacing();
				width /= m_numTablesPerRow;
				width *= 0.9;
			}
		}
		else
		{
			width *= 0.9;
		}

			setScale(width / 200.0);
	}

	LobbyTable* LobbyTableManager::getTableByID( int tableID )
	{
		for(size_t i = 0; i < m_createdTables.size(); ++i)
		{
			if(m_createdTables[i]->getTableId() == tableID)
			{
				return m_createdTables[i];
			}
		}

		return NULL;
	}

	void LobbyTableManager::focusOnTable( LobbyTable* table, bool glow )
	{
		if(table)
		{
			m_vScroll->setValue(table->getLocation().getY());
			if(glow)
			setGlowTable(table->getTableId());
		}
	}

	void LobbyTableManager::focusOnTable( int tableID, bool glow )
	{
		LobbyTable* t = getTableByID(tableID);

		if(t)
		{
			focusOnTable(t, glow);
		}
	}

	LobbyTable* LobbyTableManager::getTopTable()
	{
		agui::Rectangle absRect = getAbsoluteRectangle();
		for(agui::WidgetArray::iterator it = m_flow->getChildBegin(); it != m_flow->getChildEnd(); ++it)
		{
			Widget* w = (*it);
			agui::Rectangle childAbsRect = w->getAbsoluteRectangle();

			if(absRect.pointInside(childAbsRect.getLeftTop()) || absRect.pointInside(childAbsRect.getBottomLeft()))
			{
				int amountInside = childAbsRect.getBottom() - absRect.getTop();
				float percentInside = (float)amountInside / w->getHeight();

				if(percentInside > 0.35f)
				{for(size_t i = 0; i < m_activeTables.size(); ++i)
					{
						if(m_activeTables[i] == w)
						{
							return m_activeTables[i];
						}
					}
				}
				
			}
		}

		return NULL;
	}

	void LobbyTableManager::setTableInfo( int tableId,
		const SpadesGameInfo& info, bool isSet /* = true */ )
	{
		LobbyTable* table = getTableByID(tableId);

		if(table)
		{
			table->tableBeingSetup(false);

			if(isSet)
			{
				table->setGameInfo(info);
				table->getTable()->setInfoText(generateTableTooltip(info,tableId));
			}
			else
			{
				table->setGameInfoSet(isSet);
				if(!isSet)
				{
					//cannot be a sub seat if not active
					for(int i = 0; i < table->getNumChairs(); ++i)
					{
						table->getChairAt(i)->setAsSubSeat(false);
					}

					table->getTable()->setToolTipText("");
                    table->getTable()->setInfoText("");
				}
			}

			ClientPlayer* p = getPlayerByName("");
			//unlock, no longer setting up
			if(!p->isSitting())
			{
				table->lock(false);
			}
			else
			{
				table->setCanWatch(false);
				table->showWatchText(table->isWatchable());
			}

			filterTables();

			if(!table->isGameInfoSet() || table->isBeingSetup())
			{
				table->setCanWatch(false);
				table->showWatchText(false);
				table->getTable()->setToolTipText("");
                table->getTable()->setInfoText("");
			}
		}
	}

	void LobbyTableManager::applyTableFilter( const std::vector<TableFilterEnum>& filters )
	{
		m_filters = filters;
		filterTables();
	}
    
    void LobbyTableManager::setAvatarOpacity(float opacity) {
        for(size_t i = 0; i < m_activeTables.size(); ++i)
        {
            for(int j = 0; j < m_activeTables[i]->getNumChairs(); j++)
                m_activeTables[i]->getChairAt(j)->setAvatarOpacity(opacity);
        }
    }

	void LobbyTableManager::filterTables()
	{
		std::vector<TableFilterEnum> primaryFilters;
		std::vector<TableFilterEnum> subFilters;
		std::vector<TableFilterEnum> ratedFilters;
		std::vector<TableFilterEnum> endFilters;
		std::vector<TableFilterEnum> emptyFilters;
		std::vector<TableFilterEnum> fullFilters;

		int playerTable = -1;
		ClientPlayer* p = getPlayerByName("");
		playerTable = p->getTable();

		endFilters.push_back(SCORE_TFILTER);
		for(size_t i = 0; i < m_filters.size(); ++i)
		{
			if(m_filters[i] == NORMAL_BID_TFILTER ||
				m_filters[i] == MIRROR_BID_TFILTER ||
				m_filters[i] == SUICIDE_BID_TFILTER)
			{
				subFilters.push_back(m_filters[i]);
			}
			else if(m_filters[i] == RATED_TABLE_TFILTER ||
				m_filters[i] == SOCIAL_TABLE_TFILTER)
			{
				ratedFilters.push_back(m_filters[i]);
			}
			else if(m_filters[i] == HIDE_EMPTY_TFILTER)
			{
				emptyFilters.push_back(m_filters[i]);
			}
			else if(m_filters[i] == HIDE_FULL_TFILTER)
			{
				fullFilters.push_back(m_filters[i]);
			}
			/*
			else if(m_filters[i] == TIME_TFILTER ||
				m_filters[i] == SCORE_TFILTER ||
				m_filters[i] == HAND_TFILTER)
			{
				endFilters.push_back(m_filters[i]);
			}
			*/
			else
			{
				primaryFilters.push_back(m_filters[i]);
			}
		}

		if(getGui())
			getGui()->toggleWidgetLocationChanged(false);
		m_activeTables.clear();
		for(size_t i = 0; i < m_createdTables.size(); ++i)
		{
			bool passedA = false;
			bool passedB = false;
			bool passedC = false;
			bool passedD = true;
			bool passedE = false;
			bool passedF = true;
			for(size_t j = 0; j < primaryFilters.size(); ++j)
			{
				if(isTableFilterCriteria(m_createdTables[i],primaryFilters[j]))
				{
					passedA = true;
					break;
				}
			}

			for(size_t j = 0; j < subFilters.size(); ++j)
			{
				if(isTableFilterCriteria(m_createdTables[i],subFilters[j]))
				{
					passedB = true;
					break;
				}
			}

			for(size_t j = 0; j < ratedFilters.size(); ++j)
			{
				if(isTableFilterCriteria(m_createdTables[i],ratedFilters[j]))
				{
					passedC = true;
					break;
				}
			}

			for(size_t j = 0; j < emptyFilters.size(); ++j)
			{
				if(!isTableFilterCriteria(m_createdTables[i],emptyFilters[j]))
				{
					passedD = false;
					break;
				}
			}

			for(size_t j = 0; j < endFilters.size(); ++j)
			{
				if(isTableFilterCriteria(m_createdTables[i],endFilters[j]))
				{
					passedE = true;
					break;
				}
			}

			for(size_t j = 0; j < fullFilters.size(); ++j)
			{
				if(isTableFilterCriteria(m_createdTables[i],fullFilters[j]))
				{
					passedF = false;
					break;
				}
			}

			m_activeTables.push_back(m_createdTables[i]);
			//players table stays lit regardless
			if((passedA && passedB && passedC && passedD && passedE && passedF) || m_createdTables[i]->getTableId() == playerTable)
			{
				m_createdTables[i]->setEnabled(true);
			}
			else
			{
				m_createdTables[i]->setEnabled(false);
			}
		}


			if(getGui())
			{
				getGui()->toggleWidgetLocationChanged(true);
				getGui()->_widgetLocationChanged();
			}

	}

	bool LobbyTableManager::isTableFilterCriteria( 
		LobbyTable* table, TableFilterEnum filter )
	{
		if(filter == HIDE_EMPTY_TFILTER && !table->isGameInfoSet())
		{
			return false;
		}
		if(filter == HIDE_FULL_TFILTER && !table->isGameInfoSet())
		{
			return false;
		}
		else if(!table->isGameInfoSet())
		{
			return true;
		}

		switch(filter)
		{
		case ALL_TABLES_TFILTER:
			return true;
		case CUT_THROAT_TFILTER:
			return table->getGameInfo().isCutthroat();
		case HIDE_FULL_TFILTER:
			return table->getNumOccupants() > 0 && table->getNumOccupants() == table->getGameInfo().getNumPlayers() && table->isGameInfoSet();
		case INDIVIDUAL_TFILTER:
			return table->getGameInfo().isIndividual();
		case MIRROR_BID_TFILTER:
			return table->getGameInfo().isMirror();
		case NORMAL_BID_TFILTER:
			return !table->getGameInfo().isMirror() && 
				!table->getGameInfo().isSuicide();
		case PASS_TWO_TFILTER:
			return table->getGameInfo().isPassTwo();
		case REGULAR_TFILTER:
			return table->getGameInfo().isPartnership() && 
				!table->getGameInfo().isPassTwo();
		case SUICIDE_BID_TFILTER:
			return table->getGameInfo().isSuicide();
		case RATED_TABLE_TFILTER:
			return table->getGameInfo().isRated();
		case SOCIAL_TABLE_TFILTER:
			return table->getGameInfo().isSocial();
		case SCORE_TFILTER:
			return table->getGameInfo().isScoreEnded();
		case TIME_TFILTER:
			return table->getGameInfo().isTimeEnded();
		case HAND_TFILTER:
			return table->getGameInfo().isHandEnded();
		}

		return true;
	}

	void LobbyTableManager::sitPlayerAtTable( int tableNumber, int seatNumber, const std::string& playerName )
	{
		LobbyTable* t = getTableByID(tableNumber);

		ClientPlayer* c = getPlayerByName(playerName);

		if(!c || !t)
		{
			Log::write("Warning, table or player is null");
		}

		if(t && c)
		{
			t->occupyChair(seatNumber,c);

			if(t->getGameInfo().isBotGame())
			{
				int botIndex = 0;
				for(int i = 0; i < t->getGameInfo().getNumPlayers(); i++)
				{
					if(!t->getChairAt(i)->isOccupied())
					{
						t->occupyChair(i,&m_bots[botIndex]);
						botIndex++;
					}
				}
			}
		}

		if(c)
		{
			c->setTable(tableNumber);
			c->setSeat(seatNumber);
		}

		bool selfTable = false;

		lockAllTables(false);

		if(isSelfPlayer(c))
		{
			t->getChairAt(seatNumber)->setSelfIsHere(true);
			filterTables();
			selfTable = true;
			setHostButtonState(false);
			m_hostButtonPressed = true;
		}
		else
		{
			ClientPlayer* pl = getPlayerByName("");
			selfTable = pl->getTable() == tableNumber;
		}

		ClientPlayer* me = getPlayerByName("");
		if(t)
		{
			if(me->isSitting())
			{
				t->setCanWatch(false);
				t->showWatchText(t->isWatchable());
			}
			else
			{
				t->setCanWatch(t->isWatchable());
				t->showWatchText(t->isWatchable());
			}
		}

		if(t && t->getNumOccupants() == t->getGameInfo().getNumPlayers())
		{
			t->setStarted(true);
			DISPATCH_LOBBY_EVENT
				(*it)->tableIsFull(t->getTableId());
		}
		else
		{
			t->setStarted(false);
		}

		DISPATCH_LOBBY_EVENT
			(*it)->seatOccupancyChanged(playerName,true,
			isSelfPlayer(c),selfTable,tableNumber,seatNumber,t->getNumOccupants());

		
		if(isSelfPlayer(c))
		{
			bool needsSubs = false;
			for(size_t i = 0; i < t->getNumChairs(); i++)
			{
				LobbyChairComponent* c = t->getChairAt(i);

				if(c && !c->isOccupied() && c->isSubSeat())
				{
					needsSubs = true;
					break;
				}
			}

			DISPATCH_LOBBY_EVENT
				(*it)->tableChatRequestTextChanged(needsSubs);
		}
	}

	void LobbyTableManager::playerLeftTable( int tableNumber, int seatNumber )
	{
		LobbyTable* t = getTableByID(tableNumber);
		if(!t)
		{
			return;
		}
		LobbyChairComponent* ch = t->getChairAt(seatNumber);
		if(!ch)
		{
			return;
		}

		ClientPlayer* p = ch->getOccupant();
		if(!p)
		{
			return;
		}
		if(t)
		{
			if(p)
			{
				p->standUp();
			}
			
			t->unoccupyChair(seatNumber);

			if(t->getGameInfo().isBotGame())
			{
				for(int i = 0; i < t->getGameInfo().getNumPlayers(); i++)
				{
					t->unoccupyChair(i);
				}
			}

			bool selfTable = false;

			if(isSelfPlayer(p))
			{
				lockAllTables(false);
				filterTables();
				selfTable = true;
				setHostButtonState(true);
				m_hostButtonPressed = false;
			
			}
			else
			{
				ClientPlayer* pl = getPlayerByName("");
				selfTable = pl->getTable() == tableNumber;
			}

			ClientPlayer* me = getPlayerByName("");
			if(t)
			{
				if(me->isSitting())
				{
					t->setCanWatch(false);
					t->showWatchText(t->isWatchable());
				}
				else
				{
					t->setCanWatch(t->isWatchable());
					t->showWatchText(t->isWatchable());
				}
			}


			if(t && t->isEmpty())
			{
				DISPATCH_LOBBY_EVENT
					(*it)->tableIsEmpty(t->getTableId());
                
                if(t->getTableId() == m_wantWatchTable + 1)
                {
                    hideToolTip();
                    m_watchButton->setVisibility(false);
                    m_themeIcon->setVisibility(false);
                    m_wantWatchTable = -1;
                }
			}

			
			DISPATCH_LOBBY_EVENT
				(*it)->seatOccupancyChanged(p->getUsername(),false,
				isSelfPlayer(p),selfTable,tableNumber,seatNumber,t->getNumOccupants());

			bool myT = false;
			ClientPlayer* c = getPlayerByName("");

			if(c && c->getTable() == tableNumber)
				myT = true;
			if(myT)
			{
				DISPATCH_LOBBY_EVENT
					(*it)->tableChatRequestTextChanged(false);
			}
		}
	}

	void LobbyTableManager::lockAllTables( bool locked )
	{
		bool sitting = getPlayerByName("")->isSitting();

		for(size_t i = 0; i < m_createdTables.size(); ++i)
		{
			if(m_createdTables[i]->isBeingSetup())
			{
				m_createdTables[i]->lock(true);
			}
			else if(!m_createdTables[i]->isEmpty())
			{
				m_createdTables[i]->lock(false);
			}
			else
			{
				m_createdTables[i]->lock(locked);
			}
			if(sitting)
			{
				m_createdTables[i]->setCanWatch(false);
				m_createdTables[i]->showWatchText(m_createdTables[i]->isWatchable());
			}
			else
			{
				m_createdTables[i]->setCanWatch(m_createdTables[i]->isWatchable());
				m_createdTables[i]->showWatchText(m_createdTables[i]->isWatchable());
			}
			
		}
	}

	void LobbyTableManager::createTables( int tableStart, int numTables )
	{
		int created = m_createdTables.size();
		//start tables at start not 0
		if(created == 0)
		{
			created = tableStart;	
			
			for(int i = created; i < numTables + created; ++i)
			{
				createTable(i);
			}

			endAddTables();
		}	
	}

	void LobbyTableManager::focusOnLobbyTable( int tableNo, bool glow )
	{
	//	DISPATCH_LOBBY_EVENT
	//		(*it)->changeTableScale(1.0);
		focusOnTable(tableNo,glow);
	}

	void LobbyTableManager::tableBeingSetup( int tableID )
	{
		LobbyTable* t = getTableByID(tableID);

		if(t)
		{
			t->tableBeingSetup(true);
			t->lock(true);
		}
	}

	void LobbyTableManager::seatIsSubSeat( int tableNo, int seatNo, bool subseat )
	{
		LobbyTable* t = getTableByID(tableNo);
		if(t)
		{
			LobbyChairComponent* c = t->getChairAt(seatNo);

			if(c)
			{
				c->setAsSubSeat(subseat);
				if(subseat)
				{
					t->setStarted(true);

					ClientPlayer* mii = getPlayerByName("");
					if(mii && mii->getTable() == tableNo)
					{
						bool needsSubs = false;
						for(size_t i = 0; i < t->getNumChairs(); i++)
						{
							LobbyChairComponent* c = t->getChairAt(i);

							if(c && !c->isOccupied() && c->isSubSeat())
							{
								needsSubs = true;
								break;
							}
						}

						DISPATCH_LOBBY_EVENT
							(*it)->tableChatRequestTextChanged(needsSubs);
					}
				}
			}
		}
	}

	bool LobbyTableManager::getTableByIdImpl( LobbyTable* &table, int tableId )
	{
		table = getTableByID(tableId);

		return true;
	}

	void LobbyTableManager::cancelHostRequest()
	{
		m_hostButtonPressed = false;
	}

	void LobbyTableManager::hostTable( const SpadesGameInfo& info )
	{
		if(m_hostButtonPressed)
		{
			focusOnTable(m_lockTableNo,false);
		}
	}

	void LobbyTableManager::tableLocked( int tableNo, int seatNo, bool locked )
	{
		if(locked)
		m_lockTableNo = tableNo;
	}

	std::string LobbyTableManager::generateTableTooltip( const SpadesGameInfo& info, int tableNo )
	{
		LanguageManager* lang = m_factory->getLanguageManager();

		std::string table;
		if(tableNo != -1)
		{
			table = lang->getElement("game.table") + ": " + toString(tableNo);
		}
		else
		{
			table = lang->getElement("game.single");
		}

		std::string gType;
		if(info.isPassTwo())
		{
			gType = lang->getElement("filter.pass");
		}
		else if(info.isPartnership())
		{
			gType = lang->getElement("filter.regular");
		}
		else if(info.isCutthroat())
		{
			gType = lang->getElement("filter.cutthroat");
		}
		else if(info.isIndividual())
		{
			gType = lang->getElement("filter.individual");
		}

		gType += " ";

		if(info.isMirror())
		{
			gType += lang->getElement("filter.mirrorbid");
		}
		else if(info.isSuicide())
		{
			gType += lang->getElement("filter.suicidebid");
		}

		gType = lang->getElement("game.type") + ": " + gType;

		std::string nilVal = toString(info.m_nilValue) + " " + lang->getElement("points");
		nilVal = lang->getElement("game.nil") + ": " + nilVal;

		std::string rated = info.isSocial() ?  
			lang->getElement("filter.social") : lang->getElement("filter.rated");
		rated = lang->getElement("game.game") + ": " + rated;
/*
		std::string cutDeck = info.isDeckCutEnabled() ? 
			lang->getElement("yes") : lang->getElement("no");

		cutDeck = lang->getElement("game.deckcut") + ": " + cutDeck;
 */

		std::string watch = info.isWatchingAllowed() ? 
			lang->getElement("yes") : lang->getElement("no");

		watch = lang->getElement("game.watch") + ": " + watch;

		std::string score;
		std::string scoreRT;
		if(info.isScoreEnded())
		{
			score = lang->getElement("filter.score");
			scoreRT = lang->getElement("points");
		}
		else if(info.isTimeEnded())
		{
			score = lang->getElement("filter.time");
			scoreRT = lang->getElement("minutes");
		}
		else if(info.isHandEnded())
		{
			score = lang->getElement("filter.hand");
			scoreRT = lang->getElement("hands");
		}

		std::string scoreR = toString(info.getEndValue()) + " " + scoreRT;
		scoreR = score + ": " + scoreR;

		std::string theme = lang->getElement("theme") + ": ";
		ThemeData* data = m_themeMan->getTheme(info.getThemeID());
		if(data)
			theme += lang->getElement(data->m_name);

		std::string fee = lang->getElement("fee");
		fee += ": ";
		std::stringstream ss;
		if(info.getFee() > 0)
		{
			ss << info.getFee();
			std::string sr = ss.str();
			sr = StringUtil::commaSeparate(sr);

			fee += lang->getElementWithArg("n.coins",sr);
		}
		else
		{
			ss << lang->getElement("no.fee");
			fee += ss.str();
		}

		std::string rating = lang->getElement("min.rating");
		rating += ": ";
		if(info.getMinRating() > 0)
		{
			rating += StringUtil::toString(info.getMinRating());
		}
		else
		{
			rating += lang->getElement("rating.everyone");
		}

		return table + "\n" +
			fee + "\n" +
			gType + "\n" +
			nilVal + "\n" +
			scoreR + "\n" +
			rated + "\n" +
		//	cutDeck + "\n" +
			watch + "\n" +
			rating + "\n" +
			theme;
	}

	std::string LobbyTableManager::toString( int i ) const
	{
		std::stringstream ss;
		ss << i;
		return ss.str();
	}

	void LobbyTableManager::setHostButtonState( bool hostGame )
	{
        if(!Platf::isRetina())
		m_hostButton->setFont(m_factory->getFontManager()->getDefaultFont());
        else
           m_hostButton->setFont(m_factory->getFontManager()->getFont(2.0f));
        
		if(hostGame)
		{
			m_hostButton->setText(m_factory->getLanguageManager()->getElement("host.button"));
		}
		else
		{
			m_hostButton->setText(m_factory->getLanguageManager()->getElement("leave.button"));
		}

		m_hostButton->resizeToContents();
        if(!Platf::isRetina())
		m_hostButton->setFont(m_factory->getFontManager()->getFont(0.8f));
        else
         m_hostButton->setFont(m_factory->getFontManager()->getFont(0.8f * 2.0f));
	}

	void LobbyTableManager::loadSettings( ClientShared* shared )
	{
		float tblVal = getNumericSetting("lobby.tables");
		m_magnifySlider->setValue(tblVal);
		bool filterToggled = getBoolSetting("toggle.filter.menu.on");
		m_filterButton->setToggleState(filterToggled);
		actionPerformed(agui::ActionEvent(m_filterButton));
	}

	void LobbyTableManager::showMagnifySlider()
	{
		agui::Point p = m_magnifyButton->getAbsolutePosition();
		p.setX(p.getX() + 1
			+ (m_magnifyButton->getInnerWidth() / 2)
			- (m_magnifySlider->getWidth() / 2));
		p.setY(p.getY() + m_magnifyButton->getHeight());
		m_magnifySlider->setLocation(p);
		m_magnifySlider->setVisibility(true);
		m_magnifySlider->bringToFront();
	}

	void LobbyTableManager::hideMagnifySlider()
	{
		m_magnifySlider->setVisibility(false);
		m_magnifyButton->setToggleState(false);
	}

	void LobbyTableManager::mouseMoveCB( agui::MouseEvent& evt )
	{
		//if the user is no longer within range of the slider, close it
		if(m_magnifySlider->isVisible() && m_magnifySlider->getGui()->getLockWidget() == NULL
			&& evt.getSourceWidget() == NULL)
		{
			int x1 = m_magnifySlider->getAbsolutePosition().getX() + (m_magnifySlider->getWidth() / 2);
			int y1 = m_magnifySlider->getAbsolutePosition().getY() + (m_magnifySlider->getHeight() / 2);

			int x2 = evt.getX();
			int y2 = evt.getY();

			int xdiff = x2 - x1;
			int ydiff = y2 - y1;
			float dist = sqrt((float)(xdiff * xdiff) + (float)(ydiff * ydiff));

			if(dist > 120)
			{
				hideMagnifySlider();
			}
		}
	}

    void LobbyTableManager::mouseClickCB( agui::MouseEvent &evt )
    {
        if(m_wantWatchTable != -1 && evt.getSourceWidget() == m_watchButton)
            if(m_createdTables[m_wantWatchTable]->getTable()->canWatch() && m_createdTables[m_wantWatchTable]->isStarted())
            {
                DISPATCH_LOBBY_EVENT
                (*it)->requestWatchTable(m_createdTables[m_wantWatchTable]->getTableId());
                m_wantWatchTable = -1;
                m_watchButton->setVisibility(false);
                m_themeIcon->setVisibility(false);
                hideToolTip();
            }
        
    }
    
	void LobbyTableManager::mouseDownCB( agui::MouseEvent &evt )
	{
		if(evt.getSourceWidget() == NULL && m_magnifySlider->isVisible())
		{
			if(getGui()->getWidgetUnderMouse() != m_magnifyButton
				&& getGui()->getWidgetUnderMouse() != m_magnifySlider
				&& getGui()->getWidgetUnderMouse() != m_magnifySlider->getMarker())
			{
				hideMagnifySlider();
			}
		}
        
        if(evt.getSourceWidget() == NULL && m_tooltip->isVisible())
        {
            if(getGui()->getWidgetUnderMouse() != m_watchButton && getGui()->getWidgetUnderMouse() != m_tooltip &&
               getGui()->getWidgetUnderMouse() != m_themeIcon)
            {
                hideToolTip();
                m_watchButton->setVisibility(false);
                m_themeIcon->setVisibility(false);
                m_wantWatchTable = -1;
            }
        }

		bool needExit = false;
		for (size_t i = 0; i < m_createdTables.size(); ++i)
		{
			if(needExit)
				break;

			for(int j = 0; j < m_createdTables[i]->getNumChairs(); ++j)
			{
				LobbyChairComponent* c = m_createdTables[i]->getChairAt(j);

				//request to host if it is empty
				ClientPlayer* self = getPlayerByName("");
				bool isMySeat = m_createdTables[i]->getTableId() == self->getTable() && j == self->getSeat();
				if(c == evt.getSourceWidget() && self && c->isOccupied() /*&& !isMySeat*/)
				{
					DISPATCH_LOBBY_EVENT
						(*it)->showPlayerContext(
						c->getOccupant()->getUsername(),evt.getX(),evt.getY(),AT_TABLE_RCLICK,evt.getSourceWidget());

					needExit = true;
					break;
				}
			}
		}
	}

	void LobbyTableManager::elemLogic( double t )
	{
		if(m_glowingTable > -1)
		{
			m_tables[m_glowingTable].getTable()->elemLogic(t);
			if(m_tables[m_glowingTable].getTable()->getGlowFrames() >= 
				m_tables[m_glowingTable].getTable()->getGlowInterval() * 4)
			{
				m_tables[m_glowingTable].getTable()->resetGlow();
				m_glowingTable = -1;
			}
		}
	}

	void LobbyTableManager::setGlowTable( int t )
	{
		if(m_glowingTable > -1)
		{
			if(t == m_glowingTable)
				return;

			m_tables[m_glowingTable].getTable()->resetGlow();
			m_glowingTable = -1;
		}
		m_glowingTable = t;
		m_tables[m_glowingTable].getTable()->resetGlow();
	}

	void LobbyTableManager::receivePlayerRequest( int tableNo, const std::string& playerName )
	{
		LobbyTable* t = getTableByID(tableNo);
		if(!t) {
			return;
		}
		if(!t->isGameInfoSet())
			return;
		if(t->getNumOccupants() == t->getGameInfo().getNumPlayers())
			return;

		int numSeats = 0;
		bool anySubSeats = false;

		for (size_t i = 0; i < t->getGameInfo().getNumPlayers(); i++)
		{
			LobbyChairComponent* c = t->getChairAt(i);

			if(c && !c->isOccupied())
			{
				numSeats++;

				if(c->isSubSeat())
					anySubSeats = true;
			}
		}

		if(numSeats > 0 && t->getTable()->isEnabled())
		{
			DISPATCH_LOBBY_EVENT
				(*it)->processPlayerRequest(t->getTableId(),
				playerName,t->getGameInfo(),!anySubSeats,numSeats,
				t->getTable()->getFontColor());
		}
	}

	void LobbyTableManager::leaveRequest( LobbyTable* t )
	{
		if(t)
		{
			int satCount = 0;
			for(int i = 0; i < t->getNumChairs(); i++)
			{
				LobbyChairComponent* c = t->getChairAt(i);
				if(c->isOccupied())
				{
					satCount++;
				}
			}

			bool subseat = false;
			for(int i = 0; i < t->getNumChairs(); i++)
			{
				LobbyChairComponent* c = t->getChairAt(i);
				if(c->isSubSeat())
				{
					subseat = true;
					break;
				}
			}

			if(subseat && satCount > 1)
			{
				DISPATCH_SCENE_EVENT
					(*it)->showMessageBox(m_factory->getLanguageManager()->getElement("confirm.abandon.title"),
					m_factory->getLanguageManager()->getElement("confirm.abandon.text"),
					MessageBox::MT_YES_NO,this,MSG_WANT_QUIT);
			}
			else
			{
				DISPATCH_LOBBY_EVENT
					(*it)->requestLeaveTable();
				m_hostButtonPressed = false;
			}
		}
	}

	void LobbyTableManager::messageBoxResult( MessageBox* msgbox, MessageBox::MessageResult result )
	{
		if(result == MessageBox::MR_YES && msgbox->getTag() == MSG_WANT_QUIT)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->requestLeaveTable();
			m_hostButtonPressed = false;
		}
	}

	void LobbyTableManager::requestLeaveFromContext()
	{
		ClientPlayer* self = getPlayerByName("");
		for(int i = 0; i < m_createdTables.size(); i++)
		{
			if(m_createdTables[i]->getTableId() == self->getTable())
			{
				leaveRequest(m_createdTables[i]);
				return;
			}
		}
	}


}
