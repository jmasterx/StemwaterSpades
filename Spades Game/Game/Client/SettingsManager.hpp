#ifndef CGE_SETINGS_MANAGER_HPP
#define CGE_SETINGS_MANAGER_HPP
#include "Game/Client/LobbyEventEnums.hpp"
#include "stdlib.h"
#include <string>
#include <map>
#include <vector>
namespace cge
{
	class SettingsManager
	{
		std::map<GroupTypeEnum,bool> m_groupsToggled;
		std::map<std::string,bool> m_boolSettings;
		std::map<std::string,float> m_numericSettings;
		std::map<std::string,std::string> m_stringSettings;
		std::vector<TableFilterEnum> m_filters;
		std::string m_path;
		bool mapElement(const std::string& elemName, const std::string&  elemVal);
	public:
		SettingsManager(void);
		bool load();
		bool save();
		void setUnpersistantValues();
		void setPath(const std::string& path);
		void setGroupToggled(GroupTypeEnum group, bool toggled);
		bool isGroupToggled(GroupTypeEnum group);

		void setNumericSetting(const std::string& setting,float val);
		bool hasNumericSetting(const std::string& setting);
		float getNumericSetting(const std::string& setting);
		void setStringSetting(const std::string& setting,const std::string& val);
		bool hasStringSetting(const std::string& setting);
		std::string getStringSetting(const std::string& setting);
		void setBoolSetting(const std::string& setting,bool val);
		bool hasBoolSetting(const std::string& setting);
		bool getBoolSetting(const std::string& setting);
		void setTableFilters(const std::vector<TableFilterEnum>& filters);
		const std::vector<TableFilterEnum>& getTableFilters() const;

		~SettingsManager(void);
	};
}

#endif