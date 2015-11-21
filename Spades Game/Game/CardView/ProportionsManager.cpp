#include "Game/CardView/ProportionsManager.hpp"

namespace cge
{
	ProportionsManager::ProportionsManager(void)
		: m_playingFieldDimension(600),
		m_baseDimension(1200),
		m_centerX(1),m_centerY(1),
		m_cardInitialSize(190,276),
		m_width(0), m_height(0),
		m_tableSize(1200),
		m_tableOffset(0.5f,0.5f),
		m_basedOnHeight(true),
		m_topLightY(0),m_bottomLightY(0),
		m_isInCompactMode(false)
	{
	}

	ProportionsManager::~ProportionsManager(void)
	{
	}

	int ProportionsManager::getBaseDimension() const
	{
		return m_baseDimension;
	}

	void ProportionsManager::setCenter( int x, int y )
	{
		m_centerX = x;
		m_centerY = y;
	}

	void ProportionsManager::setPlayingFieldDimension( int dimension )
	{
		m_playingFieldDimension = dimension;
	}

	float ProportionsManager::getCardScale( const ViewCard* card ) const
	{
		return getScale() * card->m_scale;
	}

	float ProportionsManager::getScale() const
	{
		return (float)m_playingFieldDimension /(float) m_baseDimension;
	}

	Vec2 ProportionsManager::getInitialCardSize() const
	{
		return m_cardInitialSize;
	}

	int ProportionsManager::getPlayingFieldDimension() const
	{
		return m_playingFieldDimension;
	}

	Vec2 ProportionsManager::getCardPosition( const ViewCard* card ) const
	{
		if(card == NULL)
			return Vec2();
		return 	Vec2((card->m_position.getX() * getBaseDimension() * getScale()) + m_centerX,
			(card->m_position.getY() * getBaseDimension() * getScale()) + m_centerY);
	}

	void ProportionsManager::resize( int width, int height )
	{
		float shrinkScalar = 0.92f;
		setPlayingFieldDimension(m_basedOnHeight ? height * 0.65f * shrinkScalar : width);
		setCenter(width / 2,height / 2);
		
		if(height > 0 && ((float)width / (float)height) < 1.55f && m_basedOnHeight)
		{
			setPlayingFieldDimension(width * 0.485f * shrinkScalar);
			m_isInCompactMode = true;
		}
		else
		{
			m_isInCompactMode = false;
		}
		
		m_width = width;
		m_height = height;
	}

	int ProportionsManager::getScreenWidth() const
	{
		return m_width;
	}

	int ProportionsManager::getScreenHeight() const
	{
		return m_height;
	}

	int ProportionsManager::getTableSize() const
	{
		return m_tableSize;
	}

	const Vec2& ProportionsManager::getTableOffset() const
	{
		return m_tableOffset;
	}

	int ProportionsManager::getCenterX() const
	{
		return m_centerX;
	}

	int ProportionsManager::getCenterY() const
	{
		return m_centerY;
	}

	float ProportionsManager::getAspectRatio() const
	{
		if(m_height == 0)
		{
			return 1.0f;
		}

		return m_width / m_height;
	}

	float ProportionsManager::getInverseAspectRatio() const
	{
		if(m_width == 0)
		{
			return 1.0f;
		}

		return m_height / m_width;
	}

	void ProportionsManager::setBottomLightY( int light )
	{
		m_bottomLightY = light;
	}

	int ProportionsManager::getBottomLightY() const
	{
		return m_bottomLightY;
	}

	void ProportionsManager::setTopLightY( int light )
	{
		m_topLightY = light;
	}

	int ProportionsManager::getTopLightY() const
	{
		return m_topLightY;
	}

	bool ProportionsManager::isInCompactMode() const
	{
		return m_isInCompactMode;
	}

}

