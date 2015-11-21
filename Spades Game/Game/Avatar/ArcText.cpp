#include "Game/Avatar/ArcText.hpp"
#include <iostream>
#include <Agui/EmptyWidget.hpp>
namespace cge
{
	ArcText::ArcText(void)
		:m_font(NULL),m_color(255,255,255),
		m_strength(50.0f),m_x(0.0f),m_y(0.0f),
		m_direction(2),m_shadowStrength(35),
		m_shadowDirX(0.12f),m_shadowDirY(0.12f),
		m_widget(NULL,NULL,NULL,NULL,NULL,NULL)
	{
		m_widget.setEnterCursor(agui::CursorProvider::LINK_CURSOR);
	}

	ArcText::~ArcText(void)
	{
	}

	void ArcText::render( GraphicsContext* g )
	{
		float vecX = m_shadowDirX;
		float vecY = m_shadowDirY;

		if(m_direction == 1)
			vecX = -vecX;

		for(int i = 0; i < m_letters.size(); ++i)
		{
			g->drawRotatedText(m_letters[i].first,m_font,agui::Color(0,0,0,m_shadowStrength),
				m_letters[i].second.getX() + m_x + (m_font->getLineHeight() * vecX),m_letters[i].second.getY() + m_y + (m_font->getLineHeight() * vecY),
				m_letters[i].second.getAngle(),0);
		}

		for(int i = 0; i < m_letters.size(); ++i)
		{
			g->drawRotatedText(m_letters[i].first,m_font,m_color,
				m_letters[i].second.getX() + m_x,m_letters[i].second.getY() + m_y,
				m_letters[i].second.getAngle(),0);
		}
	}

	void ArcText::setFont( agui::Font* font )
	{
		m_font = font;
		positionText();
	}

	void ArcText::setFontColor( const agui::Color& color )
	{
		m_color = color;
	}

	void ArcText::setText( const std::string& text )
	{
		m_letters.clear();
		int len = m_unicode.length(text);
		for(int i = 0; i < len; ++i)
		{
			m_letters.push_back(std::make_pair(m_unicode.subStr(text,i,1),Vec2()));
		}

		positionText();
		m_text = text;

		m_widget.setEnabled(m_text.length() > 0);
	}

	void ArcText::positionText()
	{
		if(m_font == NULL || m_letters.empty())
		{
			return;
		}

		double scale = 1.0f;
		double dist = m_font->getLineHeight() * 0.05 * m_strength;

		if(m_direction == 0 || m_direction == 3)
		dist = -dist;

		Vec2 center = Vec2(0,0);
		double angOffset = PI;

		if(m_direction == 1 || m_direction == 3)
			angOffset -= (PI / 2.0f);

		double scalar = 0.385f;
		Vec2 pos;
		double ang = 0.0f;
		double totalAng = getArcTotal(dist) * 1.16f;
		double angDist = 0.0f;
		if(m_direction == 1)
		{
			for(int i = m_letters.size() - 1; i >= 0; --i)
			{
				angDist += Math::angleFromWidth(dist,m_font->getTextWidth(m_letters[i].first)) * scalar;
				pos = Math::positionFromCenterToLineAt(center,dist,totalAng,angDist,angOffset);
				angDist += Math::angleFromWidth(dist,m_font->getTextWidth(m_letters[i].first)) * scalar;	
				ang = Math::angleFromVector(center,pos);

				if(m_direction != 0 && m_direction != 3 && m_direction != 1)
					ang += PI;

				pos.setAngle(ang);

				if(m_direction == 0 || m_direction == 2)
					pos.setY(pos.getY() - dist);
				else
					pos.setX(pos.getX() - dist);

				m_letters[i].second = pos;
			}
		}
		else
		{
			for(int i = 0; i < m_letters.size(); ++i)
			{
				angDist += Math::angleFromWidth(dist,m_font->getTextWidth(m_letters[i].first)) * scalar;
				pos = Math::positionFromCenterToLineAt(center,dist,totalAng,angDist,angOffset);
				angDist += Math::angleFromWidth(dist,m_font->getTextWidth(m_letters[i].first)) * scalar;	
				ang = Math::angleFromVector(center,pos);

				if(m_direction != 0 && m_direction != 3)
					ang += PI;

				pos.setAngle(ang);

				if(m_direction == 0 || m_direction == 2)
					pos.setY(pos.getY() - dist);
				else
					pos.setX(pos.getX() - dist);

				m_letters[i].second = pos;
			}
		}


		//positionAndSizeWidget();
	}

	double ArcText::getArcTotal( double radius )
	{
		double total = 0.0f;
		for(size_t i = 0; i < m_letters.size(); ++i)
		{
			total += Math::angleFromWidth(radius,m_font->getTextWidth(m_letters[i].first));
		}

		return total;
	}

	void ArcText::setPos( double x, double y )
	{
		m_x = x;
		m_y = y;
		//positionAndSizeWidget();
	}

	void ArcText::setStrength( double s )
	{
		m_strength = s;
		positionText();
	}

	double ArcText::getX() const
	{
		return m_x;
	}

	double ArcText::getY() const
	{
		return m_y;
	}

	agui::Font* ArcText::getFont()
	{
		return m_font;
	}

	agui::Widget* ArcText::getWidget()
	{
		return &m_widget;
	}

	void ArcText::positionAndSizeWidget(int x, int y, int w, int h)
	{
		int minX = 9999;
		int minY = 9999;
		int maxX = -9999;
		int maxY = -9999;

		int sizeX = 0;
		int sizeY = 0;

		m_widget.setLocation(x,y);
		m_widget.setSize(w,h);

		/*
		if(m_letters.size() == 0)
		{
			m_widget.setLocation(0,0);
			m_widget.setSize(0,0);
		}
		else
		{
			for(int i = 0; i < m_letters.size(); ++i)
			{
				int letterMinX = m_letters[i].second.getX() - (m_font->getTextWidth(m_letters[i].first));
				int letterMinY = m_letters[i].second.getY() - (m_font->getLineHeight());
				int letterMaxX = m_letters[i].second.getX() + (m_font->getTextWidth(m_letters[i].first));
				int letterMaxY = m_letters[i].second.getY() + (m_font->getLineHeight());

				if(letterMinX < minX)
				{
					minX = letterMinX;
				}

				if(letterMinY < minY)
				{
					minY = letterMinY;
				}

				if(letterMaxX > maxX)
				{
					maxX = letterMaxX;
				}

				if(letterMaxY > maxY)
				{
					maxY = letterMaxY;
				}
			}

			sizeX = maxX - minX;
			sizeY = maxY - minY;

			m_widget.setLocation(minX + m_x,minY + m_y);
			m_widget.setSize(sizeX,sizeY);
		

		}
		*/
	}

	void ArcText::setDirection( int direction )
	{
		if(direction >= 0 && direction < 4 )
		{
			m_direction = direction;
			positionText();
		}
	}

	int ArcText::getDirection() const
	{
		return m_direction;
	}

	std::string ArcText::getText()
	{
		return m_text;
	}

	void ArcText::setShadowStrength( int strength )
	{
		m_shadowStrength = strength;
	}

	int ArcText::getShadowStrength() const
	{
		return m_shadowStrength;
	}

	void ArcText::setShadowDir( float x, float y )
	{
		m_shadowDirX = x;
		m_shadowDirY = y;
	}

}
