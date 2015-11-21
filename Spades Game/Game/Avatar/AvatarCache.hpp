#ifndef CGE_AVATAR_CACHE_HPP
#define CGE_AVATAR_CACHE_HPP
#include "Game/Resource/Sprite.hpp"
#include <map>
#include "Agui/Backends/Allegro5/Allegro5Image.hpp"
#define AVATAR_CACHE_GENDER_MALE 0
#define AVATAR_CACHE_GENDER_FEMALE 1
#define AVATAR_CACHE_ETHNICITY_WHITE 0
#define AVATAR_CACHE_ETHNICITY_BLACK 1
namespace cge
{
	struct CachedAvatarImage
	{
		int id;
		int gender;
		int ethnicity;
		Sprite* image;
		agui::Allegro5Image guiImg;
		bool reserved;
		CachedAvatarImage()
			: id(0),gender(0), ethnicity(0),reserved(false), image(NULL)
		{

		}
	};

	struct CachedAvatarData
	{
		int id;
		int gender;
		int ethnicity;
		bool reserved;
		std::string hdPath;
		std::string sdPath;
		CachedAvatarData()
			: id(0),gender(0), ethnicity(0),reserved(false)
		{

		}
	};

	class AvatarCache
	{
		std::map<int,CachedAvatarData> m_avatarData;
		std::map<int,CachedAvatarImage> m_avatarImages;
		CachedAvatarImage m_gameCache[4];
		CachedAvatarImage m_selectionCache;
		void _clearSelectionCache();
		void _clearAvatarData();
		void _clearAvatarImages();
		CachedAvatarImage loadImage(const CachedAvatarData& data, bool hd);
		bool _loadAvatars(const std::string& path, const std::string& hdPath, const std::string& sdPath, const std::string& filename);
	public:
		AvatarCache(const std::string& path, const std::string& hdPath, const std::string& sdPath, const std::string& filename);
		bool setAvatarSelection(int id);
		void clearAvatarSelection();
		bool setGameAvatar(int id, int index);
		void clearGameCache();
		void clearGameCacheAtPos(int pos);
		std::map<int,CachedAvatarImage>* getCachedAvatars();
		std::map<int,CachedAvatarData>* getCachedAvatarData();
		CachedAvatarImage* getGameAvatarAt(int index);
		bool avatarExists(int id);
		~AvatarCache(void);
	};
}
#endif