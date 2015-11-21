#ifndef CGE_AVATAR_COMPOSITE_HPP
#define CGE_AVATAR_COMPOSITE_HPP
#include "Game/Avatar/AvatarComponent.hpp"
#include "Game/Shader/Shader.hpp"
#include "Game/Engine/GraphicsContext.hpp"
#include "Game/Avatar/AvatarCache.hpp"
#include <vector>
namespace cge
{
	class AvatarComposite
	{
		Sprite* m_shadow;
		agui::Color m_shadowColor;
		agui::Color m_avatarColor;
		float m_scale;
		Shader* m_horzShader;
		Shader* m_vertShader;
		int m_x;
		int m_y;
		int m_shadowOffsetX;
		int m_shadowOffsetY;
		float m_blurVal;
		int m_blurPasses;
		int m_rotation;
		float m_alpha;

		Sprite* m_avatar;
		CachedAvatarImage m_avatarData;

		void drawBlackAvatar(GraphicsContext* g, int offX, int offY);
		
	public:
		AvatarComposite(void);
		const agui::Color& getShadowColor() const;
		const agui::Color& getAvatarColor() const;
		int getNumBlurPasses() const;
		float getBlurVal() const;
		void setBlurVal(float v);
		void setNumBlurPasses(int p);
		void setShadowColor(const agui::Color& c);
		void setAvatarColor(const agui::Color& c);
		int getShadowOffsetX() const;
		void setShadowOffsetX(int offset);
		int getShadowOffsetY() const;
		void setShadowOffsetY(int offset);
		void setX(int x);
		void setY(int y);
		int getX() const;
		int getY() const;
		float getWidth() const;
		float getHeight() const;
		void setScale(float scale);
		float getScale() const;
		void setShaders(Shader* hShader,Shader* vShader);
		void generate(GraphicsContext* c);
		void render(GraphicsContext* g);
		void setAvatar(const CachedAvatarImage& avatar);
		const CachedAvatarImage& getAvatarData() const;
		float getAvatarWidth() const;
		float getAvatarHeight() const;
		void setRoation(int rotation);
		void setAlpha(float alpha);
	
		virtual ~AvatarComposite(void);
	};
}

#endif

