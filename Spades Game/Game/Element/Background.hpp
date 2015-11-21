#ifndef CGE_BACKGROUND_HPP
#define CGE_BACKGROUND_HPP
#include "Game/Element/DynamicElement.hpp"
#include <vector>
namespace cge
{
	class Background: public DynamicElement
	{
		float m_offsetX;
		float m_offsetY;
		float m_scale;
		std::vector<Sprite*> m_bg;
	public:
		Background(void);
		void setBackground(float offsetX, float offsetY, float scale, const std::vector<Sprite*>& bg);

		virtual void render(GraphicsContext* g);
        void setOffsetX(float x);
        float getOffsetX() const;
		virtual ~Background(void);
	};
}

#endif