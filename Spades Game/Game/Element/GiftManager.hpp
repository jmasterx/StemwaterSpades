#ifndef CGE_GIFT_MANAGER_HPP
#define CGE_GIFT_MANAGER_HPP
#include <stdlib.h>
#include <string>
#include <list>
#include "Game/platform.hpp"
#include <Agui/Backends/Allegro5/Allegro5Image.hpp>
#include "Game/Resource/Sprite.hpp"
#include <vector>
#define MAX_GIFTS 4
namespace cge
{
	struct GiftData
	{
		Sprite* m_thumbnail;
		std::string m_name;
		std::string m_thumbPath;
		int id;
		agui::Allegro5Image m_img;
		std::vector<std::string> m_spritePaths;
		std::vector<std::string> m_soundPaths;
		std::vector<float> m_soundGains;
		float m_scale;
		Sprite* m_image;
		std::string m_category;
		bool m_animated;

		GiftData()
			: m_thumbnail(NULL),
			id(0),m_scale(1.0f),
			m_image(NULL)
		{

		}
		GiftData(const std::string& name,int tid,
			const std::string& thumbPath,
			const std::vector<std::string>& spritePath, float scale,
			const std::vector<std::string>& soundPath, const std::string& category,
			const std::vector<float>& gains, bool animated
			)
			: m_name(name),m_thumbnail(NULL),
			id(tid),m_thumbPath(thumbPath),m_scale(scale),
			m_spritePaths(spritePath),m_image(NULL),m_soundPaths(soundPath),
			m_category(category),m_soundGains(gains),m_animated(animated)
		{

		}
	};

	class GiftManager  
	{
		std::list<GiftData> m_gifts;
		void _cleanGift(int index);
		void _cleanGifts();
		void _cleanGiftData();
		bool _loadGifts(const std::string& path, const std::string& thumbAppend, const std::string& spriteAppend, const std::string& soundPath, const std::string& themeFile);
		GiftData m_giftCache[MAX_GIFTS];
	public:
		GiftManager(const std::string& path, const std::string& thumbAppend, const std::string& spriteAppend, const std::string& soundPath, const std::string& bgFile);
		GiftData* getGiftById(int id);
		void destroyThumbs();
		void loadThumbs();
		std::list<GiftData>* getGifts();
		~GiftManager(void);
	};
}
#endif
