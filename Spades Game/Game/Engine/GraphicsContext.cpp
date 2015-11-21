#include "Game/Engine/GraphicsContext.hpp"
#include "Agui/Backends/Allegro5/Allegro5Font.hpp"

namespace cge
{
	GraphicsContext::GraphicsContext(void)
		:m_shader(NULL),m_tripleBuffer(false),
		m_buffer(NULL),m_needsFlip(false)
	{
		black = al_map_rgb(0,0,0);
	}

	GraphicsContext::~GraphicsContext(void)
	{
		if(m_buffer)
		{
			delete m_buffer;
			m_buffer = NULL;
		}
	}

	void GraphicsContext::drawSprite( Sprite* sprite, float x, float y, int flags )
	{
		al_draw_bitmap(sprite->getBitmap(),x,y,flags);
	}

	void GraphicsContext::drawTintedSprite( Sprite* sprite, const Color& tint, float x, float y, int flags )
	{
		al_draw_tinted_bitmap(sprite->getBitmap(),tint.getColor(),x,y,flags);
	}

	void GraphicsContext::drawSpriteRegion( Sprite* sprite, float sourceX, float sourceY, float sourceWidth, float sourceHeight, float x, float y, int flags )
	{
		al_draw_bitmap_region(
			sprite->getBitmap(),
			sourceX,
			sourceY,
			sourceWidth,
			sourceHeight,
			x,
			y,
			flags);
	}

	void GraphicsContext::drawTintedSpriteRegion( Sprite* sprite,const Color& tint, float sourceX, float sourceY, float sourceWidth, float sourceHeight, float x, float y, int flags )
	{
		al_draw_tinted_bitmap_region(
			sprite->getBitmap(),
			tint.getColor(),
			sourceX,
			sourceY,
			sourceWidth,
			sourceHeight,
			x,
			y,
			flags);
	}

	void GraphicsContext::drawRotatedSprite( Sprite* sprite, float centerX, float centerY, float x, float y, float radAngle, int flags )
	{
		al_draw_rotated_bitmap(
			sprite->getBitmap(),
			centerX,
			centerY,
			x,
			y,
			radAngle,
			flags);
	}

	void GraphicsContext::drawTintedRotatedSprite( Sprite* sprite, const Color& tint, float centerX, float centerY, float x, float y, float radAngle, int flags )
	{
		al_draw_tinted_rotated_bitmap(
			sprite->getBitmap(),
			tint.getColor(),
			centerX,
			centerY,
			x,
			y,
			radAngle,
			flags);
	}

	void GraphicsContext::drawScaledRotatedSprite( Sprite* sprite, float centerX, float centerY, float x, float y, float xScale, float yScale, float radAngle, int flags )
	{
		al_draw_scaled_rotated_bitmap(
			sprite->getBitmap(),
			centerX,
			centerY,
			x,
			y,
			xScale,
			yScale,
			radAngle,
			flags);
	}

	void GraphicsContext::drawTintedScaledRotatedSprite( Sprite* sprite, const Color& tint, float centerX, float centerY, float x, float y, float xScale, float yScale, float radAngle, int flags )
	{
		al_draw_tinted_scaled_rotated_bitmap(
			sprite->getBitmap(),
			tint.getColor(),
			centerX,
			centerY,
			x,
			y,
			xScale,
			yScale,
			radAngle,
			flags);
	}

	void GraphicsContext::drawScaledSprite( Sprite *sprite, float sourceX, float sourceY, float sourceWidth, float sourceHeight, float x, float y, float destinationWidth, float destinationHeight, int flags )
	{
		al_draw_scaled_bitmap(
			sprite->getBitmap(),
			sourceX,
			sourceY,
			sourceWidth,
			sourceHeight,
			x,
			y,
			destinationWidth,
			destinationHeight,
			flags
			);
	}

	void GraphicsContext::clear()
	{
		al_clear_to_color(black);
	}

	void GraphicsContext::clear( const Color& color )
	{
		al_clear_to_color(color.getColor());
	}

	void GraphicsContext::clearTransparent()
	{
		al_clear_to_color(al_map_rgba(0,0,0,0));
	}

	void GraphicsContext::setTarget( Sprite* sprite )
	{
		al_set_target_bitmap(sprite->getBitmap());
	}

	void GraphicsContext::setTargetToBackbuffer()
	{
		al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
	}

	void GraphicsContext::holdDrawing()
	{
		al_hold_bitmap_drawing(true);
	}

	void GraphicsContext::unholdDrawing()
	{
		al_hold_bitmap_drawing(false);
	}

	void GraphicsContext::flipDisplay()
	{
			m_needsFlip = true;
		
	}

	void GraphicsContext::setClippingRect( int x, int y, int w, int h )
	{
		al_set_clipping_rectangle(x,y,w,h);
	}

	void GraphicsContext::getClippingRect( int&x,int&y,int&w,int&h )
	{
		al_get_clipping_rectangle(&x,&y,&w,&h);
	}

	void GraphicsContext::drawTintedScaledSprite( Sprite *sprite,const Color & color, float sourceX, float sourceY, float sourceWidth, float sourceHeight, float x, float y, float destinationWidth, float destinationHeight, int flags )
	{
		al_draw_tinted_scaled_rotated_bitmap(sprite->getBitmap(),
			color.getColor(),sourceWidth / 2, sourceHeight / 2,x + (destinationWidth / 2),y + (destinationHeight / 2),destinationWidth / (float)sourceWidth,destinationHeight / (float)sourceHeight,0.0f,0);
	}

	void GraphicsContext::drawText( 
		const std::string& text,Font* font,const Color& color, int x, int y, int flags )
	{
		al_draw_text(font->getFont(),
			color.getColor(),
			x,y,flags,text.c_str());
	}

	void GraphicsContext::drawText( 
		const std::string& text,agui::Font* font,const agui::Color& color, int x, int y, int flags )
	{
		al_draw_text(((agui::Allegro5Font*)font)->getFont(),
			al_map_rgba_f(color.getR(),color.getG(),color.getB(),color.getA()),
			x,y,flags,text.c_str());
	}

	void GraphicsContext::drawFilledRectangle( int x, int y, int w, int h,const agui::Color& color )
	{
		al_draw_filled_rectangle(x,y,x + w,y + h,al_map_rgba_f(color.getR(),color.getG(),color.getB(),color.getA()));
	}

	void GraphicsContext::setShader( Shader* shader )
	{
		m_shader = shader;
	}

	void GraphicsContext::resetTransform()
	{
		ALLEGRO_TRANSFORM t;
		al_identity_transform(&t);
		al_use_transform(&t);
	}

	void GraphicsContext::useTransform( Transformation& t )
	{
		al_use_transform(&t.getTransform());
	}

	int GraphicsContext::drawRotatedText( const std::string& text,agui::Font* font,const agui::Color& color, int x, int y, float deg, int flags )
	{
		int fx = font->getTextWidth(text) / 2;
		int fy = font->getLineHeight() / 2;
		m_transform.identity();
		m_transform.translate(-fx,-fy);
		m_transform.rotate(deg);
		m_transform.translate(x,y);
		useTransform(m_transform);
		drawText(text,font,color,0,0,flags);
		resetTransform();
		return fx * 2;
	}

	void GraphicsContext::begin()
	{
		if(isBuffering())
		{
			resetTransform();
		}
		
	}

	void GraphicsContext::resizeBuffer(int w, int h)
	{
		if(isBuffering())
		{
			delete m_buffer;
			m_buffer = new Sprite(w,h);
		}
	}

	bool GraphicsContext::isBuffering() const
	{
		return m_tripleBuffer;
	}

	void GraphicsContext::setBuffering( bool buffering )
	{
		m_tripleBuffer = buffering;
		delete m_buffer;
		m_buffer = NULL;

		if(buffering)
		{
			resizeBuffer(al_get_display_width(al_get_current_display())
				,al_get_display_height(al_get_current_display()));
		}
	}

	void GraphicsContext::end()
	{
		if(m_needsFlip)
		{
			m_needsFlip = false;
			al_flip_display();
		}
		
	}

	void GraphicsContext::setTargetToBuffer()
	{
		setTarget(m_buffer);
	}

	Sprite* GraphicsContext::getBuffer()
	{
		return m_buffer;
	}

}
