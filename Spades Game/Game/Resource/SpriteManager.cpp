#include "Game/Resource/SpriteManager.hpp"
#include <iostream>
#include "Game/Utility/File.hpp"
#include <string>
namespace cge
{
	SpriteManager::SpriteManager( 
		const std::string& path,const std::string& appendPath )
	{
		_loadImages(path,appendPath);
	}

	SpriteManager::~SpriteManager(void)
	{
		for(std::map<std::string,Sprite*>::iterator it = 
			m_images.begin(); it != m_images.end(); ++it)
		{
			delete it->second;
		}

		m_images.clear();
	}

	bool SpriteManager::_loadImages(const std::string& path,
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

			int numArgsParsed =	sscanf(line.c_str(), 
				"%64s%*[^\"]%*c%128[^\"]%*c", 
				name, 
				imgPath);

			//probably an empty line
			if(numArgsParsed == 0)
			{
				continue;
			}

			if(!mapImage(std::string(name), appendPath + std::string(imgPath)))
			{
				return false;
			}

			Sprite* img = getImage(std::string(name));

			if(!img)
			{
				return false;
			}
		}

		return true;
	}

	bool SpriteManager::mapImage( const std::string& imageName, const std::string& imagePath )
	{
		Sprite* img = new Sprite(imagePath);

		if(!img->getBitmap())
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

	Sprite* SpriteManager::getImage( const std::string& imageName )
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
