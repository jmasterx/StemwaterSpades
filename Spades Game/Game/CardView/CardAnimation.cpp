#include "Game/CardView/CardAnimation.hpp"
#include "Game/Utility/Math.hpp"
#include <cmath>
namespace cge
{
	void CardAnimation::step()
	{
	
		if(m_stepDelay > 0)
		{
			m_stepDelay--;
			return;
		}

			m_interval += m_rate;

		if(m_interval > 1.0f)
		{
			m_interval = 1.0f;
		}

		if(m_targetCard)
		{
			if(m_targetCard->m_tempCard != NULL && m_change.size() > 0)
			m_targetCard->m_card = m_targetCard->m_tempCard;

			if(m_startCard.m_angle != m_endCard.m_angle)
			{
				if(abs(m_endCard.m_angle - m_startCard.m_angle) > CGE_PI && 
					!m_unrestrictedAngle)
				{
					if(m_endCard.m_angle >= 0.0f && m_endCard.m_angle < m_startCard.m_angle
						&& m_endCard.m_angle <= 2.0f * CGE_PI)
					{
						m_targetCard->m_angle = 
							Math::valueAt(m_startCard.m_angle, 
							m_endCard.m_angle + (CGE_PI * 2.0f),m_interval);
					}
					else
					{
						m_targetCard->m_angle = 
							Math::valueAt(m_startCard.m_angle,
							m_endCard.m_angle - (CGE_PI * 2.0f),m_interval);
					}
				}
				else
				{
					m_targetCard->m_angle = 
						Math::valueAt(m_startCard.m_angle, m_endCard.m_angle,m_interval);
				}
				
			}

			if(m_startCard.m_scale != m_endCard.m_scale)
			{
				m_targetCard->m_scale = 
					Math::valueAt(m_startCard.m_scale,m_endCard.m_scale,m_interval);
			}

			if(m_startCard.m_position.getX() != m_endCard.m_position.getX())
			{
				m_targetCard->m_position.setX(
					Math::valueAt(m_startCard.m_position.getX(),
					m_endCard.m_position.getX(),m_interval)); 		
			}

			if(m_startCard.m_position.getY() != m_endCard.m_position.getY())
			{
				m_targetCard->m_position.setY(
					Math::valueAt(m_startCard.m_position.getY(),
					m_endCard.m_position.getY(),m_interval)); 		
			}

			if(m_startCard.m_shadowDistance != m_endCard.m_shadowDistance)
			{
				m_targetCard->m_shadowDistance =
					Math::valueAt(m_startCard.m_shadowDistance,
					m_endCard.m_shadowDistance,m_interval); 		
			}

			if(m_startCard.m_shadowIntensity != m_endCard.m_shadowIntensity)
			{
				m_targetCard->m_shadowIntensity =
					Math::valueAt(m_startCard.m_shadowIntensity,
					m_endCard.m_shadowIntensity,m_interval); 		
			}

			if(m_startCard.m_shadowNormal.getX() != m_endCard.m_shadowNormal.getX())
			{
				m_targetCard->m_shadowNormal.setX(
					Math::valueAt(m_startCard.m_shadowNormal.getX(),
					m_endCard.m_shadowNormal.getX(),m_interval)); 		
			}

			if(m_startCard.m_shadowNormal.getY() != m_endCard.m_shadowNormal.getY())
			{
				m_targetCard->m_shadowNormal.setY(
					Math::valueAt(m_startCard.m_shadowNormal.getY(),
					m_endCard.m_shadowNormal.getY(),m_interval)); 		
			}

			if(m_interval < 1.0f)
			for(size_t i = 0; i < m_change.size(); i++)
			{
				if(m_change[i].frame == m_frames)
					m_targetCard->m_card = m_change[i].card;
			}

			m_frames++;
		}
	}

	CardAnimation::CardAnimation( ViewCard* targetCard, const ViewCard& startCard, const ViewCard& endCard, float rate, int stepDelay /*= 0*/, bool unrestrictedAngle /*= false*/ )
		: m_targetCard(targetCard),
		m_startCard(startCard), m_endCard(endCard),
		m_rate(rate), m_interval(0.0f),
		m_stepDelay(stepDelay),m_unrestrictedAngle(unrestrictedAngle),
		m_frames(0)
	{

		if(m_startCard.m_angle < 0.0f)
		{
			m_startCard.m_angle += 2.0f * CGE_PI;
		}
		if(m_endCard.m_angle < 0.0f)
		{
			m_endCard.m_angle += 2.0f * CGE_PI;
		}

		m_startCard.m_angle = fmod(m_startCard.m_angle, (float)(2.0f * CGE_PI));
		m_endCard.m_angle =  fmod(m_endCard.m_angle, (float)(2.0f * CGE_PI));

		m_targetCard->m_tempCard = m_targetCard->m_card;
	}

	float CardAnimation::getInterval() const
	{
		return m_interval;
	}

	CardAnimation::~CardAnimation( void )
	{

	}

	ViewCard* CardAnimation::getCard() const
	{
		return m_targetCard;
	}

	void CardAnimation::setChanges( const std::vector<CardAnimationChange>& changes )
	{
		m_change = changes;
	}

}
