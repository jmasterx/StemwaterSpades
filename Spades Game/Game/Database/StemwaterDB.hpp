#ifndef CGE_STEMWATER_DB_HPP
#define CGE_STEMWATER_DB_HPP
#include "Game/Database/Database.hpp"
#include "Game/Database/SWDBPlayer.hpp"
#include "Game/Database/DBEventEnum.h"
#include "Game/Database/SWDBStoreItem.hpp"
#include <vector>
namespace cge
{
	struct SWDBChatMessage {
		std::string message;
		std::string username;
	};
	class StemwaterDB
	{
		Database* m_db;
		std::string m_lastError;
		void setLastError(const sql::SQLException& e);
	public:
		StemwaterDB(void);
		const std::string& getLastError() const;
		void setDB(Database* db);
		Database* getDB();
		std::string createDate(int month, int day, int year);
		bool playerCreate(const std::string& firstName,
			const std::string& lastName, const std::string& username,
			const std::string& password,
			const std::string& email,
			int avatar,
			int64* playerID,
			int* result);
		bool gameCreate(const std::string& gameType, const std::string& bidRule, const std::string& endType, 
			int playTo, bool allowDeckCut, bool allowWatching, bool isRated, int fee, int64* gameID, int* result);
		bool gameLinkPlayerGames(int64 gameID, int64 first, int64 second, int64 third, int64 fourth);
		bool gameSetCompleted(int64 gameID);
		bool playerGameCreate(int64 gameID, int teamNo, int finalScore, bool wasSub,
			int teamPlace, int64 playerID, int64* playerGameID, int* result);
		bool playerAddKarma(int64 playerID, int amount);
		bool playerResetRating(int64 playerID);
		bool playerSetAccountDisabled(int64 playerID, bool disabled);
		bool playerLogin(const std::string& username, const std::string& password, int64* playerID, int* result);
		bool playerUpdateCompletedGame(int64 playerID, int firstIndividual,int secondIndividual,int thirdIndividual,int fourthIndividual,
			int firstCutthroat, int secondCutthroat,int thirdCutthroat,int firstPartnership,
			int secondPartnership,int firstPass2,int secondPass2, bool completed);
		bool playerUpdateRating(int64 playerID, int newOverall, int newCurrent);
		bool playerSuspendAccount(int64 playerID, int minutes);
		bool playerUnsuspendAccount(int64 playerID);
		bool playerSetAvatar(int64 playerID, int avatar);
		bool playerSetAdministrator(int64 playerID,bool isAdmin);
		bool chatAddEntry(int64 playerID, const std::string& area, const std::string& message);
		bool chatCleanup(int olderThanDays);
		bool getPlayer(int64 id, SWDBPlayer* player);
		bool playerSuspendChat(int64 playerID, int minutes);
		bool playerUnsuspendChat(int64 playerID);
		bool playerSetChatDisabled(int64 playerID, bool disabled);
		bool playerSetEmailVerified(int64 playerID, bool verified);
		bool playerAddFriend(int64 playerID, int64 friendID);
		bool playerRemoveFriend(int64 playerID, int64 friendID);
		bool playerGetFriendNames(int64 playerID, SWDBPlayer* player);
		bool playerGetFriendNames(int64 playerID, std::set<std::string>& friends );
		bool eventAddEntry(int64 playerID, DBEventEnum eventID, const std::string& text = "");
		bool eventCleanup(int olderThanDays);
		bool playerChangePassword(int64 pid, const std::string& oldPassword, const std::string& newPassword, int* result);
		bool playerResetPassword(int64 pid, const std::string& newPassword, int* result);
		bool playerChangeEmail(int64 pid, const std::string& email, int* result);
		bool playerSubstitutedGame(int64 pid);
		bool playerAbandonedGame(int64 pid);
		bool playerAcceptedTerms(int64 pid);
		bool playerIdFromName(const std::string& name, int64* id);
		bool playerSetShowEmail(int64 pid, bool want);
		bool playerSetName(int64 pid, const std::string& first, const std::string& last);
		bool getLeaderboardData(std::vector<std::string> &rank, std::vector<std::string>& names, std::vector<std::string>& rating, int pageNo, int numPerPage, int gameLimit, const std::string& field);
		bool playerSetTheme(int64 playerID, int theme);
		bool playerSpendCoins(int64 pid, int64 cost,int64* newAmount);
		bool playerAddCoins(int64 pid, int64 amount,int64* newAmount);
		bool playerSetTempCoins(int64 pid, int64 amount);
		bool playerRefundTempCoins(int64 pid, int64* amountRefunded);
		bool playerGetCoinAmount(int64 pid, int64* amount);
		bool playerTryReceiveDailyBonus(int64 pid, int64 amount, float hours, int64* amountReceived);
		bool feedbackAddEntry(int64 pid, const std::string& otherPlayer, const std::string& description, const std::string& data);
		bool tokenCreate(const std::string& data, int minutes, std::string* token);
		bool storeItemsEnabled(std::vector<SWDBStoreItem>& items);
		bool tableFeesGet(std::vector<int>& items);
		bool playerGetRating(int64 pid, int* rating);
		bool getSetting(const std::string& key, std::string& val);
		int getIntSetting(const std::string& key);
		float getFloatSetting(const std::string& key);
		bool leaguePlayerIsMember(int leagueID, int64 pid, bool* result);
		bool chatFromLastN(int minutes, std::vector<SWDBChatMessage>& messages);
		~StemwaterDB(void);
	};
}
#endif
