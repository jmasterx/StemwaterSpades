#ifndef SWDB_LAYER_HPP
#define SWDB_LAYER_HPP
#include "Game/Server/ServerTable.hpp"
#include "Game/Database/StemwaterDB.hpp"
#include "Game/SpadesGame/SpadesGameInfo.hpp"
#include <vector>
namespace cge
{
	class SWDBLayer
	{
		StemwaterDB* m_db;
	public:
		SWDBLayer(void);
		~SWDBLayer(void);
		void setDB(StemwaterDB* db);
		int64 generateDBGame( const SpadesGameInfo& info );
		int calcNewRating( 
			SWDBPlayer* pid, int pidPos, SWDBPlayer* p2, 
			int p2Pos, SWDBPlayer* p3, int p3Pos, SWDBPlayer* p4,
			int p4Pos, int numPlayers, bool lifeRating );
		void punishQuitter(int64 pid, int karmAmount, const SpadesGameInfo& info, ServerTable* t, bool sub);
		bool gameIsOver( ServerTable* t, const std::vector<int>& winners, const std::vector<int>& scores,
			const std::vector<int64>& pids, const std::vector<int64>& origids, std::vector<int>& newRatings );
		bool awardCoins( ServerTable* t, const std::vector<int>& winners, const std::vector<int64>& pids, 
			const std::vector<int64>& origpids, int64 subPayout, std::vector<int64>& netChange, std::vector<int64>& newCoinAmount,
			const std::vector<float>& payratesIndividual,const std::vector<float>& payratesCutthroat,const std::vector<float>& payratesPartnership,
			float houseCut);
	};
}

#endif
