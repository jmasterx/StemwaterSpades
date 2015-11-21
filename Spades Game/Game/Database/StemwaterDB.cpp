#include "Game/Database/StemwaterDB.hpp"
#include <sstream>

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
	( std::ostringstream() << std::dec << x ) ).str()
namespace cge
{
	StemwaterDB::StemwaterDB(void)
		: m_db(NULL)
	{

	}

	StemwaterDB::~StemwaterDB(void)
	{
	}

	void StemwaterDB::setDB( Database* db )
	{
		m_db = db;
	}

	Database* StemwaterDB::getDB()
	{
		return m_db;
	}

	bool StemwaterDB::playerCreate(
		const std::string& firstName, const std::string& lastName, const std::string& username, const std::string& password,
		const std::string& email, int avatar, int64* playerID, int* result )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_create(?,?,?,?,?,?,@pid,@res)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setString(1,firstName);
			stmt->setString(2,lastName);
			stmt->setString(3,username);
			stmt->setString(4,password);
			stmt->setString(5,email);
			stmt->setInt(6,avatar);
			stmt->execute();

			std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @pid AS a, @res AS b"));
			while (res->next())
			{
				*playerID = res->getInt64("a");
				*result = res->getInt("b");
			}

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::gameCreate( const std::string& gameType, const std::string& bidRule, const std::string& endType, int playTo,
		bool allowDeckCut, bool allowWatching, bool isRated, int fee, int64* gameID, int* result )
	{
		{
			sql::Connection* con = NULL;
			try
			{
				con = getDB()->getConnection();
				std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL game_create(?,?,?,?,?,?,?,?,@gid,@res)"));
				std::auto_ptr<sql::Statement> stt(con->createStatement());
				stmt->setString(1,gameType);
				stmt->setString(2,bidRule);
				stmt->setString(3,endType);
				stmt->setInt(4,playTo);
				stmt->setBoolean(5,allowDeckCut);
				stmt->setBoolean(6,allowWatching);
				stmt->setBoolean(7,isRated);
				stmt->setInt(8,fee);
				stmt->execute();

				std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @gid AS a, @res AS b"));
				while (res->next())
				{
					*gameID = res->getInt64("a");
					*result = res->getInt("b");
				}

				if(con)
				{
					getDB()->releaseConnection(con);
					con = NULL;
				}
			}
			catch(sql::SQLException &e){
				if(con)
				{
					getDB()->releaseConnection(con);
					getDB()->endTransaction(false);
					con = NULL;
				}
				setLastError(e);
				return false;
			}
			return true;
		}
	}

	bool StemwaterDB::gameLinkPlayerGames(int64 gameID, int64 first, int64 second, int64 third, int64 fourth )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL game_link_player_games(?,?,?,?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,gameID);
			if(first >= 0)
			{
				stmt->setInt64(2,first);
			}
			else
			{
				stmt->setNull(2,0);
			}
			if(second >= 0)
			{
				stmt->setInt64(3,second);
			}
			else
			{
				stmt->setNull(3,0);
			}
			if(third >= 0)
			{
				stmt->setInt64(4,third);
			}
			else
			{
				stmt->setNull(4,0);
			}
			if(fourth >= 0)
			{
				stmt->setInt64(5,fourth);
			}
			else
			{
				stmt->setNull(5,0);
			}
			stmt->execute();
			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerGameCreate( int64 gameID, int teamNo, int finalScore, bool wasSub, int teamPlace, int64 playerID, int64* playerGameID, int* result )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_game_create(?,?,?,?,?,?,@pgid,@res)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,gameID);
			stmt->setInt(2,teamNo);
			stmt->setInt(3,finalScore);
			stmt->setBoolean(4,wasSub);
			stmt->setInt(5,teamPlace);
			stmt->setInt64(6,playerID);
			stmt->execute();

			std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @pgid AS a, @res AS b"));
			while (res->next())
			{
				*playerGameID = res->getInt("a");
				*result = res->getInt("b");
			}

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::gameSetCompleted( int64 gameID )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL game_set_completed(?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,gameID);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerAddKarma( int64 playerID, int amount )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_add_karma(?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->setInt(2,amount);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerResetRating( int64 playerID )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_reset_rating(?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerSetAccountDisabled( int64 playerID, bool disabled )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_set_account_disabled(?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->setBoolean(2,disabled);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerLogin( const std::string& username, const std::string& password, int64* playerID, int* result )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_login(?,?,@pid,@res)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setString(1,username);
			stmt->setString(2,password);
			stmt->execute();

			std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @pid AS a, @res AS b"));
			while (res->next())
			{
				*playerID = res->getInt64("a");
				*result = res->getInt("b");
			}

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerUpdateCompletedGame( 
		int64 playerID, int firstIndividual,int secondIndividual,int thirdIndividual,int fourthIndividual, 
		int firstCutthroat, int secondCutthroat,int thirdCutthroat,int firstPartnership, int secondPartnership,
		int firstPass2,int secondPass2, bool completed )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_update_completed_game(?,?,?,?,?,?,?,?,?,?,?,?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->setInt(2,firstIndividual);
			stmt->setInt(3,secondIndividual);
			stmt->setInt(4,thirdIndividual);
			stmt->setInt(5,fourthIndividual);
			stmt->setInt(6,firstCutthroat);
			stmt->setInt(7,secondCutthroat);
			stmt->setInt(8,thirdCutthroat);
			stmt->setInt(9,firstPartnership);
			stmt->setInt(10,secondPartnership);
			stmt->setInt(11,firstPass2);
			stmt->setInt(12,secondPass2);
			stmt->setBoolean(13,completed);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerUpdateRating( int64 playerID, int newOverall, int newCurrent )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_update_rating(?,?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->setInt(2,newOverall);
			stmt->setInt(3,newCurrent);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerSuspendAccount( int64 playerID, int minutes )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_suspend_account(?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->setInt(2,minutes);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerUnsuspendAccount( int64 playerID )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_unsuspend_account(?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerSetAvatar( int64 playerID, int avatar )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_set_avatar(?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->setInt(2,avatar);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerSetAdministrator( int64 playerID,bool isAdmin )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_set_administrator(?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->setBoolean(2,isAdmin);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::chatAddEntry( int64 playerID, const std::string& area, const std::string& message )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL chat_add_entry(?,?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->setString(2,area);
			stmt->setString(3,message);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::chatCleanup( int olderThanDays )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL chat_cleanup(?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt(1,olderThanDays);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::getPlayer( int64 id, SWDBPlayer* player )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::Statement> stmt(con->createStatement());
			stmt->execute("CALL player_from_id(" + SSTR(id) + ")");

			std::auto_ptr< sql::ResultSet > res;
			do {
				res.reset(stmt->getResultSet());
				while (res->next()) {
					player->id = res->getInt64("PlayerID");
					player->firstName = res->getString("PlayerFirstName");
					player->lastName = res->getString("PlayerLastName");
					player->password = res->getString("PlayerPassword");
					player->name = res->getString("PlayerName");
					player->email = res->getString("PlayerEmail");
					player->emailIsVerified = res->getBoolean("PlayerEmailIsVerified");
					player->avatar = res->getInt("PlayerAvatar");
					player->joinDate.setDateTimeFromSQL(res->getString("PlayerJoinDate"));
					player->isAdministrator = res->getBoolean("PlayerIsAdministrator");
					player->karmaPoints = res->getInt("PlayerKarmaPoints");
					player->currentRating = res->getInt("PlayerCurrentRating");
					player->currentWins = res->getInt("PlayerCurrentWins");
					player->currentLoses = res->getInt("PlayerCurrentLoses");
					player->currentGamesCompleted = res->getInt("PlayerCurrentGamesCompleted");
					player->currentGamesAbandoned = res->getInt("PlayerCurrentGamesAbandoned");
					player->currentGamesSubstituted = res->getInt("PlayerCurrentGamesSubstituted");
					player->currentHighestRating = res->getInt("PlayerCurrentHighestRating");

					player->lifetimeRating = res->getInt("PlayerLifetimeRating");
					player->lifetimeWins = res->getInt("PlayerLifetimeWins");
					player->lifetimeLoses = res->getInt("PlayerLifetimeLoses");
					player->lifetimeGamesCompleted = res->getInt("PlayerLifetimeGamesCompleted");
					player->lifetimeGamesAbandoned = res->getInt("PlayerLifetimeGamesAbandoned");
					player->lifetimeGamesSubstituted = res->getInt("PlayerLifetimeGamesSubstituted");
					player->lifetimeHighestRating = res->getInt("PlayerLifetimeHighestRating");
					
					std::string lastRatingReset = res->getString("PlayerLastRatingResetDate");

					if(lastRatingReset.length() > 0)
					{
						DateTime dt;
						dt.setDateTimeFromSQL(lastRatingReset);

						player->lastRatingResetDate = dt;
					}
					else
					{
						player->lastRatingResetDate.reset();
					}

					player->ratingResetCount = res->getInt("PlayerRatingResetCount");
					player->firstPlaceSingleCount = res->getInt("PlayerFirstPlaceSingleCount");
					player->secondPlaceSingleCount = res->getInt("PlayerSecondPlaceSingleCount");
					player->thirdPlaceSingleCount = res->getInt("PlayerThirdPlaceSingleCount");
					player->fourthPlaceSingleCount = res->getInt("PlayerFourthPlaceSingleCount");
					player->partnershipWinCount = res->getInt("PlayerPartnershipWinCount");
					player->partnershipLossCount = res->getInt("PlayerPartnershipLossCount");
					player->pass2WinCount = res->getInt("PlayerPass2WinCount");
					player->pass2LossCount = res->getInt("PlayerPass2LossCount");
					player->firstPlaceCutthroatCount = res->getInt("PlayerFirstPlaceCutthroatCount");
					player->secondPlaceCutthroatCount = res->getInt("PlayerSecondPlaceCutthroatCount");
					player->thirdPlaceCutthroatCount = res->getInt("PlayerThirdPlaceCutthroatCount");

					std::string chatSuspendedEndDate = res->getString("PlayerChatSuspendedEndDate");

					if(chatSuspendedEndDate.length() > 0)
					{
						DateTime dt;
						dt.setDateTimeFromSQL(chatSuspendedEndDate);

						player->chatSuspendEndDate = dt;
					}
					else
					{
						player->chatSuspendEndDate = boost::none;
					}

					std::string lastLoginDate = res->getString("PlayerLastLoginDate");

					if(lastLoginDate.length() > 0)
					{
						DateTime dt;
						dt.setDateTimeFromSQL(lastLoginDate);

						player->lastLoginDate = dt;
					}
					else
					{
						player->lastLoginDate = boost::none;
					}

					std::string lastCompletedDate = res->getString("PlayerLastGameCompletedDate");

					if(lastCompletedDate.length() > 0)
					{
						DateTime dt;
						dt.setDateTimeFromSQL(lastCompletedDate);

						player->lastGameCompletedDate = dt;
					}
					else
					{
						player->lastGameCompletedDate = boost::none;
					}

					std::string playerSuspendedEndDate = res->getString("PlayerSuspendedEndDate");

					if(playerSuspendedEndDate.length() > 0)
					{
						DateTime dt;
						dt.setDateTimeFromSQL(playerSuspendedEndDate);

						player->suspendedEndDate = dt;
					}
					else
					{
						player->suspendedEndDate = boost::none;
					}

					player->chatIsDisabled = res->getBoolean("PlayerChatIsDisabled");
					player->isDisabled = res->getBoolean("PlayerIsDisabled");
					player->loginCount = res->getInt("PlayerLoginCount");
					player->loginAttemptCount = res->getInt("PlayerLoginAttemptCount");

					player->acceptedTerms = res->getBoolean("PlayerAcceptedTerms");
					player->showEmail = res->getBoolean("PlayerShowEmail");
					player->theme = res->getInt("PlayerPremiumTheme");
					player->coins = res->getInt64("PlayerCoins");
					player->tempCoins = res->getInt64("PlayerTemporaryCoins");

					std::string lastReceivedCoins = res->getString("PlayerLastCoinReceiveDate");

					if(lastReceivedCoins.length() > 0)
					{
						DateTime dt;
						dt.setDateTimeFromSQL(lastReceivedCoins);

						player->lastCoinReceiveDate = dt;
					}
				}
			} while (stmt->getMoreResults());

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerSuspendChat( int64 playerID, int minutes )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_suspend_chat(?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->setInt(2,minutes);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerUnsuspendChat( int64 playerID )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_unsuspend_chat(?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerSetChatDisabled( int64 playerID, bool disabled )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_set_chat_disabled(?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->setBoolean(2,disabled);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerSetEmailVerified( int64 playerID, bool verified )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_set_email_verified(?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->setBoolean(2,verified);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerAddFriend( int64 playerID, int64 friendID )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_add_friend(?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->setInt64(2,friendID);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerRemoveFriend( int64 playerID, int64 friendID )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_remove_friend(?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->setInt64(2,friendID);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerGetFriendNames( int64 playerID, SWDBPlayer* player )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::Statement> stmt(con->createStatement());
			stmt->execute("CALL player_get_friend_names(" + SSTR(playerID) + ")");

			std::auto_ptr< sql::ResultSet > res;
			player->friends.clear();
			do {
				res.reset(stmt->getResultSet());
				player->friends.clear();
				while (res->next()) {
					std::string n = res->getString("PlayerName");
					player->friends.insert(n);
				}
			} while (stmt->getMoreResults());

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerGetFriendNames( int64 playerID, std::set<std::string>& friends )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::Statement> stmt(con->createStatement());
			stmt->execute("CALL player_get_friend_names(" + SSTR(playerID) + ")");

			std::auto_ptr< sql::ResultSet > res;
			friends.clear();
			do {
				res.reset(stmt->getResultSet());
			  friends.clear();
				while (res->next()) {
					std::string n = res->getString("PlayerName");
					friends.insert(n);
				}
			} while (stmt->getMoreResults());

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::eventAddEntry( int64 playerID, DBEventEnum eventID, const std::string& text /*= ""*/ )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL event_add_entry(?,?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt(1,(int)eventID);
			if(playerID >= 0)
				stmt->setInt64(2,playerID);
			else
				stmt->setNull(2,0);
			stmt->setString(3,text);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::eventCleanup( int olderThanDays )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL event_cleanup(?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt(1,olderThanDays);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}

			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerChangePassword( 
		int64 pid, const std::string& oldPassword, const std::string& newPassword, int* result )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_change_password(?,?,?,@res)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,pid);
			stmt->setString(2,oldPassword);
			stmt->setString(3,newPassword);
			stmt->execute();

			std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @res AS b"));
			while (res->next())
			{
				*result = res->getInt("b");
			}

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	const std::string& StemwaterDB::getLastError() const
	{
		return m_lastError;
	}

	void StemwaterDB::setLastError( const sql::SQLException& e )
	{
		std::cout << e.what() << std::endl;
		m_lastError = e.what();
	}

	bool StemwaterDB::playerChangeEmail( int64 pid, const std::string& email, int* result )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_change_email(?,?,@res)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,pid);
			stmt->setString(2,email);
			stmt->execute();

			std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @res AS b"));
			while (res->next())
			{
				*result = res->getInt("b");
			}

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerSubstitutedGame( int64 pid )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_substituted_game(?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,pid);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}

			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerAbandonedGame( int64 pid )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_abandoned_game(?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,pid);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}

			setLastError(e);
			return false;
		}
		return true;
	}

	std::string StemwaterDB::createDate( int month, int day, int year )
	{
		std::string ss;
		ss += SSTR(year);
		ss += "-";
		
		if(month < 10)
			ss += "0";
		ss += SSTR(month);
			ss += "-";

		if( day < 10)
			ss += "0";
		ss += SSTR(day);
		ss += " 00:00:00";
		std::string st = ss;
		return st;
	}

	bool StemwaterDB::playerAcceptedTerms( int64 pid )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_accepted_terms(?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,pid);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerIdFromName( const std::string& name, int64* id )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_id_from_name(?,@res)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setString(1,name);
			stmt->execute();

			std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @res AS b"));
			while (res->next())
			{
				*id = res->getInt64("b");
			}

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerSetShowEmail( int64 pid, bool want )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_set_show_email(?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,pid);
			stmt->setBoolean(2,want);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerSetName( int64 pid, const std::string& first, const std::string& last )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_set_name(?,?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,pid);
			stmt->setString(2,first);
			stmt->setString(3,last);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerResetPassword( int64 pid, const std::string& newPassword, int* result )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_reset_password(?,?,@res)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,pid);
			stmt->setString(3,newPassword);
			stmt->execute();

			std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @res AS b"));
			while (res->next())
			{
				*result = res->getInt("b");
			}

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::getLeaderboardData( std::vector<std::string> &rank, 
		std::vector<std::string>& names, std::vector<std::string>& rating, int pageNo, int numPerPage, int gameLimit, const std::string& field )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::Statement> stmt(con->createStatement());
			stmt->execute("CALL leaderboard_get_data(" + SSTR(numPerPage) + "," + SSTR(pageNo) + "," + SSTR(gameLimit) + ",'" + field  + "')");

			std::auto_ptr< sql::ResultSet > res;
			do {
				res.reset(stmt->getResultSet());
				rank.clear();
				rating.clear();
				names.clear();
				while (res->next()) {
					int theRank = res->getInt("PlayerRank");
					std::string rankStr = SSTR(theRank);
					long long theRating = res->getInt64("Obj");
					std::string ratingStr = SSTR(theRating);
					std::string name = res->getString("PlayerName");
					rank.push_back(rankStr);
					names.push_back(name);
					rating.push_back(ratingStr);
				}
			} while (stmt->getMoreResults());

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerSetTheme( int64 playerID, int theme )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_set_theme(?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,playerID);
			stmt->setInt(2,theme);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerSpendCoins( int64 pid, int64 cost,int64* newAmount )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_spend_coins(?,?,@res)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,pid);
			stmt->setInt64(2,cost);
			stmt->execute();

			std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @res AS b"));
			while (res->next())
			{
				*newAmount = res->getInt64("b");
			}

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerAddCoins( int64 pid, int64 amount,int64* newAmount )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_add_coins(?,?,@res)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,pid);
			stmt->setInt64(2,amount);
			stmt->execute();

			std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @res AS b"));
			while (res->next())
			{
				*newAmount = res->getInt("b");
			}

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerSetTempCoins( int64 pid, int64 amount )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_set_temp_coins(?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,pid);
			stmt->setInt64(2,amount);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerRefundTempCoins( int64 pid, int64* amountRefunded )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_refund_temp_coins(?,@res)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,pid);
			stmt->execute();

			std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @res AS b"));
			while (res->next())
			{
				*amountRefunded = res->getInt64("b");
			}

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerGetCoinAmount( int64 pid, int64* amount )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_get_coin_amount(?,@res)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,pid);
			stmt->execute();

			std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @res AS b"));
			while (res->next())
			{
				*amount = res->getInt64("b");
			}

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerTryReceiveDailyBonus( int64 pid, int64 amount, float hours, int64* amountReceived )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_try_receive_daily_bonus(?,?,?,@res)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,pid);
			stmt->setInt64(2,amount);
			stmt->setDouble(3,hours);
			stmt->execute();

			std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @res AS b"));
			while (res->next())
			{
				*amountReceived = res->getInt64("b");
			}

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::feedbackAddEntry( int64 pid, const std::string& otherPlayer, const std::string& description, const std::string& data )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL feedback_add_entry(?,?,?,?)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,pid);
			stmt->setString(2,otherPlayer);
			stmt->setString(3,description);
			stmt->setString(4,data);
			stmt->execute();

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::tokenCreate( const std::string& data, int minutes, std::string* token )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL token_create(?,?,@res)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setString(1,data);
			stmt->setInt(2,minutes);
			stmt->execute();

			std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @res AS b"));
			while (res->next())
			{
				*token = res->getString("b");
			}

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::storeItemsEnabled( std::vector<SWDBStoreItem>& items )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::Statement> stmt(con->createStatement());
			stmt->execute("CALL store_items_get_enabled('p')");

			std::auto_ptr< sql::ResultSet > res;
			do {
				res.reset(stmt->getResultSet());
				items.clear();
				while (res->next()) {
					items.push_back(SWDBStoreItem());
					SWDBStoreItem* item = &items.back();
					item->m_id = res->getInt64("StoreItemID");
					item->m_name = res->getString("StoreItemName");
					item->m_price = res->getString("StoreItemPrice");
					item->m_type = res->getString("StoreItemType");
					item->m_discountPercent = res->getString("StoreItemDiscountPercent");
					item->m_morePercent = res->getString("StoreItemMorePercent");
					item->m_coinValue = res->getInt64("StoreItemCoinValue");
					item->m_enabled = res->getBoolean("StoreItemEnabled");
				}
			} while (stmt->getMoreResults());

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::tableFeesGet( std::vector<int>& items )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::Statement> stmt(con->createStatement());
			stmt->execute("CALL table_fees_get()");

			std::auto_ptr< sql::ResultSet > res;
			do {
				res.reset(stmt->getResultSet());
				items.clear();
				while (res->next()) {
					items.push_back(res->getInt("TableFeeAmount"));
				}
			} while (stmt->getMoreResults());

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::playerGetRating( int64 pid, int* rating )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL player_get_life_rating(?,@res)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt64(1,pid);
			stmt->execute();

			std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @res AS b"));
			while (res->next())
			{
				*rating = res->getInt("b");
			}

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::getSetting( const std::string& key, std::string& val )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL setting_get(?,@res)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setString(1,key);
			stmt->execute();

			std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @res AS b"));
			while (res->next())
			{
				val = res->getString("b");
			}

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	int StemwaterDB::getIntSetting( const std::string& key )
	{
		std::string setting;
		getSetting(key,setting);
		int result = 0;
		std::stringstream ss;
		ss << setting;
		ss >> result;

		return result;
	}

	float StemwaterDB::getFloatSetting( const std::string& key )
	{
		std::string setting;
		getSetting(key,setting);
		float result = 0;
		std::stringstream ss;
		ss << setting;
		ss >> result;

		return result;
	}

	bool StemwaterDB::chatFromLastN( int minutes, std::vector<SWDBChatMessage>& messages )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::Statement> stmt(con->createStatement());
			std::stringstream ss;
			ss << minutes;
			stmt->execute("CALL chat_from_last_n(" + ss.str() + ")");

			std::auto_ptr< sql::ResultSet > res;
			do {
				res.reset(stmt->getResultSet());
				messages.clear();
				while (res->next()) {
					messages.push_back(SWDBChatMessage());
					SWDBChatMessage* item = &messages.back();
					item->username = res->getString("PlayerName");
					item->message = res->getString("ChatMessage");
				}
			} while (stmt->getMoreResults());

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

	bool StemwaterDB::leaguePlayerIsMember( int leagueID, int64 pid, bool* result )
	{
		sql::Connection* con = NULL;
		try
		{
			con = getDB()->getConnection();
			std::auto_ptr<sql::PreparedStatement> stmt(con->prepareStatement("CALL league_player_is_member(?,?,@res)"));
			std::auto_ptr<sql::Statement> stt(con->createStatement());
			stmt->setInt(1,leagueID);
			stmt->setInt64(2,pid);
			stmt->execute();

			std::auto_ptr<sql::ResultSet> res(stt->executeQuery("SELECT @res AS b"));
			while (res->next())
			{
				*result = res->getBoolean("b");
			}

			if(con)
			{
				getDB()->releaseConnection(con);
				con = NULL;
			}
		}
		catch(sql::SQLException &e){
			if(con)
			{
				getDB()->releaseConnection(con);
				getDB()->endTransaction(false);
				con = NULL;
			}
			setLastError(e);
			return false;
		}
		return true;
	}

}

