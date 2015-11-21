#include "GuiImageManager.hpp"
#include <iostream>
#include "Game/Utility/File.hpp"
#include <string>
namespace cge
{
	GuiImageManager::GuiImageManager( 
		const std::string& path,const std::string& appendPath )
	{
		_loadImages(path,appendPath);
	}

	GuiImageManager::~GuiImageManager(void)
	{
		for(std::map<std::string,agui::Image*>::iterator it = 
			m_images.begin(); it != m_images.end(); ++it)
		{
			delete it->second;
		}

		m_images.clear();
	}

	bool GuiImageManager::_loadImages(const std::string& path,
		const std::string& appendPath)
	{
		//open the file
		File ifs( path );

		//ensure it is open
		if(!ifs.isOpen())
		{
			return false;
		}
		std::string line;

		//read each image
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

			char name[65], imgPath[129];
			int margins[4];

		    int numArgsParsed =	sscanf(line.c_str(), 
				"%64s%*[^\"]%*c%128[^\"]%*c %d %d %d %d", 
				name, 
				imgPath, 
				&margins[0], 
				&margins[1],
				&margins[2],
				&margins[3]);

			//probably an empty line
			if(numArgsParsed == 0)
			{
				continue;
			}

			if(!mapImage(std::string(name), appendPath + std::string(imgPath)))
			{
				return false;
			}
				int numMargins = numArgsParsed - 2;
				agui::Image* img = getImage(std::string(name));

			if(!img)
			{
				return false;
			}

			if(numMargins == 1)
			{
				img->setMargins(margins[0],margins[0]);
			}
			else if(numMargins == 2)
			{
				img->setMargins(margins[0],margins[1]);
			}
			else if(numMargins == 3)
			{
				img->setMargins(margins[0],margins[1],margins[2],margins[2]);
			}
			else if(numMargins == 4)
			{
				img->setMargins(margins[0],margins[1],margins[2],margins[3]);
			}
		}

		return true;
	}

	bool GuiImageManager::mapImage( const std::string& imageName, const std::string& imagePath )
	{
		agui::Image* img = agui::Image::load(imagePath);

		if(!img)
		{
			return false;
		}

		if(m_images.find(imageName) != m_images.end())
		{
			delete m_images[imageName];
		}

		m_images[imageName] = img;

		return true;
	}

	agui::Image* GuiImageManager::getImage( const std::string& imageName )
	{
		if(m_images.find(imageName) == m_images.end())
		{
			return NULL;
		}
		else
		{
			return m_images[imageName];
		}

	}
}
