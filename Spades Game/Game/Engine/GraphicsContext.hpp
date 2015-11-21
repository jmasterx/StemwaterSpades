#ifndef CGE_GRAPHICS_CONTEXT_HPP
#define CGE_GRAPHICS_CONTEXT_HPP
#include "Game/Resource/Sprite.hpp"
#include "Game/Resource/Font.hpp"
#include "Game/platform.hpp"
#include "Agui/Agui.hpp"
#include <string>
#include "Game/Shader/Shader.hpp"
#include "Game/Engine/Transformation.hpp"
namespace cge
{
	class GraphicsContext
	{
		ALLEGRO_COLOR black;
		Sprite* m_backBuffer;
		Shader* m_shader;
		Transformation m_transform;
		bool m_tripleBuffer;
		Sprite* m_buffer;
		bool m_needsFlip;
	public:
		void clear();
		void setClippingRect(int x, int y, int w, int h);
		void getClippingRect(int&x,int&y,int&w,int&h);
		void clear(const Color& color);
		void clearTransparent();
		void setTarget(Sprite* sprite);
		void setTargetToBackbuffer();
		void setTargetToBuffer();
		void holdDrawing();
		void unholdDrawing();
		void drawSprite(Sprite* sprite, float x, float y, int flags);
		void drawTintedSprite(Sprite* sprite, const Color& tint,
			float x, float y, int flags);
		void drawSpriteRegion(Sprite* sprite,
			float sourceX, float sourceY, float sourceWidth, float sourceHeight, float x, float y, int flags);
		void drawTintedSpriteRegion(Sprite* sprite,const Color& tint,
			float sourceX, float sourceY, float sourceWidth, float sourceHeight, float x, float y, int flags);
		void drawRotatedSprite(Sprite* sprite,
			float centerX, float centerY, float x, float y, float radAngle, int flags);
		void drawTintedRotatedSprite(Sprite* sprite, const Color& tint,
			float centerX, float centerY, float x, float y, float radAngle, int flags);
		void drawScaledRotatedSprite(Sprite* sprite,
			float centerX, float centerY, float x, float y, float xScale, float yScale,
			float radAngle, int flags);
		void drawTintedScaledRotatedSprite(Sprite* sprite, const Color& tint,
			float centerX, float centerY, float x, float y, float xScale, float yScale,
			float radAngle, int flags);
		void drawScaledSprite(Sprite *sprite,
			float sourceX, float sourceY, float sourceWidth, float sourceHeight,
			float x, float y, float destinationWidth, float destinationHeight, int flags);
		void drawTintedScaledSprite(Sprite *sprite,const Color & color,
			float sourceX, float sourceY, float sourceWidth, float sourceHeight,
			float x, float y, float destinationWidth, float destinationHeight, int flags);
		void drawText(const std::string& text,Font* font,const Color& color, int x, int y, int flags);
		void drawText(const std::string& text,agui::Font* font,const agui::Color& color, int x, int y, int flags);
		int drawRotatedText(const std::string& text,agui::Font* font,const agui::Color& color, int x, int y, float deg, int flags);
		void drawFilledRectangle(int x, int y, int w, int h,const agui::Color& color);
		void flipDisplay();
		void setShader(Shader* shader);
		void resetTransform();
		void useTransform(Transformation& t);
		void begin();
		void end();
		void resizeBuffer(int w, int h);
		bool isBuffering() const;
		void setBuffering(bool buffering);
		Sprite* getBuffer();
		GraphicsContext(void);
		virtual ~GraphicsContext(void);
	};
}

#endif