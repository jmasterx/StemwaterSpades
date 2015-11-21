#ifndef CGE_SERVER_GIFT_MANAGER_HPP
#define CGE_SERVER_GIFT_MANAGER_HPP
#include <stdlib.h>
#include <list>
#include <vector>
namespace cge
{
	struct GiftData
	{
		std::string m_name;
		std::string m_thumbPath;
		int id;
		std::vector<std::string> m_spritePaths;
		std::vector<std::string> m_soundPaths;
		std::vector<float> m_soundGains;
		float m_scale;
		std::string m_category;
		bool m_animated;

		int getNumFrames() const
		{
			if(m_animated)
				return 1;

			return m_spritePaths.size();
		}

		GiftData()
			:
			id(0),m_scale(1.0f)
		{

		}
		GiftData(const std::string& name,int tid,
			const std::string& thumbPath,
			const std::vector<std::string>& spritePath, float scale,
			const std::vector<std::string>& soundPath, const std::string& category,
			const std::vector<float>& gains, bool animated
			)
			: m_name(name),
			id(tid),m_thumbPath(thumbPath),m_scale(scale),
			m_spritePaths(spritePath),m_soundPaths(soundPath),
			m_category(category),m_soundGains(gains),m_animated(animated)
		{

		}
	};

	class ServerGiftManager  
	{
		std::list<GiftData> m_gifts;
		void _cleanGift(int index);
		void _cleanGiftData();
		bool _loadGifts(const std::string& path);
	public:
		ServerGiftManager(const std::string& path);
		GiftData* getGiftById(int id);
		std::list<GiftData>* getGifts();
		~ServerGiftManager(void);
	};
}
#endif