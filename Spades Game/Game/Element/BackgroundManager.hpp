#ifndef CGE_BACKGROUND_MANAGER_HPP
#define CGE_BACKGROUND_MANAGER_HPP
#include "Game/platform.hpp"
#include "Game/Resource/Sprite.hpp"
#include <string>
#include <list>
#include <vector>
namespace cge
{
	struct BackgroundData
	{
		Sprite* m_thumbnail;
		std::vector<std::string> m_backgroundPath;
		std::string m_name;
		float m_scale;
		float m_offsetX;
		float m_offsetY;
		int id;

		BackgroundData(Sprite* thumbnail, const std::vector<std::string>& backgroundPath, 
			float offsetX, float offsetY, float scale,
			std::string name, int bid)
			: m_thumbnail(thumbnail), m_backgroundPath(backgroundPath), 
			m_offsetX(offsetX), m_offsetY(offsetY), m_scale(scale),
			m_name(name),id(bid)
		{
		}
	};

	class BackgroundManager
	{
		std::list<BackgroundData> m_backgrounds;
		bool _loadBackgrounds(const std::string& path, const std::string& bgFile);
		std::vector<Sprite*> m_currentBackground;
		BackgroundData* m_currentBackgroundData;
		void _cleanBackground();
		void _cleanBackgrounds();
	public:
		BackgroundManager(const std::string& path, const std::string& bgFile);
		bool loadBackground(BackgroundData* data);
		bool isBackgroundLoaded() const;
		BackgroundData* getCurrentBackgroundData() const;
		const std::vector<Sprite*>& getCurrentBackground() const;
		BackgroundData* getBackgroundByName(const std::string& name);
		int getBackgroundCount() const;
		std::list<BackgroundData>::iterator begin();
		std::list<BackgroundData>::iterator end();
		BackgroundData* getBackgroundAt(int index);
		BackgroundData* getBackgroundFromID(int id);
		void unload();
		virtual ~BackgroundManager(void);
	};
}
#endif
