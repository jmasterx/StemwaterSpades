#include "Game/UI/LobbyChairComponent.hpp"
namespace cge
{
	LobbyChairComponent::~LobbyChairComponent(void)
	{
	}

	LobbyChairComponent::LobbyChairComponent()
		: m_chairImg(NULL),m_chairHoverImg(NULL), m_chairNum(NULL)
	{

	}

	LobbyChairComponent::LobbyChairComponent( Sprite* chairImg, Sprite* chairHoverImg, AvatarCache* cache, int chairNumber, ClientPlayer* self )
		: m_chairImg(chairImg),m_chairHoverImg(chairHoverImg), m_playerImg(NULL),m_cache(cache),
		m_chairNum(chairNumber), m_darkness(1.0f),m_occupied(false),m_dark(false),m_locked(false),
		m_occupant(NULL),m_selfIsHere(false),m_subSeat(false),m_off(false),
		m_avatarData(NULL),m_self(self),m_playerOpacity(0.0f)
	{
		m_arcText.setFontColor(agui::Color(240,240,255));
		m_arcText.setStrength(150.0f);
		int dir = (chairNumber + 2) % 4;
		m_arcText.setDirection(dir);
		m_arcText.setShadowStrength(140);
	}

    void LobbyChairComponent::setAvatarOpacity(float opacity) {
        if(isOccupied())
        m_playerOpacity = opacity;
    }
    
	void LobbyChairComponent::paintBackground( const agui::PaintEvent &paintEvent )
	{

	}

	void LobbyChairComponent::render( GraphicsContext* g )
	{
		agui::Rectangle r = getAbsoluteRectangle();
		Sprite* img = NULL;
		if(getGui())
		img = getGui()->getWidgetUnderMouse() != this ? m_chairImg : 
			m_chairHoverImg;
		else
			img = m_chairImg;

		float ang = m_chairNum * (PI / 2.0f);
		ang -= PI / 2.0f;

		agui::Color c = agui::Color(1.0f,1.0f,1.0f);
		if(m_subSeat)
		{
			c = agui::Color(245,202,98);
		}

		float darkness = m_darkness;

		if(isOff())
		{
			darkness = 0.5f;
		}

		float scale = 0.0f;
		if(m_chairNum == 0 || m_chairNum == 2)
		{
			
			g->drawTintedScaledRotatedSprite(img,Color(darkness * c.getR(),
				darkness * c.getG(),darkness * c.getB()),img->getWidth() / 2, img->getHeight() / 2,
				r.getX() + (r.getWidth() / 2),r.getY() + (r.getHeight() / 2),
				(float)r.getWidth() / img->getHeight(),
				(float)r.getHeight() / img->getWidth(),ang,0);

			scale = (float)r.getWidth() / img->getWidth();
		}
		else
		{
			g->drawTintedScaledRotatedSprite(img,Color(darkness * c.getR(),
				darkness * c.getG(),darkness * c.getB()),img->getWidth() / 2, img->getHeight() / 2,
				r.getX() + (r.getWidth() / 2),r.getY() + (r.getHeight() / 2),
				(float)r.getWidth() / img->getWidth(),
				(float)r.getHeight() / img->getHeight(),ang,0);

			scale = (float)r.getHeight() / img->getWidth();
		}

		float factor = 1.065f;
		if(m_chairNum == 0)
		m_arcText.setPos(r.getX() + (r.getWidth() / 2),r.getY() + (r.getHeight() * factor));
		if(m_chairNum == 1)
			m_arcText.setPos(r.getX() - (r.getWidth() * (factor - 1.0f)),r.getY() + (r.getHeight() /2));
		if(m_chairNum == 2)
			m_arcText.setPos(r.getX() + (r.getWidth() / 2),r.getY() - (r.getHeight() * (factor - 1.0f)));
		if(m_chairNum == 3)
			m_arcText.setPos(r.getX() + (r.getWidth() * factor),r.getY() + (r.getHeight() /2));
	}

	void LobbyChairComponent::resizeToScale()
	{
		if(m_chairNum == 0 || m_chairNum == 2)
		{
			setSize(603 * getObjectScale(),
				747 * getObjectScale());
		}
		else
		{
			setSize(747 * getObjectScale(),
				603 * getObjectScale());
		}
	
	}

	void LobbyChairComponent::relocate( int centerX, int centerY )
	{
		int x = centerX;
		int y = centerY;

		int baseChairMovement = 150 * getObjectScale();
		int totalChairMovement = baseChairMovement;

		switch(m_chairNum)
		{
		case 0:
			x -= getWidth() / 2;
			y += totalChairMovement;
			break;
		case 1:
			x -= getWidth();
			y -= getHeight() / 2;
			x -= totalChairMovement;
			break;
		case 2:
			x -= getWidth() / 2;
			y -= getHeight();
			y -= totalChairMovement;
			break;
		case 3:
			y -= getHeight() / 2;
			x += totalChairMovement;
			break;
		}

		setLocation(x,y);
	}

	void LobbyChairComponent::setDarkness( float darkness )
	{
		m_darkness = darkness;
	}

	agui::CursorProvider::CursorEnum LobbyChairComponent::getEnterCursor() const
	{
		return agui::CursorProvider::LINK_CURSOR;
	}

	void LobbyChairComponent::setOccupied( bool occupied )
	{
		m_occupied = occupied;

		if(!occupied)
		{
			m_occupant = NULL;
			setText("");
			setSelfIsHere(false);
		}

			reassessEnabled();
	}

	bool LobbyChairComponent::isOccupied() const
	{
		return m_occupied;
	}

	bool LobbyChairComponent::isDark() const
	{
		return m_dark;
	}

	void LobbyChairComponent::setDark( bool dark )
	{
		m_dark = dark;

		reassessEnabled();
	}

	void LobbyChairComponent::setLocked( bool locked )
	{
		m_locked = locked;
		reassessEnabled();
	}

	bool LobbyChairComponent::isLocked() const
	{
		return m_locked;
	}

	void LobbyChairComponent::reassessEnabled()
	{
		setEnabled((!(isDark() || (m_self->isSitting() && !isOccupied()) || (isOccupied() && getOccupant()->isBot()) /*|| isOccupied */ || isLocked() || isOff())) || isSelfHere());
	}

	void LobbyChairComponent::setOccupant( ClientPlayer* player )
	{
		m_occupant = player;
		if(player)
		{
			if(m_cache->avatarExists(player->getAvatar()))
			{
				CachedAvatarImage img = (*m_cache->getCachedAvatars())[player->getAvatar()];
				m_playerImg = img.image;
				m_avatarData = &(*m_cache->getCachedAvatars())[player->getAvatar()];
			}
		}

		if(m_playerImg)
		{
			m_playerOpacity = 0.0f;
		}
	}

	ClientPlayer* LobbyChairComponent::getOccupant() const
	{
		return m_occupant;
	}

	void LobbyChairComponent::setSelfIsHere( bool here )
	{
		m_selfIsHere = here;
		reassessEnabled();
	}

	bool LobbyChairComponent::isSelfHere() const
	{
		return m_selfIsHere;
	}

	void LobbyChairComponent::setAsSubSeat( bool subbing )
	{
		m_subSeat = subbing;
	}

	void LobbyChairComponent::setOff( bool off )
	{
		m_off = off;
		reassessEnabled();
	}

	bool LobbyChairComponent::isOff() const
	{
		return m_off;
	}

	void LobbyChairComponent::paintComponent( const agui::PaintEvent &paintEvent )
	{

	}

	void LobbyChairComponent::setText( const std::string &text )
	{
		agui::Button::setText(text);
		m_arcText.setText(text);
	}

	void LobbyChairComponent::setFont( const agui::Font *font )
	{
		agui::Button::setFont(font);
	}

	void LobbyChairComponent::setScaledFont( GuiFontManager* fontMan, int numOnRow )
	{
		float factor = 0.007f;
		if(numOnRow == 1)
			factor *= 0.75f;
		else if(numOnRow == 2)
			factor *= 0.95f;
		else if(numOnRow == 3)
			factor *= 1.5f;
		agui::Font* f;
		if(m_chairNum == 0 || m_chairNum == 2)
		{
			f = fontMan->getFont(getWidth() * factor);
		}
		else
		{
			f = fontMan->getFont(getHeight() * factor);
		}

		m_arcText.setFont(f);

		if(numOnRow == 1)
		{
			m_arcText.setStrength(200.0f);
		}
		else
		{
			m_arcText.setStrength(150.0f);
		}
	}

	void LobbyChairComponent::renderText( GraphicsContext* g )
	{
		m_arcText.render(g);
	}

	void LobbyChairComponent::renderPlayer( GraphicsContext* g )
	{
		if(m_playerImg)
		{
			agui::Rectangle r = getAbsoluteRectangle();
			Sprite* img = getButtonState() == agui::Button::DEFAULT ? m_chairImg : 
				m_chairHoverImg;

			float ang = m_chairNum * (PI / 2.0f);
			ang -= PI / 2.0f;

			agui::Color c = agui::Color(1.0f,1.0f,1.0f);

			float darkness = m_darkness;

			if(isOff())
			{
				darkness = 0.5f;
			}

			float scale = 0.0f;
			float moveVec = 0.12f;
			float moveVecX = 0.02f;
			float mvx = 0.0f;
			float mvy = 0.0f;

			if(m_avatarData->gender == AVATAR_CACHE_GENDER_MALE)
				moveVecX = 0.0f;
			if(m_chairNum == 0 || m_chairNum == 2)
			{
				scale = (float)r.getWidth() / 747;
				moveVec *= r.getWidth();
				moveVecX *= r.getWidth();
			}
			else
			{
				scale = (float)r.getHeight() / 747;
				moveVec *= r.getHeight();
				moveVecX *= r.getWidth();
			}

			if(m_chairNum == 0)
			{
				mvy += moveVec;
				mvx += moveVecX;
			}
			else if(m_chairNum == 1)
			{
				mvx -= moveVec;
				mvy += moveVecX;
			}
			else if(m_chairNum == 2)
			{
				mvy -= moveVec;
				mvx -= moveVecX;
			}
			else if(m_chairNum == 3)
			{
				mvx += moveVec;
				mvy -= moveVecX;
			}

			img = m_playerImg;
			ang -= (CGE_PI / 2.0f);
			float scaleDiff = 512.0f / img->getWidth();
			scale *= 2.35f * scaleDiff;

			g->drawTintedScaledRotatedSprite(img,Color(darkness * c.getR() * m_playerOpacity,
				darkness * c.getG() * m_playerOpacity,darkness * c.getB() * m_playerOpacity,m_playerOpacity),img->getWidth() / 2, img->getHeight() / 2,
				r.getX() + mvx + (r.getWidth() / 2),r.getY() + mvy + (r.getHeight() / 2),
				(float)scale,
				(float)scale,ang,0);
		}
	
	}

	void LobbyChairComponent::update()
	{
		if(isOccupied())
		{
			if(m_playerOpacity < 1.0f)
				m_playerOpacity += 0.15f;
			if(m_playerOpacity > 1.0f)
				m_playerOpacity = 1.0f;
		}
		else
		{
			if(m_playerOpacity >= 0.0f)
				m_playerOpacity -= 0.15f;
			if(m_playerOpacity < 0.0f)
				m_playerOpacity = 0.0f;
            
            if(m_playerOpacity == 0 && m_playerImg) {
                m_playerImg = NULL;
            }
		}
	}

	bool LobbyChairComponent::isSubSeat() const
	{
		return m_subSeat;
	}

}

