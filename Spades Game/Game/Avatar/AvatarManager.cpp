#include "Game/Avatar/AvatarManager.hpp"
#include <Agui/Widgets/ToolTip/ToolTip.hpp>
#include "Game/Handler/LobbyContextHandler.hpp"
#include <sstream>
namespace cge
{
	float lerp(float t, float a, float b){
		return (1-t)*a + t*b;
	}

	AvatarManager::AvatarManager( 
		GraphicsContext* c,GuiFontManager* fontMan, agui::Gui* gui, LanguageManager* language, ToolTip* tooltip, 
		LobbyContextHandler* handler, GuiImageManager* guiImgMan, SpriteManager* spriteMan )
		: m_context(c),m_hblur(NULL),m_vblur(NULL),m_fontMan(fontMan),m_language(language),
		m_gui(gui),m_playerWatching(false),m_handler(handler),m_guiImgMan(guiImgMan),
		m_showPlayerNames(false),m_lastTime(0)
	{
		m_hblur = Shader::loadShader("hblur");
		m_vblur = Shader::loadShader("vblur");
		m_brighten = Shader::loadShader("brighten");
		for(int i = 0; i < 4; ++i)
		{
			m_avatars.push_back(Avatar());
		}

		for(int i = 0; i < 4; ++i)
		{
			m_avatars[i].setBlurShaders(m_hblur,m_vblur);
			m_avatars[i].getComposite().setShadowOffsetX(30);
			m_avatars[i].getComposite().setShadowOffsetY(30);
			m_avatars[i].setFontManager(m_fontMan);
			m_avatars[i].getText().setStrength(250.0f);
			m_avatars[i].getText().setFontColor(agui::Color(255,255,255));
			m_avatars[i].getComposite().setShadowColor(agui::Color(0.0f,0.0f,0.0f));
			m_avatars[i].setStallImage(spriteMan->getImage("time.glow.1"),
				spriteMan->getImage("time.glow.2"),spriteMan->getImage("time.glow.3"));
			gui->add(m_avatars[i].getText().getWidget());
		}

		m_avatars[3].getText().setDirection(1);
		m_avatars[1].getText().setDirection(3);
		m_avatars[3].getText().setStrength(340.0f);
		m_avatars[1].getText().setStrength(340.0f);

		m_avatars[2].getText().setDirection(0);
		m_avatars[2].getText().setStrength(250.0f);


		for(int i = 0; i < 4; ++i)
		{
			m_avatars[i].getText().getWidget()->addMouseListener(this);
		}

		m_tooltip = tooltip;
		m_gui->add(m_tooltip);
		m_tooltip->hideToolTip();

		m_tooltip->addMouseListener(this);

		m_gui->addMousePreviewListener(this);
	}

	AvatarManager::~AvatarManager(void)
	{
		delete m_hblur;
		delete m_vblur;
		delete m_brighten;

		for(int i = 0; i < 4; ++i)
		{
			m_avatars[i].getText().getWidget()->removeMouseListener(this);
		}

		m_tooltip->removeMouseListener(this);
		m_gui->removeMousePreviewListener(this);
	}

	int AvatarManager::getNumAvatars()
	{
		return (int)m_avatars.size();
	}

	bool AvatarManager::avatarExists( int avatar ) const
	{
		return avatar >= 0 && avatar <  m_avatars.size();
	}

	void AvatarManager::refresh( int avatar)
	{
		if(avatarExists(avatar))
		{
			m_avatars[avatar].getComposite().generate(m_context);
		}
	}

	void AvatarManager::refresh()
	{
		for(int i = 0; i < m_avatars.size(); ++i)
		{
			m_avatars[i].getComposite().generate(m_context);
		}
	}

	void AvatarManager::render( GraphicsContext* g )
	{
		for(int i = 0; i < m_avatars.size(); ++i)
		{
			agui::Point p = m_handler->getPopup()->getAbsolutePosition();

			if(m_gui->getWidgetUnderMouse() == m_avatars[i].getText().getWidget() ||
				(m_handler->getPopup()->isVisible() && m_handler->getPlayerPos() == i))
			{
				m_brighten->use();
			}
			m_avatars[i].render(g);
			m_brighten->stop();

			//mandatory now
		//	if(m_showPlayerNames)
			m_avatars[i].renderName(&m_aguiGraphics);

			m_avatars[i].renderTime(g);
		}
	}

	void AvatarManager::setText( int avatar, const std::string& text )
	{
		if(avatarExists(avatar))
		{
			m_avatars[avatar].setText(text);
		}
	}

	void AvatarManager::resize( int w, int h )
	{
		float femaleScalar = 1.5f;
		float femMovVec = 100 * getProportions()->getScale();
		float femMovVecX = 5 * getProportions()->getScale();
		float maleMoveVecX = 0 * getProportions()->getScale();

		m_avatars[2].setScale(getProportions()->getScale() * femaleScalar);
		m_avatars[2].setX(m_topChair->getX() - 
			( m_avatars[2].getComposite().getAvatarData().gender == AVATAR_CACHE_GENDER_MALE ? maleMoveVecX : femMovVecX));
		m_avatars[2].setY(m_topChair->getY() - femMovVec);
		m_avatars[2].setRoatation(0);
		m_avatars[2].getComposite().setAlpha(1.0f);

		m_avatars[1].setScale(getProportions()->getScale() * femaleScalar);
		m_avatars[1].setX(m_leftChair->getX() - femMovVec);
		m_avatars[1].setY(m_leftChair->getY() + ( m_avatars[1].getComposite().getAvatarData().gender == AVATAR_CACHE_GENDER_MALE ? maleMoveVecX : femMovVecX));
		m_avatars[1].setRoatation(3);
		m_avatars[1].getComposite().setAlpha(1.0f);

		m_avatars[3].setScale(getProportions()->getScale() * femaleScalar);
		m_avatars[3].setX(m_rightChair->getX() + femMovVec);
		m_avatars[3].setY(m_rightChair->getY() - ( m_avatars[3].getComposite().getAvatarData().gender == AVATAR_CACHE_GENDER_MALE ? maleMoveVecX : femMovVecX));
	    m_avatars[3].setRoatation(1);
		m_avatars[3].getComposite().setAlpha(0.9f);

		m_avatars[0].setScale(getProportions()->getScale() * femaleScalar);
		m_avatars[0].setX(m_bottomChair->getX() + ( m_avatars[0].getComposite().getAvatarData().gender == AVATAR_CACHE_GENDER_MALE ? maleMoveVecX : femMovVecX));
		m_avatars[0].setY(m_bottomChair->getY() + femMovVec);
	    m_avatars[0].setRoatation(2);
		m_avatars[0].getComposite().setAlpha(0.8f);

		float p1Scalar = 0.05f;
		float p1Shrinker = 0.1f;
		float p4Scalar = 0.05f;
		float lrOut = 0.1f;
		float shrinkLRScalar = 0.15f;
		m_avatars[0].getText().positionAndSizeWidget(
			(m_bottomChair->getX() + (m_bottomChair->getHeight() * p1Scalar) + (m_bottomChair->getHeight() * p1Shrinker))
			- (m_bottomChair->getHeight() / 2),m_bottomChair->getY() + (m_bottomChair->getWidth() * p1Scalar * 3.5f),m_bottomChair->getHeight() - 
			(2 * p1Scalar * m_bottomChair->getHeight()) - (2 * m_bottomChair->getHeight() * p1Shrinker),((m_bottomChair->getWidth() ) / 2) - (m_bottomChair->getWidth() * 2 * p1Scalar));
	
		m_avatars[1].getText().positionAndSizeWidget(m_leftChair->getX() - (m_leftChair->getWidth() / 2) - (m_leftChair->getWidth() * lrOut),m_leftChair->getY() - (m_leftChair->getHeight() / 2)  + (m_leftChair->getHeight() * shrinkLRScalar * 0.5f),m_leftChair->getWidth() / 2,m_leftChair->getHeight() - (m_leftChair->getHeight() * shrinkLRScalar));
		m_avatars[3].getText().positionAndSizeWidget((m_rightChair->getWidth() / 2) + m_rightChair->getX() - (m_rightChair->getWidth() / 2) + (m_rightChair->getWidth() * lrOut),m_rightChair->getY() - (m_rightChair->getHeight() / 2) + (m_rightChair->getHeight() * shrinkLRScalar * 0.5f),m_rightChair->getWidth() / 2,m_rightChair->getHeight()  - (m_rightChair->getHeight() * shrinkLRScalar));
		
		m_avatars[2].getText().positionAndSizeWidget(
			(m_topChair->getX() + (m_topChair->getHeight() * p4Scalar))
			- (m_topChair->getHeight() / 2),
			m_topChair->getY() - (m_topChair->getWidth() / 2) + (m_topChair->getWidth() * p4Scalar * 2),m_topChair->getHeight() - 
			(2 * p4Scalar * m_topChair->getHeight()),((m_topChair->getWidth() ) / 2) - (m_topChair->getWidth() * 2 * p4Scalar));

		
		for(int i = 0; i < getNumAvatars(); i++)
		{
			m_avatars[i].setNameFont(m_fontMan->getFont(getProportions()->getScale() * 1.55f));

			float stallW = 120.0f * getProportions()->getScale();
			m_avatars[i].setStallWidth(stallW * 1.2f);
			m_avatars[i].setStallFont(m_fontMan->getFont(stallW * 0.02f ));

		}

		float avX = (0.0f * getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterX();
		float avY = getProportions()->getScreenHeight() - (m_avatars[0].getNameFont()->getLineHeight() * 1.0f);
		m_avatars[0].setNamePos(avX,avY);

		avX = (0.0f * getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterX();
		avY = m_avatars[2].getNameFont()->getLineHeight() * 0.71f;
		m_avatars[2].setNamePos(avX,avY);

		avX = (0.57f * getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterX();
		avY = (0.2f * getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterY();
		m_avatars[3].setNamePos(avX,avY);

		avX = (-0.57f * getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterX();
		avY = (0.2f * getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterY();
		m_avatars[1].setNamePos(avX,avY);


		//time positions
		float tmConst = 0.555f;
		float tmX = (0.0f * getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterX();
		float tmY = (tmConst * getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterY();
		m_avatars[0].setStallPos(tmX,tmY);

		tmX = (0.0f * getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterX();
		tmY = (-tmConst * getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterY();
		m_avatars[2].setStallPos(tmX,tmY);

		tmX = (tmConst * getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterX();
		tmY = (0.0f * getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterY();
		m_avatars[3].setStallPos(tmX,tmY);

		tmX = (-tmConst * getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterX();
		tmY = (0.0f * getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterY();
		m_avatars[1].setStallPos(tmX,tmY);
	}

	Avatar* AvatarManager::getAvatarAt( int index )
	{
		if(index >= 0 && index < getNumAvatars())
			return &m_avatars[index];
		return NULL;
	}

	void AvatarManager::roundBegan()
	{
		for(int i = 0; i < m_avatars.size(); ++i)
		{
			m_avatars[i].getPlayerStats().m_playerBid = -1;
			m_avatars[i].getPlayerStats().m_playerPoints = 0;
		}

		updateTooltips();
	}

	void AvatarManager::roundEnded()
	{
		for(int i = 0; i < m_avatars.size(); ++i)
		{
			m_avatars[i].getPlayerStats().m_playerBid = -1;
			m_avatars[i].getPlayerStats().m_playerPoints = 0;
		}

		updateTooltips();
	}

	void AvatarManager::playerBidChanged( int player, int newBid )
	{
		m_avatars[player].getPlayerStats().m_playerBid = newBid;
		updateTooltips();
	}

	void AvatarManager::bidStateChanged( const std::vector<SpadesPointEnum> &b, const std::vector<SpadesPointEnum> &l, const std::vector<SpadesPointEnum> &t, const std::vector<SpadesPointEnum> &r, std::vector<int> bids )
	{
		for(int i = 0; i < bids.size(); ++i)
		{
			m_avatars[i].getPlayerStats().m_playerBid = bids[i];
		}

		m_avatars[0].getPlayerStats().m_playerPoints = b.size();
		m_avatars[1].getPlayerStats().m_playerPoints = l.size();
		m_avatars[2].getPlayerStats().m_playerPoints = t.size();
		m_avatars[3].getPlayerStats().m_playerPoints = r.size();

		updateTooltips();
	}

	void AvatarManager::playerMadeTrick( int player, SpadesPointEnum trickType )
	{
		m_avatars[player].getPlayerStats().m_playerPoints++;
		updateTooltips();
	}

	void AvatarManager::gameBegin()
	{
		for(int i = 0; i < m_avatars.size(); ++i)
		{
			m_avatars[i].getPlayerStats().m_playerBid = -1;
			m_avatars[i].getPlayerStats().m_playerPoints = 0;
		}
		updateTooltips();
	}

	void AvatarManager::updateTooltips()
	{
		for(int i = 0; i < m_avatars.size(); ++i)
		{
			std::stringstream ss;
			ss << m_avatars[i].getText().getText() << "\n\n";
			ss << m_language->getElement("bidwidget.bid") << ": ";

			if(m_avatars[i].getPlayerStats().m_playerBid == -1)
			{
				ss << m_language->getElement("bidwidget.hasnotbid");
			}
			else if(m_avatars[i].getPlayerStats().m_playerBid == 0)
			{
				ss << m_language->getElement("bidwidget.nil");
			}
			else
			{
				ss << m_avatars[i].getPlayerStats().m_playerBid;
			}

			ss << "\n";
			ss << m_language->getElement("bidwidget.won") << ": ";
			ss << m_avatars[i].getPlayerStats().m_playerPoints;

			int bags = m_avatars[i].getPlayerStats().m_playerPoints -
				m_avatars[i].getPlayerStats().m_playerBid;
			if(m_avatars[i].getPlayerStats().m_playerBid > -1 && bags > 0)
			{
				ss << "\n";
				ss << m_language->getElement("scorewidget.bags") << ": ";
				ss << bags;
			}

			m_avatars[i].setBidText(ss.str());
		}
	}

	void AvatarManager::setChairs( Chair* top, Chair* bottom, Chair* left, Chair* right )
	{
		m_topChair = top;
		m_bottomChair = bottom;
		m_leftChair = left;
		m_rightChair = right;
	}

	void AvatarManager::mouseDownCB( agui::MouseEvent &evt )
	{
		for(int i = 0; i < getNumAvatars(); i++)
		{
			if((evt.getButton() == agui::MOUSE_BUTTON_RIGHT || evt.getButton() == agui::MOUSE_BUTTON_LEFT) 
				&& evt.getSourceWidget() == m_avatars[i].getText().getWidget())
			{
				if(m_avatars[i].getText().getText() == "")
					continue;

				PlayerMenuContext context = m_playerWatching ? AVATAR_WATCHER_RCLICK : AVATAR_RCLICK;
				DISPATCH_GAME_EVENT
					(*it)->showPlayerGameContext(m_avatars[i].getText().getText(),evt.getPosition().getX(),evt.getPosition().getY(),context,
					m_avatars[i].getText().getWidget(),i);
				break;
			}
		}

		if(evt.getSourceWidget() == m_tooltip || evt.getSourceWidget() == NULL)
		{
			m_tooltip->hideToolTip();
		}
	}

	void AvatarManager::showBidTooltip( int x, int y, agui::Widget* invoker )
	{
		for(int i = 0; i < m_avatars.size(); i++)
		{
			if(m_avatars[i].getText().getWidget() == invoker)
			{
                if(!Platf::isMobile())
				m_tooltip->showToolTip(m_avatars[i].getBidText(),m_gui->getMaxToolTipWidth(),x + 10,y + 10,m_gui->getTop());
                else
                m_tooltip->showToolTip(m_avatars[i].getBidText(),m_gui->getMaxToolTipWidth(),invoker->getAbsolutePosition().getX() + (invoker->getWidth() / 2),invoker->getAbsolutePosition().getY() +  (invoker->getHeight() / 2),m_gui->getTop());
				m_tooltip->bringToFront();
				break;
			}
		}
	}

	void AvatarManager::mouseMoveCB( agui::MouseEvent &evt )
	{
		if(evt.getSourceWidget() == NULL)
		{
			if(!m_tooltip->isVisible())
				return;
			int x1 = m_tooltip->getAbsolutePosition().getX() + (m_tooltip->getWidth() / 2);
			int y1 = m_tooltip->getAbsolutePosition().getY() + (m_tooltip->getHeight() / 2);

			int x2 = evt.getX();
			int y2 = evt.getY();

			int xdiff = x2 - x1;
			int ydiff = y2 - y1;
			float dist = sqrt((float)(xdiff * xdiff) + (float)(ydiff * ydiff));

			if(dist > 150)
			{
				m_tooltip->hideToolTip();
			}
		}
	}

	void AvatarManager::setPlayerWatching( bool watching )
	{
		m_playerWatching = watching;
	}

	void AvatarManager::elemLogic( double t )
	{
		if(m_lastTime == 0)
		{
			m_lastTime = al_get_time();
		}
		else
		{
			double delta = al_get_time() - m_lastTime;
			m_lastTime = al_get_time();

			for(int i = 0; i < m_avatars.size(); i++)
			{
				m_avatars[i].updateTime(delta);
			}
		}

		{
			float numFrames = 35;
			float incr = 1.0f / numFrames;

			for(int i = 0; i < 4; i++)
			{
				AvatarGlow* g = &m_avatars[i].getGlow();

				if(!g->active)
				{
					if(g->time > 0)
					{
						g->time -= incr;
						if(g->time <= 0)
						{
							g->time = 0.0f;
							g->increasing = true;
						}
					}
				}
				else
				{
					if(g->increasing)
					{
						g->time += incr;
						if(g->time >= 1.0f)
						{
							g->time = 1.0f;
							g->increasing = false;
						}
					}
					else
					{
						g->time -= incr;

						if(g->time <= 0.0f)
						{
							g->time = 0.0f;
							g->increasing = true;
						}
					}
				}


				g->curR = lerp(g->time,g->minR,g->peakR);
				g->curG = lerp(g->time,g->minG,g->peakG);
				g->curB = lerp(g->time,g->minB,g->peakB);

				m_avatars[i].getComposite().setShadowColor(agui::Color(g->curR,g->curG,g->curB));
			}

			{
				for(int i = 0; i < 4; i++)
				{
					AvatarEffect* g = &m_avatars[i].getEffect();
					float incr = g->speed;

					if(m_avatars[i].isEffectActive())
					{
						if(g->increasing)
						{
							g->time += incr;
							if(g->time >= 1.0f)
							{
								g->time = 1.0f;
								g->increasing = false;
								g->numAlterationsCompleted++;
							}
						}
						else
						{
							g->time -= incr;

							if(g->time <= 0.0f)
							{
								g->time = 0.0f;
								g->increasing = true;
								g->numAlterationsCompleted++;
							}
						}

						g->curR = lerp(g->time,g->minR,g->peakR);
						g->curG = lerp(g->time,g->minG,g->peakG);
						g->curB = lerp(g->time,g->minB,g->peakB);

						m_avatars[i].getComposite().setAvatarColor(agui::Color(g->curR,g->curG,g->curB));

						if(g->numAlterationsCompleted >= g->numAlterations)
						{
							m_avatars[i].setEffectActive(false);
						}
					}
				}
			}
		}
	}

	void AvatarManager::setEffect( int player, float speed, int numAlterations, float stickTime, const agui::Color& minColor, const agui::Color& maxColor )
	{
		if(player >= 0 && player < getNumAvatars())
		{
			AvatarEffect e;
			e.speed = speed;
			e.numAlterations = numAlterations;
			e.minR = minColor.getR();
			e.minG = minColor.getG();
			e.minB = minColor.getB();
			e.peakR = maxColor.getR();
			e.peakG = maxColor.getG();
			e.peakB = maxColor.getB();
			e.stickTime = stickTime;

			if(!m_avatars[player].isEffectActive())
			m_avatars[player].setEffect(e);
		}
	}

	void AvatarManager::giftExploded( const std::string& giftName, int srcPlayer, int destPlayer )
	{
		if(giftName == "gift.cloud")
		{
			setEffect(destPlayer,1.0f / 10.0f,11,0.0f,agui::Color(0.0f,0.0f,0.2f),agui::Color(1.0f,1.0f,1.0f));
		}
		else if(giftName == "gift.coin" || giftName == "gift.sunflower" || giftName == "gift.star")
		{
			setEffect(destPlayer,1.0f / 90.0f,1,0.0f,agui::Color(0.9f,0.85f,0.55f),agui::Color(1.0f,1.0f,1.0f));
		}
		else if(giftName == "gift.pie")
		{
			setEffect(destPlayer,1.0f / 140.0f,1,0.0f,agui::Color(0.7f,0.7f,1.0f),agui::Color(1.0f,1.0f,1.0f));
		}
		else if(giftName == "gift.flake")
		{
			setEffect(destPlayer,1.0f / 120.0f,1,0.0f,agui::Color(0.5f,0.5f,0.8f),agui::Color(1.0f,1.0f,1.0f));
		}
		else if(giftName == "gift.heart" || giftName == "gift.pain")
		{
			setEffect(destPlayer,1.0f / 100.0f,1,0.0f,agui::Color(1.0f,0.7f,0.7f),agui::Color(1.0f,1.0f,1.0f));
		}
		else if(giftName == "gift.luck")
		{
			setEffect(destPlayer,1.0f / 100.0f,1,0.0f,agui::Color(0.7f,1.0f,1.0f),agui::Color(1.0f,1.0f,1.0f));
		}
	}

	void AvatarManager::showPlayerNamesChanged( bool show )
	{
		m_showPlayerNames = show;
		DISPATCH_SCENE_EVENT
			(*it)->setBoolSetting("player.names.on",show);
	}

	void AvatarManager::loadSettings( ClientShared* shared )
	{
		bool show = shared->getSettingsManager()->getBoolSetting("player.names.on");
		showPlayerNamesChanged(show);
	}

	void AvatarManager::showPlayerStrike( int player, int numStrikes, int time, bool begin )
	{
		for(int i = 0; i < m_avatars.size(); i++)
		{
			if(i == player)
			{
				m_avatars[i].setStallStrikes(numStrikes);
				m_avatars[i].setStallTime(time);
			}
		}
	}

}
