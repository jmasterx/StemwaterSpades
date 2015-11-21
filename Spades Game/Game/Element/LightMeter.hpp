#ifndef CGE_LIGHT_METER_HPP
#define CGE_LIGHT_METER_HPP
#include "Game/Element/DynamicElement.hpp"
#include "Game/SpadesGame/SpadesGameMessages.hpp"
#include <vector>
#include <sstream>
namespace cge
{

	class LightMeter : public DynamicElement
	{
	public:
		struct Light
		{
			Vec2 m_pos;
			SpadesPointEnum m_point;
			Light()
				: m_point(NOT_A_POINT)
			{

			}
			Light(const Vec2& pos, SpadesPointEnum point)
				: m_pos(pos), m_point(point)
			{

			}
		};

		Vec2 m_center;
		std::vector<Light> m_lights;
		Light m_leftLight;
		Light m_rightLight;

		Sprite* m_regularPointImg;
		Sprite* m_pointFromPartnerImg;
		Sprite* m_overTrickImg;
		Sprite* m_notAPointImg;
		Sprite* m_contractPointImg;
		Sprite* m_orbCap;
		Sprite* m_orbRedCap;

		int m_totalCount;
		int m_currentCount;

		bool m_inverted;

		int m_playerNum;

		float m_baseSize;

		bool m_useLights;
		bool m_nilFailMode;
		bool m_nilFailIncreasing;
		float m_nilFailedOpacity;
		std::string m_nilStr;

		void positionLights();
		void resetNilFailed();
		agui::Font* m_font;
		bool m_hasBid;
	public:
		LightMeter(
		Sprite* regularPointImg,
		Sprite* pointFromPartnerImg,
		Sprite* overTrickImg,
		Sprite* notAPointImg,
		Sprite* contractPointImg,
		Sprite* orbCap,
		Sprite* redOrbCap,
		int numLights, int playerNum, const std::string& nilStr);
		int getNumTricksMade() const;
		void addTrick(SpadesPointEnum point);
		int getTotalLightCount() const;
		void clearTricks();
		void setContractTrickCount(int count);
		SpadesPointEnum getLightValueAt(int index) const;
		const Vec2& getLightPosAt(int index) const;
		void setInverted(bool inverted);
		void setHasBid(bool bid);
		int getContractAmount() const;
		virtual void elemLogic(double t);
		virtual void onFailedNil();
		virtual void render(GraphicsContext* g);
		void setFont(agui::Font* f);
		void setUseLights(bool use);
		bool isUsingLights() const;
		virtual ~LightMeter(void);
	};
}

#endif
