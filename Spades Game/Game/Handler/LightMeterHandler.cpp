#include "Game/Handler/LightMeterHandler.hpp"

namespace cge
{
	LightMeterHandler::LightMeterHandler( SpriteManager* spriteManager, int numTricks, GuiFontManager* fontMan, LanguageManager* langMan )
		: m_fontMan(fontMan),m_langMan(langMan)
	{
		//create the meters
		for(int i = 0; i < 4; ++i)
		{
			m_lightMeters.push_back(LightMeter(
				spriteManager->getImage("light.point"),
				spriteManager->getImage("light.partnerpoint"),
				spriteManager->getImage("light.overtrick"),
				spriteManager->getImage("light.nothing"),
				spriteManager->getImage("light.contract"),
				spriteManager->getImage("orb.cap"),
				spriteManager->getImage("orb.cap.red"),
				numTricks,i,langMan->getElement("game.nil")));
		}
	}

	LightMeterHandler::~LightMeterHandler(void)
	{
	}

	void LightMeterHandler::render( GraphicsContext* g )
	{
		for(size_t i = 0; i < m_lightMeters.size(); ++i)
		{
			m_lightMeters[i].render(g);
		}
	}

	void LightMeterHandler::resize( int w, int h )
	{
		for(size_t i = 0; i < m_lightMeters.size(); ++i)
		{
			m_lightMeters[i].resize(w,h);
		}

		agui::Font* f = m_fontMan->getFont(getProportions()->getScale() * 1.9f);

		for(size_t i = 0; i < m_lightMeters.size(); ++i)
		{
			m_lightMeters[i].setFont(f);
		}
	}

	void LightMeterHandler::roundEnded()
	{
		for(size_t i = 0; i < m_lightMeters.size(); ++i)
		{
			m_lightMeters[i].clearTricks();
		}
	}

	void LightMeterHandler::playerBidChanged( int player, int newBid )
	{
		if(player < (int)m_lightMeters.size())
		{
			m_lightMeters[player].setContractTrickCount(newBid);
			m_lightMeters[player].setHasBid(true);
		}
	}

	void LightMeterHandler::playerMadeTrick( int player, SpadesPointEnum trickType )
	{
		if(player < (int)m_lightMeters.size())
		{
			m_lightMeters[player].addTrick(trickType);
		}
	}

	void LightMeterHandler::setProportions( ProportionsManager* manager )
	{
		DynamicElement::setProportions(manager);

		for(size_t i = 0; i < m_lightMeters.size(); ++i)
		{
			m_lightMeters[i].setProportions(manager);
		}
		
	}

	void LightMeterHandler::gameBegin()
	{
		for(size_t i = 0; i < m_lightMeters.size(); ++i)
		{
			m_lightMeters[i].clearTricks();
			m_lightMeters[i].setHasBid(false);
		}
	}

	void LightMeterHandler::roundBegan()
	{
		for(size_t i = 0; i < m_lightMeters.size(); ++i)
		{
			m_lightMeters[i].clearTricks();
			m_lightMeters[i].setHasBid(false);
		}
	}

	void LightMeterHandler::bidStateChanged( const std::vector<SpadesPointEnum> &b,
		const std::vector<SpadesPointEnum> &l, const std::vector<SpadesPointEnum> &t, 
		const std::vector<SpadesPointEnum> &r, std::vector<int> bids )
	{
		for(size_t i = 0; i < m_lightMeters.size(); ++i)
		{
			m_lightMeters[i].clearTricks();
		}

		for(size_t i = 0; i < bids.size(); ++i)
		{
			int bid = bids[i];
			if(bid < 0)
			{
				bid = 0;
			}
			if(bids[i] > -1)
			playerBidChanged(i,bid);
			else
			{
				m_lightMeters[i].clearTricks();
				m_lightMeters[i].setHasBid(false);
			}
		}

		for(size_t i = 0; i < b.size(); ++i)
		{
			playerMadeTrick(0,b[i]);
		}
		for(size_t i = 0; i < l.size(); ++i)
		{
			playerMadeTrick(1,l[i]);
		}

		for(size_t i = 0; i < t.size(); ++i)
		{
			playerMadeTrick(2,t[i]);
		}

		for(size_t i = 0; i < r.size(); ++i)
		{
			playerMadeTrick(3,r[i]);
		}
	}

	void LightMeterHandler::playerFailedNil( int player )
	{
		if(player < (int)m_lightMeters.size())
		{
			m_lightMeters[player].onFailedNil();
		}
	}

	void LightMeterHandler::elemLogic( double t )
	{
		for(size_t i = 0; i < m_lightMeters.size(); ++i)
		{
			m_lightMeters[i].elemLogic(t);
		}
	}

	void LightMeterHandler::bidMeterStyleChanged( bool useLights )
	{
		DISPATCH_SCENE_EVENT
			(*it)->setBoolSetting("light.meter.on",useLights);

		for(size_t i = 0; i < m_lightMeters.size(); ++i)
		{
			m_lightMeters[i].setUseLights(false); //never use lights
		}
	}

	void LightMeterHandler::loadSettings( ClientShared* shared )
	{
		bool useLights = shared->getSettingsManager()->getBoolSetting("light.meter.on");
		bidMeterStyleChanged(useLights);
	}

}

