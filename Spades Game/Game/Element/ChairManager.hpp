#ifndef CGE_CHAIR_MANAGER_HPP
#define CGE_CHAIR_MANAGER_HPP
#include "Game/platform.hpp"
#include <Game/Resource/Sprite.hpp>
#include <string>
#include <list>
#include <vector>
namespace cge
{
	struct ChairData
	{
		Sprite* m_thumbnail;
		std::string m_chairPath;
		std::string m_name;
		float m_scale;
		float m_offsetX;
		float m_offsetY;
		float m_gscale;
		float m_goffsetX;
		float m_goffsetY;
		int id;

		ChairData(Sprite* thumbnail, std::string chairPath, 
			float offsetX, float offsetY, float scale,
			std::string name, int cid, float gscale, float goffsetx, float goffsety)
			: m_thumbnail(thumbnail), m_chairPath(chairPath), 
			m_offsetX(offsetX), m_offsetY(offsetY), m_scale(scale),
			m_name(name),id(cid),m_gscale(gscale),m_goffsetX(goffsetx),m_goffsetY(goffsety)
		{
		}
	};

	class ChairManager
	{
		std::list<ChairData> m_chairs;
		bool _loadChairs(const std::string& path, const std::string& bgFile);
		std::vector<ChairData*> m_currentChairData;
		std::vector<Sprite*> m_currentChair;
		void _cleanChair();
		void _cleanChairs();
	public:
		ChairManager(const std::string& path, const std::string& chairFile);
		bool loadChair( const std::vector<ChairData*>& chairDatas );
		ChairData* getCurrentChairData(int chairNo) const;
		Sprite* getCurrentChair(int chairNo) const;
		ChairData* getChairByName(const std::string& name);
		int getChairCount() const;
		std::list<ChairData>::iterator begin();
		std::list<ChairData>::iterator end();
		ChairData* getChairAt(int index);
		ChairData* getChairFromID(int id);
		int getLoadedChairCount() const;
		void unload();
		virtual ~ChairManager(void);
	};
}
#endif
