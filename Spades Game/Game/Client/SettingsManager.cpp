#include "Game/Client/SettingsManager.hpp"
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include "Game/Utility/File.hpp"
namespace cge
{
	SettingsManager::SettingsManager(void)
	{
		m_filters.push_back(ALL_TABLES_TFILTER);

		setGroupToggled(FRIENDS_GROUP,true);
		setGroupToggled(ENEMIES_GROUP,true);
		setGroupToggled(IN_LOBBY_GROUP,true);
		setGroupToggled(IN_GAME_GROUP,true);

		setBoolSetting("chat.censored",false);
		setBoolSetting("display.fullscreen",false);
		setBoolSetting("audio.enabled",true);
		setBoolSetting("account.created",false);
		setNumericSetting("audio.gain",1.75f);
		setNumericSetting("chat.lobby.font",0.25f);
		setNumericSetting("chat.game.font",0.35f);
		setNumericSetting("lobby.tables",1.0f);
		setStringSetting("user.name","");
		setStringSetting("user.language","english");
		setBoolSetting("game.settings.on",false);
		setBoolSetting("autologin.active",false);
		setStringSetting("auth.name","");
		setStringSetting("auth.hash","");
		setBoolSetting("light.meter.on",false);
		setBoolSetting("player.names.on",true);
		setBoolSetting("welcome",true);

		//new game
		setStringSetting("newgame.playwith","people");
		setStringSetting("newgame.rated","rated");
		setStringSetting("newgame.watch","yes");
		setStringSetting("newgame.cutdeck","yes");
		setStringSetting("newgame.endtype","score");
		setStringSetting("newgame.type","regular");
		setStringSetting("newgame.bidtype","normal");
		setNumericSetting("newgame.feeindex",0.0f);
		setNumericSetting("newgame.endvalindex",1.0f);

		//sound
		setNumericSetting("master.volume",1.0f);
		setNumericSetting("game.sounds",1.0f);
		setNumericSetting("gift.sounds",1.0f);
		setNumericSetting("music.ambiance",1.0f);
		setBoolSetting("turn.sound",false);
		setBoolSetting("mute.lobby.chat.on",false);

		setStringSetting("chat.style","game");
	}

	SettingsManager::~SettingsManager(void)
	{
	}

	void SettingsManager::setGroupToggled( GroupTypeEnum group, bool toggled )
	{
		m_groupsToggled[group] = toggled;
	}

	bool SettingsManager::isGroupToggled( GroupTypeEnum group )
	{
		if(m_groupsToggled.find(group) != m_groupsToggled.end())
		{
			return m_groupsToggled[group];
		}

		return false;
	}

	void SettingsManager::setNumericSetting( const std::string& setting,float val )
	{
		m_numericSettings[setting] = val;
		save();
	}

	bool SettingsManager::hasNumericSetting( const std::string& setting )
	{
		return m_numericSettings.find(setting) != m_numericSettings.end();
	}

	float SettingsManager::getNumericSetting(const std::string& setting)
	{
		if(hasNumericSetting(setting))
		{
			return m_numericSettings[setting];
		}

		return -1.0f;
	}

	void SettingsManager::setBoolSetting( const std::string& setting,bool val )
	{
		m_boolSettings[setting] = val;
		save();
	}

	bool SettingsManager::hasBoolSetting( const std::string& setting )
	{
		return m_boolSettings.find(setting) != m_boolSettings.end();
	}

	bool SettingsManager::getBoolSetting( const std::string& setting )
	{
		if(hasBoolSetting(setting))
		{
			return m_boolSettings[setting];
		}

		return false;
	}

	const std::vector<TableFilterEnum>& SettingsManager::getTableFilters() const
	{
		return m_filters;
	}

	void SettingsManager::setTableFilters( const std::vector<TableFilterEnum>& filters )
	{
		m_filters = filters;
	}

	void SettingsManager::setPath( const std::string& path )
	{
		m_path = path;
	}

	bool SettingsManager::load()
	{
		std::string path = m_path;

		//open the file
		std::ifstream ifs;
		ifs.open(path.c_str());

		//ensure it is open
		if(!ifs.is_open())
		{
			return false;
		}
		std::string line;

		//read each line
		while( !ifs.eof() )
		{
			std::getline(ifs,line);
			if(line.length() == 0)
			{
				continue;
			}
			else if(line.length() >= 1 && line[0] == '#')
			{
				continue;
			}

			int lineAt = 0;
			std::string elemName;

			//extract until space
			while(lineAt < (int)line.length())
			{
				if(line[lineAt] != ' ')
				{
					elemName += line[lineAt];
					lineAt++;
				}
				else 
				{
					lineAt += 3; //' = '
					break;
				}
			}

			std::string elemVal;
			//extract until end
			while(lineAt < (int)line.length())
			{
				//filter newline
				if(line[lineAt] == '\\' && lineAt + 1 < 
					(int)line.length() && line[lineAt + 1] == 'n')
				{
					elemVal += '\n';
					lineAt += 2;
				}
				else 
				{
					elemVal += line[lineAt];
					lineAt++;
				}
			}
			mapElement(elemName, elemVal);
		}
		return true;
	}

	bool SettingsManager::mapElement( const std::string& elemName, const std::string& elemVal )
	{
		std::stringstream stream(elemVal);
		float val = -1.0f;
		stream >> val;
		if (!stream.fail()) 
		{
			setNumericSetting(elemName,val);
			return true;
		}
		else if(elemVal.length() >= 2 && elemVal[0] == '"') // remove ""
		{
			setStringSetting(elemName,elemVal.substr(1,elemVal.length() - 2));
			return true;
		}
		else if(elemVal == "true")
		{
			setBoolSetting(elemName,true);
			return true;
		}
		else if(elemVal == "false")
		{
			setBoolSetting(elemName,false);
			return true;
		}

		return false;
	}

	bool SettingsManager::save()
	{
		if(m_path == "")
			return false;
		std::fstream filestr;

		filestr.open(m_path.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);

		if(!filestr.is_open())
			return false;

		for(std::map<std::string,bool>::iterator it = 
			m_boolSettings.begin(); it != m_boolSettings.end(); ++it)
		{
			filestr << it->first << " = " << (it->second ? "true" : "false") << std::endl;
		}

		for(std::map<std::string,float>::iterator it = 
			m_numericSettings.begin(); it != m_numericSettings.end(); ++it)
		{
			filestr << it->first << " = " << it->second << std::endl;
		}

		for(std::map<std::string,std::string>::iterator it = 
			m_stringSettings.begin(); it != m_stringSettings.end(); ++it)
		{
			filestr << it->first << " = " << "\"" << it->second << "\"" << std::endl;
		}

		filestr.close();

		return true;
	}

	void SettingsManager::setStringSetting( const std::string& setting,const std::string& val )
	{
		m_stringSettings[setting] = val;
		save();
	}

	bool SettingsManager::hasStringSetting( const std::string& setting )
	{
		return m_stringSettings.find(setting) != m_stringSettings.end();
	}

	std::string SettingsManager::getStringSetting( const std::string& setting )
	{
		if(hasStringSetting(setting))
		{
			return m_stringSettings[setting];
		}

		return "";
	}

	void SettingsManager::setUnpersistantValues()
	{
		setBoolSetting("toggle.filter.menu.on",false);
	}

}
