#ifndef CGE_PROPORTIONS_MANAGER_HPP
#define CGE_PROPORTIONS_MANAGER_HPP
#include "Game/CardView/ViewCard.hpp"
#include "Game/Utility/Vec2.hpp"
#include "Agui/Widget.hpp"
namespace cge
{
	class ProportionsManager
	{
		int m_playingFieldDimension;
		int m_centerX;
		int m_centerY;
		int m_tableSize;
		Vec2 m_tableOffset;
		int m_baseDimension;
		Vec2 m_cardInitialSize;
		int m_width;
		int m_height;
		bool m_basedOnHeight;
		int m_topLightY;
		int m_bottomLightY;
		bool m_isInCompactMode;
	public:
		void setCenter( int x, int y) ;
		void setPlayingFieldDimension( int dimension );
		float getScale() const;
		int getBaseDimension() const;
		int getPlayingFieldDimension() const;
		float getCardScale( const ViewCard* card ) const;
		Vec2 getCardPosition( const ViewCard* card ) const;
		void resize(int width, int height);
		Vec2 getInitialCardSize() const;
		int getTableSize() const;
		int getScreenWidth() const;
		int getScreenHeight() const;
		const Vec2& getTableOffset() const;
		int getCenterX() const;
		int getCenterY() const;
		float getAspectRatio() const;
		float getInverseAspectRatio() const;
		bool isInCompactMode() const;
		void setTopLightY(int light);
		int getTopLightY() const;
		void setBottomLightY(int light);
		int getBottomLightY() const;
		ProportionsManager(void);
		virtual ~ProportionsManager(void);
	};
}

#endif
