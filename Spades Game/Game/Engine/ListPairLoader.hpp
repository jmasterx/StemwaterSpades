#ifndef CGE_LIST_PAIR_LOADER_HPP
#define CGE_LIST_PAIR_LOADER_HPP
#include <stdlib.h>
#include <string>
#include <vector>
#include <Agui/Agui.hpp>
namespace cge
{
	struct ListPairItem
	{
		std::string descr;
		std::string val;
		agui::Image* image;
		bool operator< (const ListPairItem &rhs) const;
	};
	class ListPairLoader
	{
		static std::vector<ListPairItem> m_strings;
		static std::vector<ListPairItem> m_stringsSorted;
		
	public:
		static bool _initStrings(const std::string& path, const std::string& appendPath);
		static const std::vector<ListPairItem>& getStrings();
		static const std::vector<ListPairItem>& getStringsSorted();
		static void clear();
		static agui::Image* getImage(const std::string& descr);
		virtual ~ListPairLoader(void);
	};
}

#endif
