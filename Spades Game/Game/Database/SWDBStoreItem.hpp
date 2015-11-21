#ifndef CGE_SWDB_STOREITEM_HPP
#define CGE_SWDB_STOREITEM_HPP
#include <stdlib.h>
#include <string>
#include "Game/serverplatform.hpp"
namespace cge
{
	struct SWDBStoreItem
	{
		int64 m_id;
		std::string m_name;
		std::string m_price;
		std::string m_type;
		std::string m_discountPercent;
		std::string m_morePercent;
		int64 m_coinValue;
		bool m_enabled;
		SWDBStoreItem(void);
		~SWDBStoreItem(void);
	};
}
#endif
