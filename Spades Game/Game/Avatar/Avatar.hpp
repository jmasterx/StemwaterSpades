#ifndef CGE_AVATAR_HPP
#define CGE_AVATAR_HPP
#include "Game/Avatar/ArcText.hpp"
#include "Game/Avatar/AvatarComposite.hpp"
#include "Game/UI/GuiFontManager.hpp"
#include <Agui/Graphics.hpp>
namespace cge
{
	struct ClientPlayerStats
	{
		int m_playerBid;
		int m_playerPoints;

		ClientPlayerStats()
			: m_playerBid(-1),m_playerPoints(0)
		{

		}
	};

	struct AvatarGlow 
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

		AvatarGlow()
			: peakR(0.0f),peakG(0.0f),peakB(0.0f),
			curR(0.0f),curG(0.0f),curB(0.0f),
			minR(0.0f),minG(0.0f),minB(0.0f),
			active(false),time(0.0f),increasing(true)

		{

		}
	};

	struct AvatarEffect
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
		float time;
		bool increasing;
		int numAlterationsCompleted;

		int numAlterations;
		float speed;
		float stickTime;

		AvatarEffect()
			: peakR(0.0f),peakG(0.0f),peakB(0.0f),
			curR(0.0f),curG(0.0f),curB(0.0f),
			minR(0.0f),minG(0.0f),minB(0.0f),
			time(0.0f),increasing(true),
			numAlterations(0),speed(0.0f),stickTime(0.0f),
			numAlterationsCompleted(0)

		{

		}
	};

	class Avatar
	{
		ArcText m_text;
		AvatarComposite m_composite;
		GuiFontManager* m_fontMan;
		float m_scale;
		ClientPlayerStats m_playerStats;
		std::string m_bidText;
		void solveTextSize();
		AvatarGlow m_glow;
		bool m_effectActive;
		AvatarEffect m_effect;
		float m_nameX;
		float m_nameY;
		agui::Image* m_nameCap;
		agui::Font* m_nameFont;
		float m_stallTime;
		Sprite* m_stallImg[3];
		agui::Font* m_stallFont;
		float m_stallX;
		float m_stallY;
		float m_stallWidth;
		int m_stallStrikes;
	public:
		Avatar(void);
		ArcText& getText();
		AvatarComposite& getComposite();
		void setText(const std::string& text);
		void setTextArcStrength(float strength);
		void setRoatation(int rotation);
		void render(GraphicsContext* g);
		void setFontManager(GuiFontManager* font);
		void setScale(float scale);
		void setBlurShaders(Shader* hBlur, Shader* vBlur);
		void clear();
		float getWidth() const;
		float getHeight() const;

		void setX(int x);
		void setY(int y);
		ClientPlayerStats& getPlayerStats();
		void setBidText(const std::string& text);
		const std::string& getBidText() const;
		AvatarGlow& getGlow();
		void setEffect(const AvatarEffect& effect);
		AvatarEffect& getEffect();
		void setEffectActive(bool active);
		bool isEffectActive() const;
		void setNamePos(float x, float y);
		void setNameFont(agui::Font* f);
		void setNameCapImage(agui::Image* img);
		void renderName(agui::Graphics* g);
		void renderTime(GraphicsContext* g);
		void setStallTime(int t);
		void setStallFont(agui::Font* f);
		void setStallWidth(float w);
		void setStallImage(Sprite* first,Sprite* second, Sprite* third);
		void setStallPos(float x, float y);
		void updateTime(double delta);
		void setStallStrikes(int strikes);
		agui::Font* getNameFont();
		virtual ~Avatar(void);
	};
}

#endif
