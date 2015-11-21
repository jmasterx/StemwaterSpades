#ifndef CGE_THEME_MANAGER_HPP
#define CGE_THEME_MANAGER_HPP
#include <stdlib.h>
#include "Game/Resource/Sprite.hpp"
#include <Agui/Backends/Allegro5/Allegro5Image.hpp>
#include <list>
namespace cge
{
	struct ThemeData
	{
		Sprite* m_thumbnail;
		std::string m_name;
		std::string m_thumbPath;
		int m_chairID;
		int m_tableID;
		int m_backgroundID;
		int id;
		int vid;
		std::string m_song;
		agui::Allegro5Image m_img;

		ThemeData(const std::string& name,
			int chairID, int tableID, int backgroundID,int tid,
			Sprite* thumbnail, const std::string& thumbPath,
			const std::string& song, int visualId)
			: m_name(name),m_chairID(chairID),m_tableID(tableID),
			m_backgroundID(backgroundID),m_thumbnail(thumbnail),
			id(tid),m_thumbPath(thumbPath),vid(visualId),m_song(song)
		{

		}
	};

	class ThemeManager
	{
		std::list<ThemeData> m_themes;
		bool _loadThemes(const std::string& path, const std::string& themeFile);
		ThemeData* m_currentThemeData;
	public:
		ThemeManager(const std::string& path, const std::string& bgFile);
		std::list<ThemeData>* getThemes();
		ThemeData* getTheme(int id);
		void loadTheme(ThemeData* data);
		ThemeData* getLoadedTheme();
		bool isThemeLoaded() const;
		void destroyThumbs();
		void loadThumbs();
		~ThemeManager(void);
	};
}

#endif