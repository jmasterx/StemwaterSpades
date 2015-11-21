#include "Game/UI/GuiColorManager.hpp"
#include "Game/Utility/File.hpp"
#include <iostream>
namespace cge
{
	GuiColorManager::~GuiColorManager(void)
	{
	}

	GuiColorManager::GuiColorManager( const std::string& path )
	{
		_loadColors(path);
	}

	agui::Color GuiColorManager::getColor( const std::string& elementName )
	{
		if(m_colors.find(elementName) == m_colors.end())
		{
			return agui::Color(0,0,0,0);
		}
		else
		{
			return m_colors[elementName];
		}
	}

	Color GuiColorManager::getGameColor( const std::string& elementName )
	{
		agui::Color c = getColor(elementName);

		return Color(c.getR(),c.getG(),c.getB(),c.getA());
	}

	void GuiColorManager::mapColor( const std::string& colorName, const agui::Color& color )
	{
		m_colors[colorName] = color;
	}

	bool GuiColorManager::_loadColors( const std::string& path )
	{
		//open the file
		File ifs( path.c_str() );

		//ensure it is open
		if(!ifs.isOpen())
		{
			return false;
		}
		std::string line;

		//read each line
		while( ifs.readLine(line) )
		{
			if(line.length() == 0)
			{
				continue;
			}
			else if(line.length() >= 1 && line[0] == '#')
			{
				continue;
			}

			char name[65];
			int r,g,b,a;
			float fr,fg,fb,fa;
			std::string elemName;
			
			int numArgs = sscanf(line.c_str(),"%64s = %d,%d,%d,%d",name,&r,&g,&b,&a);
			if(numArgs == 4 || numArgs == 5)
			{
				elemName = name;
				if(numArgs == 4)
				{
					mapColor(elemName,agui::Color(r,g,b));
				}
				else
				{
					mapColor(elemName,agui::Color(r,g,b,a));
				}
			}
			else
			{
				numArgs = sscanf(line.c_str(),"%64s = %f,%f,%f,%f",name,&fr,&fg,&fb,&fa);
				elemName = name;

				if(numArgs == 4)
				{
					mapColor(elemName,agui::Color(fr,fg,fb));
				}
				else if(numArgs == 5)
				{
					mapColor(elemName,agui::Color(fr,fg,fb,fa));
				}
				else
				{
					return false;
				}
			}
		}

		return true;
	}

}

