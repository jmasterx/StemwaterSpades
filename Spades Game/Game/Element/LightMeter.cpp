#include "Game/Element/LightMeter.hpp"
#include "Game/CardView/CardSettings.hpp"
#include "Game/Utility/Math.hpp"
namespace cge
{
	LightMeter::~LightMeter(void)
	{
	}

	void LightMeter::render( GraphicsContext* g )
	{
		float w = m_baseSize * getProportions()->getScale();
		if(!isUsingLights())
		{
			if(m_hasBid)
			{
				std::stringstream ss;
				int xpos = 	((getLightPosAt(m_lights.size() / 2).getX() + m_center.getX()) *
					getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterX() - (w / 2);

				int ypos = ((getLightPosAt(m_lights.size() / 2).getY() + m_center.getY()) * 
					getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterY() - (w / 2);

				float posMul = 245.0f;
				if(m_playerNum == 0 || m_playerNum == 2)
				{
					xpos = getProportions()->getScreenWidth() / 2;

					if(m_playerNum == 2)
						ypos -= getProportions()->getScale() * posMul;
					else
						ypos += getProportions()->getScale() * 331.0f;
				}
				else
				{
					ypos = getProportions()->getScreenHeight() / 2;

					if(m_playerNum == 1)
						xpos -= getProportions()->getScale() * posMul;
					else
						xpos += getProportions()->getScale() * posMul;
				}
				float ang = 0.0f;
				if(m_playerNum == 1)
				{
					ang = CGE_PI * 0.5f;
				}
				else if(m_playerNum == 3)
				{
					ang = CGE_PI * 1.5f;
				}

				ang = 0.0f;

				float ww = 137.0f * getProportions()->getScale();
				float aspect = (float)m_orbCap->getHeight() / m_orbCap->getWidth();
				float hh = aspect * ww;
				ww *= 1.05f;


				if(m_playerNum == 0 || m_playerNum == 2)
				{
					if(m_playerNum == 0)
						ypos += 1.5f * m_font->getLineHeight() / 2;
				}
				else
				{
					xpos += m_font->getLineHeight() / 2;
				}

				float capOpacity = 0.95f;
			
				if(!m_nilFailMode)
				{
					g->drawTintedScaledSprite(m_orbCap,Color(capOpacity,capOpacity,capOpacity,capOpacity),0,0,m_orbCap->getWidth(),m_orbCap->getHeight(),
						(ww * 0.01f) + xpos - (ww / 2),
						(hh * 0.04f) + ypos - (hh / 2),
						ww,hh,0);
				}
				else
				{
					float op1 = 1.0f - m_nilFailedOpacity;
					float op2 = m_nilFailedOpacity;

					g->drawTintedScaledSprite(m_orbCap,Color(op1,op1,op1,op1),0,0,m_orbCap->getWidth(),m_orbCap->getHeight(),
						(ww * 0.01f) + xpos - (ww / 2),
						(hh * 0.04f) + ypos - (hh / 2),
						ww,hh,0);

					g->drawTintedScaledSprite(m_orbRedCap,Color(op2,op2,op2,op2),0,0,m_orbRedCap->getWidth(),m_orbRedCap->getHeight(),
						(ww * 0.01f) + xpos - (ww / 2),
						(hh * 0.04f) + ypos - (hh / 2),
						ww,hh,0);
				}
			

				if(getNumTricksMade() == 0 && getContractAmount() == 0)
				{
					ss << m_nilStr;
				}
				else
				{
					ss << getNumTricksMade();
					ss << "/";
					ss << getContractAmount();
				}

				int opac = 255;
				g->drawRotatedText(ss.str(),m_font,agui::Color(255,255,255),xpos,ypos,ang,0);
			}
		}
		else
		{
			for(int i = 0; i < getTotalLightCount(); ++i)
			{
				Sprite* img = NULL;
				switch(getLightValueAt(i))
				{
				case CONTRACT_POINT:
					img = m_contractPointImg;
					break;
				case REGULAR_POINT:
					img = m_regularPointImg;
					break;
				case OVERTRICK_POINT:
					img = m_overTrickImg;
					break;
				case NOT_A_POINT:
					img = m_notAPointImg;
					break;
				case POINT_FROM_PARTNER:
					img = m_pointFromPartnerImg;
					break;
				}

				if(!img)
				{
					return;
				}
				Color c = Color(1.0f,1.0f,1.0f,1.0f);
				if(!m_hasBid)
				{
					float ca = 0.3f;
					c = Color(ca,ca,ca,ca);
				}

				if(m_nilFailMode)
				{
					float op1 = 1.0f - m_nilFailedOpacity;
					float op2 = m_nilFailedOpacity;
					if(m_nilFailIncreasing)
						img = m_notAPointImg;
					g->drawTintedScaledSprite(img,Color(op1,op1,op1,op1),0,0,img->getWidth(),img->getHeight(),
						((getLightPosAt(i).getX() + m_center.getX()) *
						getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterX() - (w / 2),
						((getLightPosAt(i).getY() + m_center.getY()) * 
						getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterY() - (w / 2),
						w,w,0);

					g->drawTintedScaledSprite(m_overTrickImg,Color(op2,op2,op2,op2),0,0,m_overTrickImg->getWidth(),m_overTrickImg->getHeight(),
						((getLightPosAt(i).getX() + m_center.getX()) *
						getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterX() - (w / 2),
						((getLightPosAt(i).getY() + m_center.getY()) * 
						getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterY() - (w / 2),
						w,w,0);
				}
				else
				{
					g->drawTintedScaledSprite(img,c,0,0,img->getWidth(),img->getHeight(),
						((getLightPosAt(i).getX() + m_center.getX()) *
						getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterX() - (w / 2),
						((getLightPosAt(i).getY() + m_center.getY()) * 
						getProportions()->getScale() * getProportions()->getBaseDimension()) + getProportions()->getCenterY() - (w / 2),
						w,w,0);
				}
			}

			if((m_playerNum == 0 || m_playerNum == 2) && m_lights.size() > 0)
			{
				int l = m_lights.size() / 2;
				int y = ((getLightPosAt(l).getY() + m_center.getY()) * 
					getProportions()->getScale() * 
					getProportions()->getBaseDimension()) + getProportions()->getCenterY() - (w / 2);

				if(m_playerNum == 0)
					getProportions()->setBottomLightY(y);
				if(m_playerNum == 2)
					getProportions()->setTopLightY(y);
			}
		}
	}

	int LightMeter::getNumTricksMade() const
	{
		return m_currentCount;
	}

	void LightMeter::addTrick( SpadesPointEnum point )
	{
		if(m_currentCount < getTotalLightCount())
		{
			m_lights[m_currentCount].m_point = point;
			m_currentCount++;
		}
	}

	int LightMeter::getTotalLightCount() const
	{
		return m_lights.size();
	}

	void LightMeter::clearTricks()
	{
		for(int i = 0; i < getTotalLightCount(); ++i)
		{
			m_lights[i].m_point =  NOT_A_POINT;
		}

		m_currentCount = 0;
		resetNilFailed();
	}

	void LightMeter::setContractTrickCount( int count )
	{
		clearTricks();

		for(int i = 0; i < getTotalLightCount() && i < count; ++i)
		{
			m_lights[i].m_point =  CONTRACT_POINT;
		}

		m_totalCount = count;
	}

	SpadesPointEnum LightMeter::getLightValueAt(int index) const
	{
		if(!m_inverted)
		{
			return m_lights[getTotalLightCount() - index - 1].m_point;
		}
		else
		{
			return m_lights[index].m_point;
		}
	}

	void LightMeter::setInverted( bool inverted )
	{
		m_inverted = inverted;
	}

	const Vec2& LightMeter::getLightPosAt( int index ) const
	{
		return m_lights[index].m_pos;
	}

	LightMeter::LightMeter( 
		Sprite* regularPointImg, 
		Sprite* pointFromPartnerImg, 
		Sprite* overTrickImg, 
		Sprite* notAPointImg,
		Sprite* contractPointImg,
		Sprite* orbCap,
		Sprite* orbRedCap,
		int numLights, int playerNum, const std::string& nilStr )
		: m_currentCount(0),m_inverted(playerNum == 1 || playerNum == 2),m_totalCount(0),
		m_lights(numLights,Light(Vec2(0,0),NOT_A_POINT)),
		m_regularPointImg(regularPointImg),m_pointFromPartnerImg(pointFromPartnerImg),
		m_overTrickImg(overTrickImg),m_notAPointImg(notAPointImg), m_orbCap(orbCap),
		m_contractPointImg(contractPointImg),
		m_playerNum(playerNum),
		m_baseSize(32.0f),m_hasBid(false), m_useLights(true),
		m_nilFailedOpacity(0.0f),m_nilFailIncreasing(true),m_nilFailMode(false),
		m_font(NULL),m_orbRedCap(orbRedCap),m_nilStr(nilStr)
	{
		positionLights();
	}

	void LightMeter::positionLights()
	{
		for(int i = 0; i < getTotalLightCount(); ++i)
		{
			float offset = m_playerNum == 0 ? 1.03f : 1.0f;
			if(m_playerNum == 2)
			{
				offset = 1.0f + (0.022f);
			}
			m_lights[i].m_pos = Math::positionFromCenterToLineAt(
				CardSettings::getPosCenter(m_playerNum),
				offset * CardSettings::getHandDistance(1) * (1.0f + (0.42f * CardSettings::getHandScale(1))),
				getTotalLightCount(),i,CardSettings::getHandAngularDistance(1) * 0.9f,
				CardSettings::getHandAngleOffset(m_playerNum));	
		}
	}

	void LightMeter::setHasBid( bool bid )
	{
		m_hasBid = bid;
	}

	void LightMeter::elemLogic( double t )
	{
		float mul = 2.5f;
		float nilFailIncr = 0.07 / mul;
		float nilFailDecr = 0.1f / mul;
		if(m_nilFailMode && !m_nilFailIncreasing && m_nilFailedOpacity <= 0.0f)
		{
			resetNilFailed();
		}

		if(m_nilFailMode)
		{
			if(m_nilFailIncreasing)
			{
				m_nilFailedOpacity += nilFailIncr;

				if(m_nilFailedOpacity >= 1.0f)
				{
					m_nilFailedOpacity = 1.0f;
					m_nilFailIncreasing = false;
				}
			}
			else
			{
				m_nilFailedOpacity -= nilFailDecr;

				if(m_nilFailedOpacity <= 0.0f)
				{
					m_nilFailedOpacity = 0.0f;
				}
			}
		}
	}

	void LightMeter::onFailedNil()
	{
		resetNilFailed();
		m_nilFailMode = true;
	}

	void LightMeter::resetNilFailed()
	{
		m_nilFailedOpacity = 0.0f;
		m_nilFailIncreasing = true;
		m_nilFailMode = false;
	}

	int LightMeter::getContractAmount() const
	{
		if(m_hasBid)
		return m_totalCount;
		else
			return -1;
	}

	void LightMeter::setFont( agui::Font* f )
	{
		m_font = f;
	}

	void LightMeter::setUseLights( bool use )
	{
		m_useLights = use;
	}

	bool LightMeter::isUsingLights() const
	{
		return m_useLights;
	}

}
