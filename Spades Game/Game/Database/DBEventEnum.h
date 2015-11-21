#ifndef CGE_DB_EVENT_ENUM_H
#define CGE_DB_EVENT_ENUM_H
namespace cge
{
	enum DBEventEnum
	{
		DBEVT_NEW_CONNECTION,
		DBEVT_DISCONNECTION,
		DBEVT_NETWORK_ERROR
	};
}
#endif