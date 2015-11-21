#include "Game/Net/NetSpadesGame.hpp"
#include "Game/Net/RawGameListener.hpp"
#include <GetTime.h>
using namespace cge;

class StatsMan : public RawGameListener {
	bool m_running;
	std::vector<int> roundBids;
	std::vector<int> bidChange;
	int nilsAttempted;
	int nilsSucceeded;

	void printNilData()
	{
		std::cout << "nil attempts: " << nilsAttempted << std::endl;
		std::cout << "nil succeeded: " << nilsSucceeded << std::endl;
		std::cout << "nil success rate: " << (float)nilsSucceeded / (float) (nilsAttempted + 0.001f) << std::endl;
	}

	void printBidData()
	{
		int rbSum = 0;
		int rbChangeSum = 0;
		int rblz = 0;
		for(int i = 0; i < roundBids.size();i++)
		{
			rbSum += roundBids[i];
		}

		for(int i = 0; i < bidChange.size();i++)
		{
			rbChangeSum += bidChange[i];
			if(bidChange[i] < 0)
				rblz++;
		}

		float avgRb = (float)rbSum / (float)roundBids.size() + 0.0001f;
		float avgRbCh = (float)rbChangeSum / (float)bidChange.size() + 0.0001f;
		float avgRbChz = (float)rblz / (float)bidChange.size() + 0.0001f;

		std::cout << "Average bid: " << avgRb << std::endl;
		std::cout << "Average bid delta: " << avgRbCh << std::endl;
		std::cout << "Percent bid l zero: " << avgRbChz << std::endl;
	}
public:
	StatsMan()
		: nilsAttempted(0),nilsSucceeded(0)
	{

	}
	virtual void gameIsOver(const std::vector<SpadesGame::Team>& teams, const std::vector<SpadesGame::Player>& players, const std::vector<int>& winningTeams, const std::vector<int>& winningScores)
	{
		std::cout << "Final Scores" << std::endl;
		for(size_t i = 0; i < teams.size(); i++)
		{
			std::cout << "Team " << (i + 1) << " " << teams[i].getScore() << std::endl;
		}
		m_running = false;
	}

	virtual void bidsReceived(const std::vector<SpadesGame::Team>& teams, const std::vector<SpadesGame::Player>& players)
	{
		int sum = 0;
		for(size_t i = 0; i < players.size(); i++)
		{
			sum += players[i].getRoundBid();
		}
		roundBids.push_back(sum);
	}

	virtual void roundEnded(const std::vector<SpadesGame::Team>& teams, const std::vector<SpadesGame::Player>& players)
	{
		for(size_t i = 0; i < players.size(); i++)
		{
			bidChange.push_back(players[i].getRoundTricks() - players[i].getRoundBid());

			if(players[i].getRoundBid() == 0)
				nilsAttempted++;

			if(players[i].getRoundBid() == 0 && players[i].getRoundTricks() == 0)
				nilsSucceeded++;
		}
	}
	void run()
	{
		int reps = 200;
		m_running = true;
		RakNet::TimeMS t1 =  RakNet::GetTimeMS();
		for(int i = 0; i < reps; i++)
		{
			NetSpadesGame game;
			game.setRawListener(this);
			SpadesGameInfo info = SpadesGameInfo(CUTTHROAT,SCORE_REACHED,DISALLOW_MOON_SHOT,NO_PASS,
				100,500,WANT_DECK_CUT,false,ANYONE_JOIN,false,true,0,1,0,0);
			game.setTableNo(1);
			game.sendGameInfo(info);
			game.setBots(info,true);
			while(m_running)
				game.flushBotCache();

			m_running = true;			
		}

		printBidData();
		printNilData();

		RakNet::TimeMS t2 = RakNet::GetTimeMS() - t1;

		std::cout << t2 / (float)reps << std::endl;

		roundBids.clear();
		bidChange.clear();
	}
};

int main()
{
	StatsMan sm;
	sm.run();
	return 0;
}