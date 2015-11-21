#include "Game/Handler/GameOverScreen.hpp"
#include <sstream>

namespace cge
{
	GameOverScreen::~GameOverScreen(void)
	{
		m_leftButton->removeActionListener(this);
		m_leftButton->removeActionListener(this);
	}

	GameOverScreen::GameOverScreen( OrbButton* leftButton, OrbButton* rightButton, GuiImageManager* imageMan,
		GuiFontManager* fontMan, LanguageManager* language, SpriteManager* spriteMan, AudioManager* audioMan )
		: m_leftButton(leftButton),m_rightButton(rightButton),
		m_imgMan(imageMan),m_fontMan(fontMan),
		m_language(language),m_spadeRed(NULL),
		m_spadeBlue(NULL),m_spadeYellow(NULL),m_spadeGreen(NULL),
		m_nameFontA(NULL),
		m_nameFontB(NULL),
		m_posFontA(NULL),
		m_posFontB(NULL),
		m_subPosFontA(NULL),
		m_subPosFontB(NULL),
		m_ratingFontA(NULL),
		m_ratingFontB(NULL),
		m_scoreFontA(NULL),
		m_scoreFontB(NULL),
		m_moneyFontA(NULL),
		m_moneyFontB(NULL),
		m_coinsFontA(NULL),
		m_coinsFontB(NULL),
		m_coinImg(NULL),
		m_timeStamp(0.0),
		m_overFont(NULL),
		m_seconds(0),
		m_curSecond(0),
		m_watch(false),
		m_particles(spriteMan,audioMan),
		m_tempGraphics(NULL)

	{
		setVisibility(false);

		m_spadeRed = m_imgMan->getImage("game.spade.red");	    m_imgs[0] = m_spadeRed;	
		m_spadeBlue = m_imgMan->getImage("game.spade.blue");    m_imgs[1] = m_spadeBlue;	
		m_spadeGreen = m_imgMan->getImage("game.spade.green");  m_imgs[2] = m_spadeGreen;	
		m_spadeYellow = m_imgMan->getImage("game.spade.yellow");m_imgs[3] = m_spadeYellow;	

		m_coinImg = m_imgMan->getImage("coin");
		add(m_leftButton);
		add(m_rightButton);
		m_leftButton->setText(m_language->getElement("over.leave"));
		m_rightButton->setMultiline(true);
		m_playAgain = m_language->getElement("over.play");
		m_rightButton->setText(m_playAgain);

		for(int i = 0; i < 4; ++i)
		{
			std::stringstream ss;
			ss << i + 1;
			m_strPoses[i] = ss.str();
		}

		m_strPosEnds[0] = m_language->getElement("first.place");
		m_strPosEnds[1] = m_language->getElement("second.place");
		m_strPosEnds[2] = m_language->getElement("third.place");
		m_strPosEnds[3] = m_language->getElement("fourth.place");
		m_newRating = m_language->getElement("new.rating") + ": ";
		m_scoreTxt = m_language->getElement("over.score") + ": ";

		m_gOverStrYou = m_language->getElement("over.you");
		m_gOverStrWon = m_language->getElement("over.won");
		m_gOverStrLost = m_language->getElement("over.lost");
		m_gOverStrGame = m_language->getElement("over.game");
		m_gOverStrOver = m_language->getElement("over.over");
		m_gOverStrCoins = m_language->getElement("coins.won");

		m_gOverStrA = m_gOverStrGame;
		m_gOverStrB = m_gOverStrOver;

		m_rightButton->addActionListener(this);
		m_leftButton->addActionListener(this);
	}
	void GameOverScreen::render( GraphicsContext* g )
	{
		m_tempGraphics = g;
	}

	void GameOverScreen::paintComponent( const agui::PaintEvent &paintEvent )
	{

	}

	void GameOverScreen::paintBackground( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawFilledRectangle(getSizeRectangle(),agui::Color(0,0,0,100));
		if(m_nameFontA == NULL)
		{
			return;
		}

		//oh my is this ever evil
		if(m_tempGraphics)
		{
			m_particles.render(*m_tempGraphics);
		}

		int overX2 = 0;

		{
			int overX = (((getSpadeCenterX(0)) - m_overFont->getTextWidth(m_gOverStrA)) / 2);
			overX2 = (getWidth() / 2) + ((getWidth() / 2) - (overX + m_overFont->getTextWidth(m_gOverStrA)));
			int overY = ((getHeight() / 2) - m_overFont->getLineHeight()) / 2;
			paintEvent.graphics()->drawText(agui::Point(overX,overY),
				m_gOverStrA.c_str(),agui::Color(255,255,255),m_overFont);
		}

		{
			//int overX = getSpadeCenterX(0) + getSpadeWidth(0) + ((getSpadeCenterX(0) - m_overFont->getTextWidth(m_gOverStrB)) / 2);
			int overX = overX2;
			int overY = ((getHeight() / 2) - m_overFont->getLineHeight()) / 2;
			paintEvent.graphics()->drawText(agui::Point(overX,overY),
				m_gOverStrB.c_str(),agui::Color(255,255,255),m_overFont);
		}

		for(int i = 0; i < m_teams.size(); ++i)
		{
			int posX = getSpadeCenterX(i);
			int posY = getSpadeCenterY(i);
			int w = getSpadeWidth(i);
			int h = getSpadeHeight(i);
			int aw = getActualSpadeWidth(i);
			int ah = getActualSpadeHeight(i);

			int offX = scaleSpadeMargin(m_spadeRed->getMargin(agui::SIDE_LEFT),i);
			int offY = scaleSpadeMargin(m_spadeRed->getMargin(agui::SIDE_LEFT),i);

			int placeW = getNumberFont(i)->getTextWidth(m_strPoses[i]);
			int subPlaceW = getSubNumberFont(i)->getTextWidth(m_strPosEnds[i]);
			int totalPlaceW = placeW + subPlaceW;
			int placeStartRelX = (aw - totalPlaceW) / 2;
			int placeStartX = offX + posX + placeStartRelX;

			paintEvent.graphics()->drawScaledImage(m_imgs[m_teams[i].m_teamNo],
				agui::Point(posX,posY),agui::Point(),
				agui::Dimension(m_spadeRed->getWidth(),m_spadeRed->getHeight()),
				agui::Dimension(w,h));

			agui::Color c = agui::Color(255,255,255);
		
			paintEvent.graphics()->drawText(agui::Point(placeStartX,posY + offY + (ah * 0.14f)),
				m_strPoses[i].c_str(),c,getNumberFont(i));
			paintEvent.graphics()->drawText(agui::Point(placeStartX + placeW,posY + offY + (ah * 0.14f)),
				m_strPosEnds[i].c_str(),c,getSubNumberFont(i));

			float scoreOffset = 0.273f;

			if(m_teams.size() > 2)
			{
				scoreOffset += 0.05f;
			}

			if(m_teams[i].m_firstName.length() > 0)
			{
				m_tempScore = m_scoreTxt + m_teams[i].m_score;
				int nameWidth = getScoreFont(i)->getTextWidth(m_tempScore);
				int totalWidth = nameWidth;
				int nameStartRelX = (aw - totalWidth) / 2;
				int nameStartX = offX + posX + nameStartRelX;
				paintEvent.graphics()->drawText(agui::Point(nameStartX,posY + offY + 
					(ah * scoreOffset)),
					m_tempScore.c_str(),agui::Color(255,255,255),getScoreFont(i));
			}

			float firstNameOffset = scoreOffset + 0.08f;
			if(m_teams[i].m_firstName.length() > 0)
			{
				int nameWidth = getNameFont(i)->getTextWidth(m_teams[i].m_firstName);
				int totalWidth = nameWidth;
				int nameStartRelX = (aw - totalWidth) / 2;
				int nameStartX = offX + posX + nameStartRelX;
				paintEvent.graphics()->drawText(agui::Point(nameStartX,posY + offY + 
					(ah * firstNameOffset)),
					m_teams[i].m_firstName.c_str(),agui::Color(255,255,255),getNameFont(i));
			}
			
			if(m_teams[i].m_firstName.length() > 0 && !m_teams[i].m_isSocial)
			{
				m_tempRating = m_newRating + m_teams[i].m_firstRating;
				int nameWidth = getRatingFont(i)->getTextWidth(m_tempRating);
				int totalWidth = nameWidth;
				int nameStartRelX = (aw - totalWidth) / 2;
				int nameStartX = offX + posX + nameStartRelX;
				paintEvent.graphics()->drawText(agui::Point(nameStartX,posY + offY + 
					(ah * firstNameOffset ) + getNameFont(i)->getLineHeight()),
					m_tempRating.c_str(),agui::Color(255,255,255),getRatingFont(i));
			}
			

			float secondNameOffset = firstNameOffset + 0.03f;
			int extraH = getNameFont(i)->getLineHeight() + getRatingFont(i)->getLineHeight();
			//secondNameOffset *= 1.1f;
			//int extraH = 0;

			if(m_teams[i].m_isSocial)
			{
				extraH = getNameFont(i)->getLineHeight();
			}

			
			if(m_teams[i].m_secondName.length() > 0)
			{
				int nameWidth = getNameFont(i)->getTextWidth(m_teams[i].m_secondName);
				int totalWidth = nameWidth;
				int nameStartRelX = (aw - totalWidth) / 2;
				int nameStartX = offX + posX + nameStartRelX;
				paintEvent.graphics()->drawText(agui::Point(nameStartX,posY + offY + 
					(ah * secondNameOffset) + extraH),
					m_teams[i].m_secondName.c_str(),agui::Color(255,255,255),getNameFont(i));
			}

			if(m_teams[i].m_secondName.length() > 0 && !m_teams[i].m_isSocial)
			{
				m_tempRating = m_newRating + m_teams[i].m_secondRating;
				int nameWidth = getRatingFont(i)->getTextWidth(m_tempRating);
				int totalWidth = nameWidth;
				int nameStartRelX = (aw - totalWidth) / 2;
				int nameStartX = offX + posX + nameStartRelX;
				paintEvent.graphics()->drawText(agui::Point(nameStartX,posY + offY + 
					(ah * secondNameOffset ) + extraH + getNameFont(i)->getLineHeight()),
					m_tempRating.c_str(),agui::Color(255,255,255),getRatingFont(i));
			}

			if(m_teams[i].m_coinNetChange.length() > 0)
			{
				int coinW = getSpadeScale(i) * 30;

				int totalWidth = coinW;
				int nameStartRelX = (aw - totalWidth) / 2;
				int nameStartX = offX + posX + nameStartRelX + (aw * 0.32f);

				int nameStartRelY = (ah - totalWidth) / 2;
				int nameStartY = offY + posY + nameStartRelY + (ah * 0.17f);

				agui::Font* moneyFont = getMoneyFont(i);
				agui::Font* coinFont = getCoinsFont(i);
				int textWidth = moneyFont->getTextWidth(m_teams[i].m_coinNetChange) / 2;
				
				paintEvent.graphics()->drawScaledImage(m_coinImg,agui::Point(nameStartX,nameStartY - coinFont->getLineHeight()),
					agui::Point(),agui::Dimension(m_coinImg->getWidth(),m_coinImg->getHeight()),agui::Dimension(coinW,coinW));

				paintEvent.graphics()->drawText(agui::Point(nameStartX - textWidth + (coinW / 2),nameStartY + coinW),m_teams[i].m_coinNetChange.c_str(),
					agui::Color(255,255,255),moneyFont);
				paintEvent.graphics()->drawText(agui::Point(nameStartX - (coinFont->getTextWidth(m_gOverStrCoins) / 2) + (coinW / 2),nameStartY + coinW + (coinW * 0.05f) - coinFont->getLineHeight()),m_gOverStrCoins.c_str(),
					agui::Color(255,255,255),coinFont);
			}

		}
	}

	void GameOverScreen::resize( int w, int h )
	{
		setSize(w,h);
		int sz = 400 * getProportions()->getScale();
		m_leftButton->setFont(m_fontMan->getFont(getProportions()->getScale() * 2.2f));
		m_rightButton->setFont(m_fontMan->getFont(getProportions()->getScale() * 2.2f));
		m_leftButton->setSize(sz,sz);
		m_rightButton->setSize(sz,sz);
		m_leftButton->alignToParent(agui::ALIGN_MIDDLE_LEFT);
		m_rightButton->alignToParent(agui::ALIGN_MIDDLE_RIGHT);

		m_nameFontA = m_fontMan->getFont(getSpadeScale(0) * 1.33f);
		m_nameFontB = m_fontMan->getFont(getSpadeScale(1) * 1.33f);
		m_scoreFontA = m_fontMan->getFont(m_nameFontA,1.15f);
		m_scoreFontB = m_fontMan->getFont(m_nameFontB,1.15f);

		m_ratingFontA = m_fontMan->getFont(m_nameFontA,0.85f);
		m_ratingFontB = m_fontMan->getFont(m_nameFontB,0.85f);
		m_posFontA  = m_fontMan->getFont(getSpadeScale(0) * 2.5f);
		m_posFontB  = m_fontMan->getFont(getSpadeScale(1) * 2.5f);
		m_subPosFontA = m_fontMan->getFont(m_posFontA,0.75f);
		m_subPosFontB = m_fontMan->getFont(m_posFontB,0.75f);

		m_overFont = m_fontMan->getPrettyFont(8.0f);

		m_coinsFontA = m_fontMan->getPrettyFont(getSpadeScale(0) * 1.05f);
		m_coinsFontB = m_fontMan->getPrettyFont(getSpadeScale(1) * 1.05f);

		m_moneyFontA = m_fontMan->getFont(getSpadeScale(0) * 1.0f);
		m_moneyFontB = m_fontMan->getFont(getSpadeScale(1) * 1.0f);
		m_particles.resize(w,h);
	}

	void GameOverScreen::addTeam( const Team& team )
	{
		m_teams.push_back(team);
	}

	void GameOverScreen::clearTeams()
	{
		m_teams.clear();
	}

	float GameOverScreen::getSpadeScale( int teamNo ) const
	{
		return getSpadeHeight(teamNo) / (float)m_spadeRed->getHeight();
	}

	int GameOverScreen::getSpadeWidth(int team) const
	{
		int h = getSpadeHeight(team);
		float r = h / (float)m_spadeRed->getHeight();
		return r * m_spadeRed->getWidth();
	}

	int GameOverScreen::getSpadeHeight( int team ) const
	{
		if(team == 0)
		return getHeight() * 0.54f;
		else
		return getHeight() * 0.44f;
	}

	int GameOverScreen::scaleSpadeMargin( int originalMargin, int teamNo ) const
	{
		return originalMargin * getSpadeScale(teamNo);
	}

	int GameOverScreen::getActualSpadeWidth( int teamNo ) const
	{
		return getSpadeWidth(teamNo) -
			scaleSpadeMargin(m_spadeRed->getMargin(agui::SIDE_LEFT),teamNo) -
			scaleSpadeMargin(m_spadeRed->getMargin(agui::SIDE_RIGHT),teamNo);
	}

	int GameOverScreen::getActualSpadeHeight( int teamNo ) const
	{
		return getSpadeHeight(teamNo) -
			scaleSpadeMargin(m_spadeRed->getMargin(agui::SIDE_TOP),teamNo) -
			scaleSpadeMargin(m_spadeRed->getMargin(agui::SIDE_BOTTOM),teamNo);
	}

	int GameOverScreen::getSpadeCenterX( int team ) const
	{
		if(team == 0)
		{
			int offX = ((getWidth() - getActualSpadeWidth(team)) / 2) - 
				scaleSpadeMargin(m_spadeRed->getMargin(agui::SIDE_LEFT),team);
			return offX;
		}
		else
		{
			int num = m_teams.size() - 1;
			if(num <= 0)
			{
				return 0;
			}

			int totalWidth = num * getActualSpadeWidth(team);
			int totalPad = getSpadePadding(team) * (num - 1);
			int totalW = totalWidth + totalPad;
			int startX = (getWidth() - totalW) / 2;
			startX -= scaleSpadeMargin(m_spadeRed->getMargin(agui::SIDE_LEFT),team);
			startX += getActualSpadeWidth(team) * (team - 1);
			startX += getSpadePadding(team) * (team - 1);
			return startX;
		}

		return 0;
	}

	int GameOverScreen::getSpadePadding( int team ) const
	{
		return 40 * getSpadeScale(team);
	}

	int GameOverScreen::getSpadeCenterY( int team ) const
	{
		if(team == 0)
		{
			int offY = (((getHeight() / 2) - getActualSpadeHeight(team)) / 2) - 
				scaleSpadeMargin(m_spadeRed->getMargin(agui::SIDE_TOP),team);
			return offY;
		}
		else
		{
			int offY = (((getHeight() / 2) - getActualSpadeHeight(team)) / 2) - 
				scaleSpadeMargin(m_spadeRed->getMargin(agui::SIDE_TOP),team);
			offY += getHeight() * 0.45;
			return offY;
		}
	}

	agui::Font* GameOverScreen::getNameFont( int team )
	{
		if(team == 0)
		{
			return m_nameFontA;
		}
		else
		{
			return m_nameFontB;
		}
	}

	agui::Font* GameOverScreen::getNumberFont( int team )
	{
		if(team == 0)
		{
			return m_posFontA;
		}
		else
		{
			return m_posFontB;
		}
	}

	agui::Font* GameOverScreen::getSubNumberFont( int team )
	{
		if(team == 0)
		{
			return m_subPosFontA;
		}
		else
		{
			return m_subPosFontB;
		}
	}

	agui::Font* GameOverScreen::getRatingFont( int team )
	{
		if(team == 0)
		{
			return m_ratingFontA;
		}
		else
		{
			return m_ratingFontB;
		}
	}

	agui::Font* GameOverScreen::getScoreFont( int team )
	{
		if(team == 0)
		{
			return m_scoreFontA;
		}
		else
		{
			return m_scoreFontB;
		}
	}

	void GameOverScreen::showScreen( int seconds, bool showButtons )
	{
		setVisibility(true);
		//m_watch = !showButtons;
		m_rightButton->setVisibility(showButtons);
		m_leftButton->setVisibility(showButtons);
		m_rightButton->setEnabled(true);
		m_timeStamp = 0.0;
		m_seconds = seconds;
		m_curSecond = seconds;
		changeTime(seconds);

		if(m_won && !m_watch)
		m_particles.start();
	}

	void GameOverScreen::logic( double timeElapsed )
	{
		if(m_curSecond > 0 && isVisible())
		{
			if(m_timeStamp == 0.0)
			{
				m_timeStamp = timeElapsed;
			}
			else
			{
				int delta = m_seconds - (timeElapsed - m_timeStamp);
				if(delta != m_curSecond)
				{
					m_curSecond = delta;
					changeTime(m_curSecond);
				}

				if(m_curSecond == 0)
				{
					m_rightButton->setEnabled(false);
					DISPATCH_GAME_EVENT
						(*it)->wantLeave();
					//hide();
				}
			}
		}

		m_particles.elemLogic(timeElapsed);
	}

	void GameOverScreen::changeTime(int newTime)
	{
		std::stringstream ss;
		ss << newTime;
		m_rightButton->setText(m_playAgain + "\n" + ss.str());
	}

	void GameOverScreen::actionPerformed( const agui::ActionEvent& evt )
	{
		if(evt.getSource() == m_leftButton)
		{
			DISPATCH_GAME_EVENT
				(*it)->wantLeave();
			hideScreen(false);
		}
		else if(evt.getSource() == m_rightButton)
		{
			DISPATCH_GAME_EVENT
				(*it)->wantPlayAgain();
			hideScreen(true);
		}
	}

	void GameOverScreen::hideScreen(bool playAgainResult)
	{
		m_particles.stop();

		if(isVisible())
		{
			hide();
			DISPATCH_GAME_EVENT
				(*it)->gameOverScreenHidden(playAgainResult);
		}
	}

	void GameOverScreen::mouseClick( agui::MouseEvent &mouseEvent )
	{
		if(m_watch)
		{
			hideScreen(false);
		}
	}

	void GameOverScreen::setHasWon( bool won, bool watching )
	{
		m_won = won;
		m_watch = watching;

		if(m_watch)
		{
			m_gOverStrA = m_gOverStrGame;
			m_gOverStrB = m_gOverStrOver;
		}
		else
		{
			m_gOverStrA = m_gOverStrYou;
			if(m_won)
			{
				m_gOverStrB = m_gOverStrWon;
			}
			else
			{
				m_gOverStrB = m_gOverStrLost;
			}
		}
	}

	void GameOverScreen::setProportions( ProportionsManager* manager )
	{
		DynamicElement::setProportions(manager);
		m_particles.setProportions(manager);
	}

	agui::Font* GameOverScreen::getMoneyFont( int team )
	{
		if(team == 0)
		{
			return m_moneyFontA;
		}
		else
		{
			return m_moneyFontB;
		}
	}

	agui::Font* GameOverScreen::getCoinsFont( int team )
	{
		if(team == 0)
		{
			return m_coinsFontA;
		}
		else
		{
			return m_coinsFontB;
		}
	}

}

