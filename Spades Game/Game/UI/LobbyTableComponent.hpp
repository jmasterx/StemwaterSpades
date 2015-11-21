#ifndef CGE_LOBBY_TABLE_COMPONENT_HPP
#define CGE_LOBBY_TABLE_COMPONENT_HPP
#include "Agui/Widgets/Button/Button.hpp"
#include "Game/Resource/Sprite.hpp"
#include "Game/Element/DynamicElement.hpp"
namespace cge
{
	struct LobbyTableGlow 
	{
		float peakR;
		float peakG;
		float peakB;
		float curR;
		float curG;
		float curB;
		float minR;
		float minG;
		float minB;
		bool active;
		float time;
		bool increasing;

		LobbyTableGlow()
			: peakR(0.0f),peakG(0.0f),peakB(0.0f),
			curR(0.0f),curG(0.0f),curB(0.0f),
			minR(0.0f),minG(0.0f),minB(0.0f),
			active(false),time(0.0f),increasing(true)

		{

		}
	};

	class LobbyTableComponent : public DynamicElement, public agui::Button
	{
		Sprite* m_tableImg;
		Sprite* m_tableShadowImg;
		agui::Font* m_subFont;
		agui::Font* m_ratedFont;
		agui::Font* m_watchFont;
		agui::Font* m_feeFont;
		agui::Font* m_ptsFont;
		std::string m_watchText;
		std::string m_subText;
		std::string m_mainText;
		std::string m_ratedText;
		std::string m_feeText;
		std::string m_ptsText;
        std::string m_infoText;
		agui::Rectangle m_textRect;
		agui::Color m_ratedColor;
		float m_darkness;
		bool m_canWatch;
		bool m_hasStarted;
		LobbyTableGlow m_glow;
		agui::Color m_shadowColor;
		int m_glowFrames;
		float m_angle;
		void calcTextRect();
	public:
		LobbyTableComponent(Sprite* tableImg, Sprite* tableShadowImg);
		LobbyTableComponent();
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual void render(GraphicsContext* g);
		void resizeToScale();
		virtual bool intersectionWithPoint(const agui::Point &p) const;
		agui::Point getCenter() const;
		void setSubFont(const agui::Font* font);
		agui::Font* getSubFont() const;
		void setWatchFont(const agui::Font* font);
		agui::Font* getWatchFont() const;
		void setFeeFont(const agui::Font* font);
		void setPtsFont(const agui::Font* font);
		agui::Font* getPtsFont() const;
		agui::Font* getFeeFont() const;
		void setRatedFont(const agui::Font* font);
		agui::Font* getRatedFont() const;
		void setSubText(const std::string& text);
		void setTypeText(const std::string& text);
		virtual void renderShadow(GraphicsContext* g);
		virtual void renderShadowWeak(GraphicsContext* g);
		const std::string& getSubText() const;
		int getTextCenter(const agui::Font* f, const std::string& text) const;
		void setDarkness(float darkness);
		void setWatchText(const std::string& text);
		void setRatedText(const std::string& text);
		void setPtsText(const std::string& text);
		void setCanWatch(bool watch);
		bool canWatch() const;
		void setStarted(bool started);
		bool isStarted() const;
		void setRatedColor(const agui::Color& color);
		virtual void elemLogic(double t);
		virtual agui::CursorProvider::CursorEnum getEnterCursor() const;
		void setFeeText(const std::string& text);
		void resetGlow();
		int getGlowFrames() const;
		int getGlowInterval() const;
		void setAngle(float angle);
        void setInfoText(const std::string& infoText);
        const std::string& getInfoText() const;
		virtual ~LobbyTableComponent(void);
	};
}

#endif
