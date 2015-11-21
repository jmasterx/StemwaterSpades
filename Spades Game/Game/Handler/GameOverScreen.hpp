#ifndef CGE_GAME_OVER_SCREEN_HPP
#define CGE_GAME_OVER_SCREEN_HPP
#include "Game/Element/DynamicElement.hpp"
#include "Game/UI/OrbButton.hpp"
#include "Game/UI/GuiImageManager.hpp"
#include "Game/UI/GuiFontManager.hpp"
#include "Agui/Widget.hpp"
#include "Game/Handler/GameEventProvider.hpp"
#include "Game/Engine/LanguageManager.hpp"
#include "Agui/ActionListener.hpp"
#include "Game/Particle/GameOverAPS.hpp"
namespace cge
{
	class GameOverScreen : public agui::Widget, 
		public agui::ActionListener,
		public DynamicElement, 
		public GameEventProvider
	{
	public:
		struct Team
		{
			std::string m_firstName;
			std::string m_secondName;
			std::string m_firstRating;
			std::string m_secondRating;
			std::string m_score;
			int m_teamNo;
			bool m_isSocial;
			std::string m_coinNetChange;
			Team()
				: m_teamNo(0), m_isSocial(false)
			{

			}
		};
	private:
		double m_timeStamp;
		int m_seconds;
		int m_curSecond;
		agui::Image* m_spadeRed;
		agui::Image* m_spadeBlue;
		agui::Image* m_spadeGreen;
		agui::Image* m_spadeYellow;
		agui::Image* m_coinImg;
		OrbButton* m_leftButton;
		OrbButton* m_rightButton;
		GuiImageManager* m_imgMan;
		GuiFontManager* m_fontMan;
		LanguageManager* m_language;
		std::vector<Team> m_teams;
		agui::Image* m_imgs[4];
		agui::Font* m_nameFontA;
		agui::Font* m_nameFontB;
		agui::Font* m_posFontA;
		agui::Font* m_posFontB;
		agui::Font* m_subPosFontA;
		agui::Font* m_subPosFontB;
		agui::Font* m_ratingFontA;
		agui::Font* m_ratingFontB;
		agui::Font* m_scoreFontA;
		agui::Font* m_scoreFontB;
		agui::Font* m_moneyFontA;
		agui::Font* m_moneyFontB;
		agui::Font* m_coinsFontA;
		agui::Font* m_coinsFontB;
		agui::Font* m_overFont;
		std::string m_strPoses[4];
		std::string m_strPosEnds[4];
		std::string m_newRating;
		std::string m_tempRating;
		std::string m_scoreTxt;
		std::string m_tempScore;
		std::string m_playAgain;
		std::string m_gOverStrYou;
		std::string m_gOverStrWon;
		std::string m_gOverStrLost;
		std::string m_gOverStrGame;
		std::string m_gOverStrOver;
		std::string m_gOverStrCoins;
		std::string m_gOverStrA;
		std::string m_gOverStrB;
		bool m_watch;
		bool m_won;
		GraphicsContext* m_tempGraphics;
		GameOverAPS m_particles;
	public:
		GameOverScreen(	OrbButton* leftButton,
		OrbButton* rightButton,
		GuiImageManager* imageMan,
		GuiFontManager* fontMan,
		LanguageManager* language,
		SpriteManager* spriteMan,
		AudioManager* audioMan);
		virtual void actionPerformed(const agui::ActionEvent& evt);
		virtual void render(GraphicsContext* g);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual void resize(int w, int h);
		void addTeam(const Team& team);
		void clearTeams();
		float getSpadeScale(int teamNo) const;
		int getSpadeWidth(int team) const;
		int getSpadeHeight(int team) const;
		int scaleSpadeMargin(int originalMargin, int teamNo) const;
		int getActualSpadeWidth(int teamNo) const;
		int getActualSpadeHeight(int teamNo) const;
		int getSpadeCenterX(int team) const;
		int getSpadeCenterY(int team) const;
		int getSpadePadding(int team) const;
		agui::Font* getNameFont(int team);
		agui::Font* getNumberFont(int team);
		agui::Font* getSubNumberFont(int team);
		agui::Font* getRatingFont(int team);
		agui::Font* getScoreFont(int team);
		agui::Font* getMoneyFont(int team);
		agui::Font* getCoinsFont(int team);
		void showScreen(int seconds, bool showButtons);
		virtual void logic(double timeElapsed);
		void changeTime(int newTime);
		virtual void hideScreen(bool playAgain);
		void setHasWon(bool won, bool watching);
		virtual void mouseClick(agui::MouseEvent &mouseEvent);
		virtual void setProportions(ProportionsManager* manager);
		virtual ~GameOverScreen(void);
	};
}

#endif
