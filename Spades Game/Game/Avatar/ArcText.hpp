#ifndef CGE_ARC_TEXT_HPP
#define CGE_ARC_TEXT_HPP
#include "Game/Engine/GraphicsContext.hpp"
#include "Game/Utility/Vec2.hpp"
#include "Game/Utility/Math.hpp"
#include "Game/UI/Button.hpp"
namespace cge
{
	class ArcText
	{
		agui::Font* m_font;
		agui::Color m_color;
		agui::UTF8 m_unicode;
		double m_strength;
		double m_x;
		double m_y;
		std::vector<std::pair<std::string,Vec2> > m_letters;
		Button m_widget;
		std::string m_text;
		int m_direction;
		int m_shadowStrength;
		float m_shadowDirX;
		float m_shadowDirY;
	public:
		ArcText(void);
		void setPos(double x, double y);
		double getX() const;
		double getY() const;
		agui::Font* getFont();
		void setStrength(double s);
		double getArcTotal(double radius);
		void setText(const std::string& text);
		void positionText();
		void positionAndSizeWidget(int x, int y, int w, int h);
		void setFont(agui::Font* font);
		void setFontColor(const agui::Color& color);
		void render(GraphicsContext* g);
		void setDirection(int direction);
		int getDirection() const;
		agui::Widget* getWidget();
		std::string getText();
		void setShadowStrength(int strength);
		int getShadowStrength() const;
		void setShadowDir(float x, float y);
		virtual ~ArcText(void);
	};
}

#endif

