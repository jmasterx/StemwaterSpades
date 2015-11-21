#ifndef STEMWATER_DB_UNIT_TEST_HPP
#define STEMWATER_DB_UNIT_TEST_HPP
#include "Game/Database/StemwaterDB.hpp"
namespace cge
{
	class StemwaterDBUnitTest
	{
		void executeUnit(bool val);
		StemwaterDB* m_db;
	public:
		StemwaterDBUnitTest(void);
		void unitTest(StemwaterDB& db);
		~StemwaterDBUnitTest(void);
	};
}

#endif