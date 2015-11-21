#include "Game/Database/SWDBLayer.hpp"
#define KARMA_FOR_WIN 1
#define KARMA_FOR_LOSS 0
#define KARMA_FOR_SUB_LOSS 1
#define KARMA_FOR_SUB_WIN 2
namespace cge
{
	struct GOPlayer
	{
		int64 m_id;
		int64 m_origID;
		int m_score;
		int m_curRating;
		int m_lifeRating;
		int m_teamPlace;
		int m_teamNo;
		int64 m_pgid;
		int64 m_netCoinChange;
		int64 m_newCoinAmount;
		SWDBPlayer m_player;
		SWDBPlayer m_ratePlayer;
		GOPlayer()
			: m_teamPlace(0),m_id(-1),m_score(0),
			m_curRating(0),m_lifeRating(0),m_origID(-1),m_teamNo(-1),
			m_pgid(-1),m_netCoinChange(0),m_newCoinAmount(0)
		{

		}
	};

	SWDBLayer::SWDBLayer(void)
	{
	}

	SWDBLayer::~SWDBLayer(void)
	{
	}

	void SWDBLayer::setDB( StemwaterDB* db )
	{
		m_db = db;
	}

	int64 SWDBLayer::generateDBGame( const SpadesGameInfo& info )
	{
			std::string gtype;
			if(info.isIndividual())
				gtype = "individual";
			else if(info.isCutthroat())
				gtype = "cutthroat";
			else if(info.isPartnership())
				gtype = "partnership";
			else
				gtype = "pass2";

			std::string btype;
			if(info.isMirror())
				btype = "mirror";
			else if(info.isSuicide())
				btype = "suicide";
			else
				btype = "normal";

			std::string ptype;
			if(info.isHandEnded())
				ptype = "hand";
			else if(info.isTimeEnded())
				ptype = "time";
			else
				ptype = "score";

			int64 gid = -1;
			int result = -1;
			if(!m_db->gameCreate(
				gtype,btype,ptype,info.getEndValue(),
				info.isDeckCutEnabled(),info.isWatchingAllowed(),
				info.isRated(),info.getFee(),&gid,&result))
			{
				return -1;
			}

			if(result == 1)
			{
				return gid;
			}
			else
			{
				return -1;
			}
	}

	void SWDBLayer::punishQuitter( int64 pid, int karmAmount, const SpadesGameInfo& info, ServerTable* t, bool sub )
	{
		if(sub)
			return;

		m_db->playerAbandonedGame(pid);
		m_db->playerAddKarma(pid,karmAmount);
		if(info.isIndividual())
			m_db->playerUpdateCompletedGame(pid,0,0,0,1,0,0,0,0,0,0,0,false);
		else if(info.isCutthroat())
			m_db->playerUpdateCompletedGame(pid,0,0,0,0,0,0,1,0,0,0,0,false);
		else if(info.isPassTwo())
			m_db->playerUpdateCompletedGame(pid,0,0,0,0,0,0,0,0,0,0,1,false);
		else if(!info.isPassTwo() && info.isPartnership())
			m_db->playerUpdateCompletedGame(pid,0,0,0,0,0,0,0,0,1,0,0,false);

		//rated not sub
		if(info.isRated() && !sub)
		{
			SWDBPlayer players[4];
			SWDBPlayer allPlayers[4];
			SWDBPlayer myPlayer;
			int newCur = 0;
			int newLife = 0;
			m_db->getPlayer(pid,&myPlayer);
			int count = 0;
			int otherCount = 0;

			for(int i = 0; i < info.getNumPlayers(); i++)
			{
				if(t->getChairAt(i)->getOriginalPid() != pid)
				{
					m_db->getPlayer(t->getChairAt(count)->getOriginalPid(),&players[count]);
				
					allPlayers[otherCount] = players[count];
					count++;
					otherCount++;
				}
				else
				{
					m_db->getPlayer(t->getChairAt(otherCount)->getOriginalPid(),&allPlayers[otherCount]);
					otherCount++;
				}
			}

			if(info.isIndividual())
			{
				newCur = calcNewRating(&myPlayer,4,&players[0],1,&players[1],2,&players[2],3,3,false);
				newLife = calcNewRating(&myPlayer,4,&players[0],1,&players[1],2,&players[2],3,3,true);
			}
			else if(info.isCutthroat())
			{
				newCur = calcNewRating(&myPlayer,3,&players[0],1,&players[1],2,NULL,3,2,false);
				newLife = calcNewRating(&myPlayer,3,&players[0],1,&players[1],2,NULL,3,2,true);
			}
			else
			{
				if(t->getChairAt(0)->getOriginalPid() == pid || t->getChairAt(2)->getOriginalPid() == pid)
				{
					newCur = calcNewRating(&myPlayer,2,&allPlayers[1],1,&allPlayers[3],1,NULL,3,2,false);
					newLife = calcNewRating(&myPlayer,2,&allPlayers[1],1,&allPlayers[3],1,NULL,3,2,false);
				}
				else
				{
					newCur = calcNewRating(&myPlayer,2,&allPlayers[0],1,&allPlayers[2],1,NULL,3,2,false);
					newLife = calcNewRating(&myPlayer,2,&allPlayers[0],1,&allPlayers[2],1,NULL,3,2,false);
				}
			}

			m_db->playerUpdateRating(pid,newLife,newCur);
		}
		else if(info.isRated() && sub)
		{
				SWDBPlayer players[4];
				SWDBPlayer allPlayers[4];
				SWDBPlayer myPlayer;
				int newCur = 0;
				int newLife = 0;
				m_db->getPlayer(pid,&myPlayer);
				int count = 0;
				int otherCount = 0;

				for(int i = 0; i < info.getNumPlayers(); i++)
				{
					if(t->getChairAt(i)->getPlayer() && t->getChairAt(i)->getPlayer()->getId() != pid)
					{
						m_db->getPlayer(t->getChairAt(count)->getLastPid(),&players[count]);

						allPlayers[otherCount] = players[count];
						count++;
						otherCount++;
					}
					else
					{
						m_db->getPlayer(t->getChairAt(otherCount)->getOriginalPid(),&allPlayers[otherCount]);
						otherCount++;
					}
				}

				if(info.isIndividual())
				{
					newCur = calcNewRating(&myPlayer,4,&players[0],1,&players[1],2,&players[2],3,3,false);
					newLife = calcNewRating(&myPlayer,4,&players[0],1,&players[1],2,&players[2],3,3,true);
				}
				else if(info.isCutthroat())
				{
					newCur = calcNewRating(&myPlayer,3,&players[0],1,&players[1],2,NULL,3,2,false);
					newLife = calcNewRating(&myPlayer,3,&players[0],1,&players[1],2,NULL,3,2,true);
				}
				else
				{
					if((t->getChairAt(0)->getPlayer() && t->getChairAt(0)->getPlayer()->getId() == pid) || 
						(t->getChairAt(2)->getPlayer() && t->getChairAt(2)->getPlayer()->getId() == pid))
					{
						newCur = calcNewRating(&myPlayer,2,&allPlayers[1],1,&allPlayers[3],1,NULL,3,2,false);
						newLife = calcNewRating(&myPlayer,2,&allPlayers[1],1,&allPlayers[3],1,NULL,3,2,false);
					}
					else
					{
						newCur = calcNewRating(&myPlayer,2,&allPlayers[0],1,&allPlayers[2],1,NULL,3,2,false);
						newLife = calcNewRating(&myPlayer,2,&allPlayers[0],1,&allPlayers[2],1,NULL,3,2,false);
					}
				}

				//loses half the normal amount on rating for quitting

				newLife = (newLife + myPlayer.lifetimeRating) / 2;
				newCur = (newCur + myPlayer.currentRating) / 2;

				m_db->playerUpdateRating(pid,newLife,newCur);
			}
	}


	int SWDBLayer::calcNewRating( 
		SWDBPlayer* pid, int pidPos, SWDBPlayer* p2, int p2Pos, SWDBPlayer* p3, 
		int p3Pos, SWDBPlayer* p4, int p4Pos, int numPlayers, bool lifeRating )
	{
		std::pair<SWDBPlayer*,int> players[3];
		players[0] = std::make_pair(p2,p2Pos);
		players[1] = std::make_pair(p3,p3Pos);
		players[2] = std::make_pair(p4,p4Pos);

		int ratings[3];

		float r1 = lifeRating ? pid->lifetimeRating : pid->currentRating;
//		float n1 = lifeRating ? pid->lifetimeGamesCompleted : pid->currentGamesCompleted;
		for(int i = 0; i < numPlayers; i++)
		{
			float r2 = players[i].first->lifetimeRating;
//			float n2 = players[i].first->lifetimeGamesCompleted;
			float o = 0.0f;
			if(pidPos < players[i].second)
				o = 1.0f;
			else if(pidPos == players[i].second)
				o = 0.0f;
			else
				o = -1.0f;

			float w = 0.0f;
			if(pidPos < players[i].second)
				w = 1.0f;
			else if(pidPos == players[i].second)
				w = 0.5f;
			else
				w = 0.0f;

			ratings[i] = (int)(r1 + 32*(w-(1.0/(1.0+pow(10.0f,(float)(r2-r1)/400.0f)))));
			/*
			if(n2 >= 20)
			{
				ratings[i] = (int)(r1 + 32*(w-(1.0/(1.0+pow(10.0f,(float)(r2-r1)/400.0f)))));
			}
			else
			{
				float points = 16+(16*(n2/20));
				ratings[i] = (int)(r1 + points * (w-(1.0/(1.0+pow(10.0f, (float)(r2-r1)/400.0f)))));
			}
			*/
		}

		int ret = 0;
		for(int i = 0; i < numPlayers; i++)
		{
			ret += ratings[i];
		}
		ret /= numPlayers;
		return ret;

	}

	bool SWDBLayer::gameIsOver( ServerTable* t, const std::vector<int>& winners,
		const std::vector<int>& scores, const std::vector<int64>& pids,const std::vector<int64>& origids, std::vector<int>& newRatings )
	{
		if(t->getGameId() == -1)
			return false;
		newRatings.clear();
		std::vector<GOPlayer> players;
		m_db->getDB()->beginTransaction();
		for(size_t i = 0; i < winners.size(); ++i)
		{
			//partnership
			if(winners.size() == 2 && pids.size() == 4)
			{
				GOPlayer p1;
				GOPlayer p2;
				p1.m_teamNo = winners[i];
				p1.m_teamPlace = i + 1;
				p1.m_score = scores[i];
				p2.m_teamNo = winners[i];
				p2.m_teamPlace = i + 1;
				p2.m_score = scores[i];

				int start = 0;
				if(winners[i] == 1)
				{
					start = 1;
				}

				p1.m_id = pids[start];
				p2.m_id = pids[(start + 2) % 4];
				p1.m_origID = origids[start];
				p2.m_origID = origids[(start + 2) % 4];

				players.push_back(p1);
				players.push_back(p2);
			}
			else
			{
				GOPlayer p;
				p.m_teamNo = i;
				for(int j = 0; j < winners.size(); j++)
				{
					if(winners[j] == i)
					{
						p.m_teamPlace = j + 1;
						p.m_score = scores[j];
						break;
					}
				}
				p.m_id = pids[i];
				p.m_origID = origids[i];
				players.push_back(p);
			}
		}

		if(players.empty())
		{
			m_db->getDB()->endTransaction(false);
			return false;
		}

		for(int i = 0; i < t->getNumChairs(); i++)
		{
			for(int j = 0; j < players.size(); j++)
			{
				if(t->getChairAt(i)->isOccupied() && players[j].m_id == t->getChairAt(i)->getPlayer()->getId())
				{
					players[j].m_origID = t->getChairAt(i)->getOriginalPid();

					if(!m_db->getPlayer(players[j].m_id,&players[j].m_player))
					{
						m_db->getDB()->endTransaction(false);
						return false;
					}

					
					players[j].m_ratePlayer = players[j].m_player;

					//sub
					if(players[j].m_origID != players[j].m_id)
					{
						if(!m_db->getPlayer(players[j].m_origID,&players[j].m_ratePlayer))
						{
							m_db->getDB()->endTransaction(false);
							return false;
						}

						players[i].m_ratePlayer.currentGamesCompleted = 
							(players[i].m_ratePlayer.currentGamesCompleted + players[i].m_player.currentGamesCompleted) / 2;
						players[i].m_ratePlayer.lifetimeGamesCompleted = 
							(players[i].m_ratePlayer.lifetimeGamesCompleted + players[i].m_player.lifetimeGamesCompleted) / 2;
						players[i].m_ratePlayer.lifetimeRating = 
							(players[i].m_ratePlayer.lifetimeRating + players[i].m_player.lifetimeRating) / 2;
						players[i].m_ratePlayer.currentRating = 
							(players[i].m_ratePlayer.currentRating + players[i].m_player.currentRating) / 2;
					}
				}
			}
		}

		if(t->getInfo().isRated())
		{
			if(winners.size() == 2)
			{
				for(int i = 0; i < players.size(); i+= 2)
				{
					players[(0 + i) % (players.size())].m_curRating = 
						calcNewRating(
						&players[(0 + i) % (players.size())].m_player,
						players[(0 + i) % (players.size())].m_teamPlace,
						&players[(2 + i) % (players.size())].m_ratePlayer,
						players[(2 + i) % (players.size())].m_teamPlace,
						&players[(3 + i) % (players.size())].m_ratePlayer,
						players[(3 + i) % (players.size())].m_teamPlace,
						NULL,4,2,false);
					players[(0 + i) % (players.size())].m_lifeRating = 
						calcNewRating(
						&players[(0 + i) % (players.size())].m_player,
						players[(0 + i) % (players.size())].m_teamPlace,
						&players[(2 + i) % (players.size())].m_ratePlayer,
						players[(2 + i) % (players.size())].m_teamPlace,
						&players[(3 + i) % (players.size())].m_ratePlayer,
						players[(3 + i) % (players.size())].m_teamPlace,
						NULL,4,2,true);

					players[(1 + i) % (players.size())].m_curRating = 
						calcNewRating(
						&players[(1 + i) % (players.size())].m_player,
						players[(1 + i) % (players.size())].m_teamPlace,
						&players[(2 + i) % (players.size())].m_ratePlayer,
						players[(2 + i) % (players.size())].m_teamPlace,
						&players[(3 + i) % (players.size())].m_ratePlayer,
						players[(3 + i) % (players.size())].m_teamPlace,
						NULL,4,2,false);

					players[(1 + i) % (players.size())].m_lifeRating = 
						calcNewRating(
						&players[(1 + i) % (players.size())].m_player,
						players[(1 + i) % (players.size())].m_teamPlace,
						&players[(2 + i) % (players.size())].m_ratePlayer,
						players[(2 + i) % (players.size())].m_teamPlace,
						&players[(3 + i) % (players.size())].m_ratePlayer,
						players[(3 + i) % (players.size())].m_teamPlace,
						NULL,4,2,true);
				}
			}
			else if(winners.size() == 3)
			{
				for(int i = 0; i < players.size(); i++)
				{
					players[(0 + i) % (players.size())].m_curRating = calcNewRating(&players[(0 + i) % (players.size())].m_player,players[(0 + i) % (players.size())].m_teamPlace,&players[(1 + i) % (players.size())].m_ratePlayer,
						players[(1 + i) % (players.size())].m_teamPlace,&players[(2 + i) % (players.size())].m_ratePlayer,players[(2 + i) % (players.size())].m_teamPlace,NULL,4,2,false);
					players[(0 + i) % (players.size())].m_lifeRating = calcNewRating(&players[(0 + i) % (players.size())].m_player,players[(0 + i) % (players.size())].m_teamPlace,&players[(1 + i) % (players.size())].m_ratePlayer,
						players[(1 + i) % (players.size())].m_teamPlace,&players[(2 + i) % (players.size())].m_ratePlayer,players[(2 + i) % (players.size())].m_teamPlace,NULL,4,2,true);
				}
			}
			else if(winners.size() == 4)
			{
				for(int i = 0; i < players.size(); i++)
				{
					players[(0 + i) % (players.size())].m_curRating = calcNewRating(&players[(0 + i) % (players.size())].m_player,players[(0 + i) % (players.size())].m_teamPlace,&players[(1 + i) % (players.size())].m_ratePlayer,
						players[(1 + i) % (players.size())].m_teamPlace,&players[(2 + i) % (players.size())].m_ratePlayer,players[(2 + i) % (players.size())].m_teamPlace,&players[(3 + i) % (players.size())].m_ratePlayer,players[(3 + i) % (players.size())].m_teamPlace,3,false);

					players[(0 + i) % (players.size())].m_lifeRating = calcNewRating(&players[(0 + i) % (players.size())].m_player,players[(0 + i) % (players.size())].m_teamPlace,&players[(1 + i) % (players.size())].m_ratePlayer,
						players[(1 + i) % (players.size())].m_teamPlace,&players[(2 + i) % (players.size())].m_ratePlayer,players[(2 + i) % (players.size())].m_teamPlace,&players[(3 + i) % (players.size())].m_ratePlayer,players[(3 + i) % (players.size())].m_teamPlace,3,false);
				}
			}
		}
		else
		{
			for(int i = 0; i < players.size(); i++)
			{
				players[i].m_lifeRating = players[i].m_player.lifetimeRating;
				players[i].m_curRating = players[i].m_player.currentRating;
			}
		}

		if(t->getInfo().isRated())
		{
			for(int i = 0; i < players.size(); i++)
			{
				
				//subs that lose only lose 1/4 the points
				if(!(players[i].m_id != players[i].m_origID && players[i].m_teamPlace != 1))
				{
					//players[i].m_curRating = players[i].m_curRating + ((players[i].m_curRating - players[i].m_player.currentRating) * 0);
					//players[i].m_lifeRating = players[i].m_lifeRating + ((players[i].m_lifeRating - players[i].m_player.lifetimeRating) * 0);
					m_db->playerUpdateRating(players[i].m_id,players[i].m_lifeRating,players[i].m_curRating);
				}
				
			}
		}

		for(int i = 0; i < players.size(); i++)
		{
			if(!(players[i].m_id != players[i].m_origID && players[i].m_teamPlace != 1))
			{
				if(t->getInfo().isIndividual() && players[i].m_teamPlace == 1)
					m_db->playerUpdateCompletedGame(players[i].m_id,1,0,0,0,0,0,0,0,0,0,0,true);
				else if(t->getInfo().isIndividual() && players[i].m_teamPlace == 2)
					m_db->playerUpdateCompletedGame(players[i].m_id,0,1,0,0,0,0,0,0,0,0,0,true);
				else if(t->getInfo().isIndividual() && players[i].m_teamPlace == 3)
					m_db->playerUpdateCompletedGame(players[i].m_id,0,0,1,0,0,0,0,0,0,0,0,true);
				else if(t->getInfo().isIndividual() && players[i].m_teamPlace == 4)
					m_db->playerUpdateCompletedGame(players[i].m_id,0,0,0,1,0,0,0,0,0,0,0,true);
				else if(t->getInfo().isCutthroat() && players[i].m_teamPlace == 1)
					m_db->playerUpdateCompletedGame(players[i].m_id,0,0,0,0,1,0,0,0,0,0,0,true);
				else if(t->getInfo().isCutthroat() && players[i].m_teamPlace == 2)
					m_db->playerUpdateCompletedGame(players[i].m_id,0,0,0,0,0,1,0,0,0,0,0,true);
				else if(t->getInfo().isCutthroat() && players[i].m_teamPlace == 3)
					m_db->playerUpdateCompletedGame(players[i].m_id,0,0,0,0,0,0,1,0,0,0,0,true);
				else if(t->getInfo().isPassTwo() && players[i].m_teamPlace == 1)
					m_db->playerUpdateCompletedGame(players[i].m_id,0,0,0,0,0,0,0,0,0,1,0,true);
				else if(t->getInfo().isPassTwo() && players[i].m_teamPlace == 2)
					m_db->playerUpdateCompletedGame(players[i].m_id,0,0,0,0,0,0,0,0,0,0,1,true);
				else if(t->getInfo().isPartnership() && players[i].m_teamPlace == 1)
					m_db->playerUpdateCompletedGame(players[i].m_id,0,0,0,0,0,0,0,1,0,0,0,true);
				else if(t->getInfo().isPartnership() && players[i].m_teamPlace == 2)
					m_db->playerUpdateCompletedGame(players[i].m_id,0,0,0,0,0,0,0,0,1,0,0,true);
			}

		}

		for(int i = 0; i < players.size(); i++)
		{
			int res = -1;
			m_db->playerGameCreate(t->getGameId(),
				players[i].m_teamNo,players[i].m_score,players[i].m_id != players[i].m_origID,
				players[i].m_teamPlace,players[i].m_id
				,&players[i].m_pgid,&res);

			if(res != 1)
			{
				players[i].m_pgid = -1;
			}
		}

		if(players.size() == 4)
		m_db->gameLinkPlayerGames(t->getGameId(),players[0].m_pgid,players[1].m_pgid,players[2].m_pgid,players[3].m_pgid);
		else if(players.size() == 3)
			m_db->gameLinkPlayerGames(t->getGameId(),players[0].m_pgid,players[1].m_pgid,players[2].m_pgid,-1);

		m_db->gameSetCompleted(t->getGameId());

		for(int i = 0; i < players.size(); i++)
		{
			if(players[i].m_id != players[i].m_origID)
			{
				m_db->playerSubstitutedGame(players[i].m_id);

				if(players[i].m_teamPlace == 1)
				{
					m_db->playerAddKarma(players[i].m_id,KARMA_FOR_SUB_WIN);
				}
				else
				{
					m_db->playerAddKarma(players[i].m_id,KARMA_FOR_SUB_LOSS);
				}
			}
			else
			{
				if(players[i].m_teamPlace == 1)
				{
					m_db->playerAddKarma(players[i].m_id,KARMA_FOR_WIN);
				}
				else
				{
					m_db->playerAddKarma(players[i].m_id,KARMA_FOR_LOSS);
				}
			}
		}

		for(int i = 0; i < pids.size(); i++)
		{
			for(int j = 0; j < players.size(); j++)
			{
				if(pids[i] == players[j].m_id)
				{
					newRatings.push_back(players[j].m_lifeRating);
					break;
				}
			}
		}

		m_db->getDB()->endTransaction(true);
		return true;
	}

	bool SWDBLayer::awardCoins( ServerTable* t, const std::vector<int>& winners, const std::vector<int64>& pids, 
		const std::vector<int64>& origpids, int64 subPayout, std::vector<int64>& netChange, std::vector<int64>& newCoinAmount,
		const std::vector<float>& payratesIndividual,const std::vector<float>& payratesCutthroat,const std::vector<float>& payratesPartnership,
		float houseCut)
	{
		if(t->getGameId() == -1)
			return false;
		netChange.clear();
		newCoinAmount.clear();
		std::vector<GOPlayer> players;
		m_db->getDB()->beginTransaction();
		for(size_t i = 0; i < winners.size(); ++i)
		{
			//partnership
			if(winners.size() == 2 && pids.size() == 4)
			{
				GOPlayer p1;
				GOPlayer p2;
				p1.m_teamNo = winners[i];
				p1.m_teamPlace = i + 1;
				p1.m_score = -1;
				p2.m_teamNo = winners[i];
				p2.m_teamPlace = i + 1;
				p2.m_score = -1;

				int start = 0;
				if(winners[i] == 1)
				{
					start = 1;
				}

				p1.m_id = pids[start];
				p2.m_id = pids[(start + 2) % 4];
				p1.m_origID = origpids[start];
				p2.m_origID = origpids[(start + 2) % 4];

				players.push_back(p1);
				players.push_back(p2);
			}
			else
			{
				GOPlayer p;
				p.m_teamNo = i;
				for(int j = 0; j < winners.size(); j++)
				{
					if(winners[j] == i)
					{
						p.m_teamPlace = j + 1;
						p.m_score = -1;
						break;
					}
				}
				p.m_id = pids[i];
				p.m_origID = origpids[i];
				players.push_back(p);
			}
		}

		if(players.empty())
		{
			m_db->getDB()->endTransaction(false);
			return false;
		}

		int netAmount = players.size() * t->getInfo().getFee() * (1.0f - houseCut);

		if(winners.size() == 2)
		{
			for(int i = 0; i < players.size(); i++)
			{
				int amountWon = (int)((netAmount * payratesPartnership[0]) + 0.5f);
				if(players[i].m_teamPlace == 1)
				{
					players[i].m_netCoinChange = amountWon;
					bool sub = players[i].m_id != players[i].m_origID;
					if(sub)
					{
						players[i].m_netCoinChange = subPayout;
					}
					else
					{
						players[i].m_netCoinChange = amountWon;
					}
				}
				else
				{
					players[i].m_netCoinChange = 0;
				}
			}
		}
		else if(winners.size() == 3)
		{
			for(int i = 0; i < players.size(); i++)
			{
				int amountFirst = (int)((netAmount * payratesCutthroat[0])+ 0.5f);
				int amountSecond = (int)((netAmount * payratesCutthroat[1])+ 0.5f);
				bool sub = players[i].m_id != players[i].m_origID;
				if(sub)
				{
					amountFirst = subPayout;
					amountSecond = 0;
				}
				if(players[i].m_teamPlace == 1)
				{
					players[i].m_netCoinChange = amountFirst;
				}
				else if(players[i].m_teamPlace == 2)
				{
					players[i].m_netCoinChange = amountSecond;
				}
				else
				{
					players[i].m_netCoinChange = 0;
				}
			}
		}
		else if(winners.size() == 4)
		{
			for(int i = 0; i < players.size(); i++)
			{
				int amountFirst = (int)((netAmount * payratesIndividual[0])+ 0.5f);
				int amountSecond = (int)((netAmount * payratesCutthroat[1])+ 0.5f);
				int amountThird = (int)((netAmount * payratesCutthroat[2])+ 0.5f);

				bool sub = players[i].m_id != players[i].m_origID;
				if(sub)
				{
					amountFirst = subPayout;
					amountSecond = 0;
					amountThird = 0;
				}

				if(players[i].m_teamPlace == 1)
				{
					players[i].m_netCoinChange = amountFirst;
				}
				else if(players[i].m_teamPlace == 2)
				{
					players[i].m_netCoinChange = amountSecond;
				}
				else if(players[i].m_teamPlace == 3)
				{
					players[i].m_netCoinChange = amountThird;
				}
				else
				{
					players[i].m_netCoinChange = 0;
				}
			}
		}

		for(int i = 0; i < players.size(); i++)
		{
			if(!m_db->playerAddCoins(players[i].m_id,players[i].m_netCoinChange,&players[i].m_newCoinAmount))
			{
				m_db->getDB()->endTransaction(false);
				return false;
			}
		}

		for(int i = 0; i < pids.size(); i++)
		{
			for(int j = 0; j < players.size(); j++)
			{
				if(pids[i] == players[j].m_id)
				{
					netChange.push_back(players[j].m_netCoinChange);
					newCoinAmount.push_back(players[j].m_newCoinAmount);
					break;
				}
			}
		}

		m_db->getDB()->endTransaction(true);
		return true;
	}
}
