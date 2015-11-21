#ifndef CGE_GUI_COLOR_MANAGER_HPP
#define CGE_GUI_COLOR_MANAGER_HPP
#include "Agui/Agui.hpp"
#include "Game/platform.hpp"
#include <map>
namespace cge
{
	class GuiColorManager
	{
		std::map<std::string,agui::Color> m_colors;
		bool _loadColors(const std::string& path);
	public:
		void mapColor(const std::string& colorName, const agui::Color& color);
		agui::Color getColor(const std::string& elementName);
		Color getGameColor(const std::string& elementName);
		GuiColorManager(const std::string& path);
		virtual ~GuiColorManager(void);
	};
}

#endif
