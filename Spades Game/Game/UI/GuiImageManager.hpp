#ifndef CGE_GUI_IMAGE_MANAGER_HPP
#define CGE_GUI_IMAGE_MANAGER_HPP
#include "Agui/Agui.hpp"
#include <map>
namespace cge
{
	class GuiImageManager
	{
		std::map<std::string,agui::Image*> m_images;
		bool _loadImages(const std::string& path, const std::string& appendPath);
	public:
		bool mapImage(const std::string& imageName, const std::string& imagePath);
		agui::Image* getImage(const std::string& imageName);
			GuiImageManager(const std::string& path,const std::string& appendPath);
		virtual ~GuiImageManager(void);
	};
}

#endif
