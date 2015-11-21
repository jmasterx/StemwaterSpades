#include "Game/Element/GiftRenderer.hpp"
namespace cge
{
	float glerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}

	float distance(float x1, float y1, float x2, float y2)
	{
		float xdiff = x2 - x1;
		float ydiff = y2 - y1;
		float dist = sqrt((float)(xdiff * xdiff) + (float)(ydiff * ydiff));

		return dist;
	}

	GiftRenderer::GiftRenderer( GiftManager* giftMan, const SpadesGameInfo& info, agui::Gui* gui, ClientCore* net )
		: m_giftMan(giftMan),m_info(info),m_widget(NULL,NULL,NULL,NULL,NULL,NULL),m_watching(false),
		m_animationTime(0.0),m_net(net)
	{
		m_gui = gui;
		gui->add(&m_widget);
		m_widget.addActionListener(this);
		m_widget.setFocusable(false);
		m_widget.setEnterCursor(agui::CursorProvider::LINK_CURSOR);
		m_brightener = Shader::loadShader("brighten");
		for(int i = 0; i < 4; i++)
		{
			m_giftID[i] = -1;
		}
	}


	GiftRenderer::~GiftRenderer(void)
	{
		m_widget.removeActionListener(this);
		m_gui->remove(&m_widget);
		delete m_brightener;
		std::list<ViewGift>::iterator i = m_gifts.begin();
		while (i != m_gifts.end())
		{
			delete i->image;
			i->image = NULL;
			m_gifts.erase(i++);  // alternatively, i = items.erase(i);
		}
	}

	void GiftRenderer::render( GraphicsContext* g )
	{
	}

	void GiftRenderer::addGift( int senderPlayer, int targetPlayer, int giftID, int frame, bool restore, bool triggerSound )
	{

		ViewGift gift;
		GiftData* dat = m_giftMan->getGiftById(giftID);
		if(!dat)
			return;

        if(dat->m_animated && m_net->isBGExec())
        {
            return;
        }
        
		if(!dat->m_animated)
		{
			for(std::list<ViewGift>::iterator it = m_gifts.begin(); it != m_gifts.end(); it++)
			{
				if(it->targetPlayer == targetPlayer && !it->needsToDie && !it->giftData->m_animated)
				{
					it->needsToDie = true;
				}
			}
		}

		if(giftID == -1)
			return;

		float srcXList[4] = {
			0.0f,-1.0f,0.0f,1.0f
		};

		float srcYList[4] = {
			1.0f,0.0f,-1.0f,0.0f
		};

        float pc = 1.25f;

		float destXList[4] = {
			0.69f,-pc,-0.65f,pc
		};

		float destYList[4] = {
			1.25f,0.68f,-1.2,-0.68f
		};

		if(senderPlayer >= 4 || senderPlayer < 0 || targetPlayer >= 4 || targetPlayer < 0)
			return;

		//do not restore animated gifts
		if(dat->m_animated && restore)
			return;


		if(dat->m_animated)
		{
			for(int i = 0; i < 4; i++)
			{
				destXList[i] = srcXList[i];
				destYList[i] = srcYList[i];

				float incr = 0.15f;

				if(i == 0)
					destYList[i] += incr;
				else if(i == 1)
					destXList[i] -= incr;
				else if(i == 2)
					destYList[i] -= incr;
				else if(i == 3)
					destXList[i] += incr;
			}
			
		}
		gift.giftData = dat;
		m_giftID[targetPlayer] = giftID;

		if(dat->m_spritePaths.empty())
			return;

		if(frame < 0 || frame >= dat->m_spritePaths.size())
			frame = dat->m_spritePaths.size() - 1;

		if(dat->m_animated)
		{
			frame = 0;
			gift.isAnimated = true;
			gift.image = NULL;
			gift.animImgInitial = new Sprite(dat->m_spritePaths[0]);
			gift.animImgChild = new Sprite(dat->m_spritePaths[1]);
			gift.animImgTrail = new Sprite(dat->m_spritePaths[2]);
			gift.giftAPS = new GiftAPS();
			gift.giftAPS->setProportions(getProportions());
			gift.giftAPS->setImages(gift.animImgInitial,gift.animImgChild,gift.animImgTrail);
			gift.giftAPS->setCallback(this,dat->m_name,senderPlayer,targetPlayer);
			gift.giftAPS->start();
		}
		else
		{
			gift.image = new Sprite(dat->m_spritePaths[frame]);
		}
        
        float mobileamt = 1.0f;
        if(Platf::isMobile())
            mobileamt = 0.9f;

		gift.frame = frame;
		gift.scale = m_giftMan->getGiftById(giftID)->m_scale * mobileamt;
		gift.senderPlayer = senderPlayer;
		gift.targetPlayer = targetPlayer;
		gift.animScale = 0.0f;
		gift.curOpacity = 0.0f;
		gift.curScale = 0.0f;
		gift.destX = destXList[targetPlayer];
		gift.destY = destYList[targetPlayer];
		gift.srcX = srcXList[senderPlayer];
		gift.srcY = srcYList[senderPlayer];
		float dist = distance(gift.srcX,gift.srcY,gift.destX,gift.destY);
		gift.timeIncrease = 0.8;
		if(dat->m_animated)
			gift.timeIncrease = 1.5f;
		gift.timeIncrease = 1.0f / gift.timeIncrease;
		gift.time = 0.0f;

		if(restore)
			gift.time = 1.0f;

		if(senderPlayer == targetPlayer)
		{
			gift.srcX = gift.destX;
			gift.srcY = gift.destY;
		}

		if(triggerSound && !m_net->isBGExec())
		{
			if(frame >= 0 && frame < gift.giftData->m_soundPaths.size() &&
				!gift.giftData->m_soundPaths.empty())
			{
				std::string sndPath = gift.giftData->m_soundPaths[frame];
				float gain = gift.giftData->m_soundGains[frame];

				if(sndPath.length() > 0)
				{
					DISPATCH_SCENE_EVENT
						(*it)->playSoundOnce(sndPath,gain);
				}
			}
		}

		m_gifts.push_back(gift);
	}

	void GiftRenderer::updateGift( ViewGift* g, float t )
	{
		float opacityDecrease = 1.0f * t;
		float constScalar = 1.0f;
		float baseSize = 256.0f;

		if(g->isAnimated && g->giftAPS->isDone())
		{
			g->isDead = true;
		}

		if(g->isDead)
		{
			g->curOpacity = 0.0f;
		}
		else if(g->needsToDie)
		{
			g->curOpacity -= opacityDecrease;
			if(g->curOpacity < 0)
			{
				g->curOpacity = 0;
				g->isDead = true;
				g->needsToDie = false;
			}
		}
		else
		{
			g->curOpacity = glerp(0.0f,1.0f,g->time);
		}

		float mx = (int)(getProportions()->getTableOffset().getX() * 
			(float)getProportions()->getScreenWidth());
		float my = (int)(getProportions()->getTableOffset().getY() * 
			(float)getProportions()->getScreenHeight());

		int tableOffset = (int)((float)getProportions()->getTableSize()) / 2;

		g->curX = (glerp(g->srcX,g->destX,g->time) * tableOffset * getProportions()->getScale()) + mx;
		g->curY = (glerp(g->srcY,g->destY,g->time) * tableOffset * getProportions()->getScale()) + my;
		g->animScale = glerp(0.0f,1.0f,g->time);
		g->curScale = g->animScale * g->scale * constScalar;

		float w;
		float h;

		if(g->isAnimated)
		{
			g->width = baseSize * g->curScale;
			g->height = g->width;
		}
		else
		{
			if(g->image->getWidth() > g->image->getHeight())
			{
				w = baseSize;
				w *= g->curScale;
				float aspect = g->image->getHeight() / (float)g->image->getWidth();
				h = w * aspect;
			}
			else
			{
				h = baseSize;
				h *= g->curScale;
				float aspect = g->image->getWidth() / (float)g->image->getHeight();
				w = h * aspect;
			}

			g->width = w;
			g->height = h;
		}

		g->time += g->timeIncrease * t;
		if(g->time >= 1.0f)
			g->time = 1.0f;

		m_animationTime += t;

		if(g->isAnimated)
		{
			g->giftAPS->setWorldPosition(Vec2(g->curX ,
				g->curY ));
			//todo: cleaner solution than direct al call
			g->giftAPS->elemLogic(al_get_time());
			g->giftAPS->setOpacity(g->curOpacity);
		}
	}

	void GiftRenderer::elemLogic( double t )
	{
		m_widget.getGui()->toggleWidgetLocationChanged(false);
		int curW = m_widget.getWidth();
		int curH = m_widget.getHeight();
		int curX = m_widget.getLocation().getX();
		int curY = m_widget.getLocation().getY();
		m_widget.setSize(0,0);
		std::list<ViewGift>::iterator i = m_gifts.begin();
		bool en = m_widget.isEnabled();
		bool anyNonAnimated = false;
		while (i != m_gifts.end())
		{
			updateGift(&(*i),t);
			if(i->time == 1.0f && i->targetPlayer == 0)
			{
				if(!anyNonAnimated)
				{
					float w = i->width * getProportions()->getScale();
					float h = i->height * getProportions()->getScale();
					float x = i->curX - (w/2);
					float y = i->curY - (h/2);

					m_widget.setSize(w,h);
					m_widget.setLocation(x,y);
				}

				bool enabled = i->frame < i->giftData->m_spritePaths.size() - 1;
				if(i->frame == i->giftData->m_spritePaths.size() - 1  &&
					i->needsToDie || i->isDead)
					enabled = false;

				if(i->isAnimated) {
					if(!anyNonAnimated)
					enabled = false;
				}
				else
				{
					anyNonAnimated = true;
				}

				m_widget.setEnabled(enabled && !m_watching);
			}

			if (i->isDead)
			{
				delete i->image;
				delete i->animImgInitial;
				delete i->animImgTrail;
				delete i->animImgChild;
				delete i->giftAPS;
				i->image = NULL;
				i->animImgInitial = NULL;
				i->animImgChild = NULL;
				i->animImgTrail = NULL;
				i->giftAPS = NULL;
				m_gifts.erase(i++);  // alternatively, i = items.erase(i);
			}
			else
			{
				++i;
			}
		}

		m_widget.getGui()->toggleWidgetLocationChanged(true);
		int newW = m_widget.getWidth();
		int newH = m_widget.getHeight();
		int newX = m_widget.getLocation().getX();
		int newY = m_widget.getLocation().getY();

		if(curX != newX || curY != newY || curW != newW || curH != newH || m_widget.isEnabled() != en)
		{
			m_widget.getGui()->_widgetLocationChanged();
		}

	}

	void GiftRenderer::receiveGift( int fromPlayer, int toPlayer, int giftID )
	{
		if(toPlayer > -1)
		{
			addGift(fromPlayer,toPlayer,giftID,0,false,true);
		}
		else if(m_info.getNumPlayers() == 3)
		{
			if(fromPlayer == 0)
			{
				addGift(fromPlayer,1,giftID,0,false,true);
				addGift(fromPlayer,3,giftID,0,false,false);
			}
			else if(fromPlayer == 1)
			{
				addGift(fromPlayer,0,giftID,0,false,true);
				addGift(fromPlayer,3,giftID,0,false,false);
			}
			else if(fromPlayer == 3)
			{
				addGift(fromPlayer,0,giftID,0,false,true);
				addGift(fromPlayer,1,giftID,0,false,false);
			}
		}
		else if(m_info.getNumPlayers() == 4)
		{
			bool hasSounded = false;
			for(int i = 0; i < 4; i++)
			{
				if(fromPlayer != i)
				{
					addGift(fromPlayer,i,giftID,0,false,!hasSounded);
					hasSounded = true;
				}
			}
		}
	}

	void GiftRenderer::resize( int w, int h )
	{

	}

	void GiftRenderer::actualRender( GraphicsContext* g )
	{
		for(std::list<ViewGift>::iterator it = m_gifts.begin(); it != m_gifts.end(); it++)
		{
#ifndef CGE_MOBILE
			if(!it->isAnimated && it->time == 1.0f && it->targetPlayer == 0 && m_gui->getWidgetUnderMouse() == &m_widget)
			{
				it->hoverScalar += 0.03f;
				if(it->hoverScalar > 1.15f)
				{
					it->hoverScalar = 1.15f;
				}
			}
			else if(!it->isAnimated && it->time == 1.0f && it->targetPlayer == 0 && m_gui->getWidgetUnderMouse() != &m_widget)
			{
				it->hoverScalar -= 0.03f;
				if(it->hoverScalar < 1.0f)
				{
					it->hoverScalar = 1.0f;
				}
			}
#endif

			if(!it->isAnimated)
			{
				float w = it->width * it->hoverScalar * getProportions()->getScale();
				float h = it->height * it->hoverScalar * getProportions()->getScale();
				float x = it->curX;
				float y = it->curY;
				float a = it->curOpacity;
				float iw = it->image->getWidth();
				float ih = it->image->getHeight();
				Sprite* img = it->image;
#ifndef CGE_MOBILE
				if(it->time == 1.0f && it->targetPlayer == 0 && m_gui->getWidgetUnderMouse() == &m_widget)
				{
					m_brightener->use();
				}
#endif
				g->drawTintedScaledSprite(img,Color(a,a,a,a),0,0,iw,ih,x - (w/2),y - (h/2),w,h,0);
#ifndef CGE_MOBILE
				m_brightener->stop();
#endif
			}
			else
			{
				it->giftAPS->render(*g);
			}
		}
	}

	void GiftRenderer::actionPerformed( const agui::ActionEvent &evt )
	{
		if(evt.getSource() == &m_widget)
		{
			std::list<ViewGift>::iterator i = m_gifts.begin();
			while (i != m_gifts.end())
			{
					if(i->frame == i->giftData->m_spritePaths.size() - 1  &&
						i->targetPlayer == 0)
						return;
					i++;
			}

			DISPATCH_GAME_EVENT
				(*it)->sendGiftUpdate();
		}
	}

	void GiftRenderer::receiveGiftUpdate( int player, int giftID, int frame )
	{
		if(giftID == -1)
		{
			for(std::list<ViewGift>::iterator it = m_gifts.begin(); it != m_gifts.end(); it++)
			{
				if(it->targetPlayer == player && !it->needsToDie && !it->isAnimated)
				{
					it->needsToDie = true;
				}
			}
		}
		else
		{
			addGift(player,player,giftID,frame,false,true);
		}
	}

	void GiftRenderer::setWatching( bool watching )
	{
		m_watching = watching;
	}

	void GiftRenderer::onGiftExplode( const std::string& giftName, int srcPlayer, int destPlayer )
	{
		DISPATCH_GAME_EVENT
			(*it)->giftExploded(giftName,srcPlayer,destPlayer);
	}

}