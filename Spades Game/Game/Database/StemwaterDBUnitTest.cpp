#include "Game/Database/StemwaterDBUnitTest.hpp"
#include <assert.h>
namespace cge
{
	StemwaterDBUnitTest::StemwaterDBUnitTest(void)
		:m_db(NULL)
	{
	}

	StemwaterDBUnitTest::~StemwaterDBUnitTest(void)
	{
	}

	void StemwaterDBUnitTest::executeUnit( bool val )
	{
		if(!val)
		{
			std::cout << "Unit Failed: " << m_db->getLastError() << std::endl;
		}
	}

	void StemwaterDBUnitTest::unitTest( StemwaterDB& db )
	{
		m_db = &db;
		int result = 0;
		int64 pid = 0;
		int64 loginID = 0;
		int64 gid = 0;
		SWDBPlayer player;

		executeUnit(db.playerCreate("Josh","Larouche","Bob","test","tom@hotmail.com",3,&pid,&result));
		assert(result == 1);
		assert(pid >= 0);
		executeUnit(db.getPlayer(pid,&player));
		assert(player.emailIsVerified == false);
		assert(player.name == "Bob");

		//INITIAL PLAYER LOGIN

		//player login, email unverified
		executeUnit(db.playerLogin("bob","test",&loginID,&result));
		assert(result == -4); //email not verified
		executeUnit(db.playerLogin("b","test",&loginID,&result));
		assert(result == -1); //invalid username
		executeUnit(db.playerLogin("bob","net",&loginID,&result));
		assert(result == -5); //invalid password (attempt count = 1)
		executeUnit(db.getPlayer(pid,&player));
		assert(player.loginAttemptCount == 1);

		//END INITIAL PLAYER LOGIN

		//BEGIN EMAIL VERIFIED

		executeUnit(db.playerSetEmailVerified(pid,true));
		executeUnit(db.getPlayer(pid,&player));
		assert(player.emailIsVerified == true);

		//END EMAIL VERIFIED

		//BEGIN PLAYER LOGIN

		//player login, email verified
		executeUnit(db.playerLogin("bob","test",&loginID,&result));
		assert(result == 1); //success (login count = 1)
		executeUnit(db.playerLogin("b","test",&loginID,&result));
		assert(result == -1); //invalid username
		executeUnit(db.playerLogin("bob","net",&loginID,&result));
		assert(result == -5); //invalid password (attempt count = 1)
		executeUnit(db.getPlayer(pid,&player));
		assert(player.loginAttemptCount == 1);

		//DISABLED
		executeUnit(db.playerSetAccountDisabled(pid,true));
		executeUnit(db.playerLogin("bob","test",&loginID,&result));
		assert(result == -3); //account disabled
		executeUnit(db.playerSetAccountDisabled(pid,false));
		executeUnit(db.playerLogin("bob","test",&loginID,&result));
		assert(result == 1); //success (login count = 2)

		//SUSPENDED
		executeUnit(db.playerSuspendAccount(pid,5));
		executeUnit(db.getPlayer(pid,&player));
		executeUnit(db.playerLogin("bob","test",&loginID,&result));
		assert(result == -2); //suspended
		executeUnit(db.playerUnsuspendAccount(pid));
		executeUnit(db.playerLogin("bob","test",&loginID,&result));
		assert(result == 1); //success (login count = 3)
		executeUnit(db.getPlayer(pid,&player));
		assert(player.loginCount == 3);
		assert(player.loginAttemptCount == 0);

		//END PLAYER LOGIN


		//TEST KARMA
		executeUnit(db.getPlayer(pid,&player));
		int karma = player.karmaPoints;
		executeUnit(db.playerAddKarma(pid,55));
		executeUnit(db.getPlayer(pid,&player));
		assert(player.karmaPoints == karma + 55);
		executeUnit(db.playerAddKarma(pid,-10000));
		executeUnit(db.getPlayer(pid,&player));
		assert(player.karmaPoints >= 0);


		//AVATAR
		executeUnit(db.playerSetAvatar(pid,9));
		executeUnit(db.getPlayer(pid,&player));
		assert(player.avatar == 9);

		//ADMIN ON
		executeUnit(db.playerSetAdministrator(pid,true));
		executeUnit(db.getPlayer(pid,&player));
		assert(player.isAdministrator);

		//ADMIN OFF
		executeUnit(db.playerSetAdministrator(pid,false));
		executeUnit(db.getPlayer(pid,&player));
		assert(!player.isAdministrator);

		//CHAT DISABLED
		executeUnit(db.playerSetChatDisabled(pid,true));
		executeUnit(db.getPlayer(pid,&player));
		assert(player.chatIsDisabled);

		//CHAT ENABLED
		executeUnit(db.playerSetChatDisabled(pid,false));
		executeUnit(db.getPlayer(pid,&player));
		assert(!player.chatIsDisabled);

		//CHAT SUSPENDED
		executeUnit(db.playerSuspendChat(pid,5));
		executeUnit(db.getPlayer(pid,&player));
		assert(!(player.chatSuspendEndDate == boost::none));

		//CHAT UNSUSPENDED
		executeUnit(db.playerUnsuspendChat(pid));
		executeUnit(db.getPlayer(pid,&player));
		assert(player.chatSuspendEndDate == boost::none);

		//TEST CREATE PLAYER
		int64 p2ID = 0;
		int64 p3ID = 0;
		int64 p4ID = 0;
		int64 p5ID = 0;

		executeUnit(db.playerCreate("Paul","McG","bob","test","big@hotmail.com",3,&p2ID,&result));
		assert(result == -3);//name exists
		executeUnit(db.playerCreate("Paul","McG","BigPaul","test","tom@hotmail.com",3,&p2ID,&result));
		assert(result == -2);//email exists

		executeUnit(db.playerCreate("Paul","McG","BigPaul","test","big@hotmail.com",3,&p2ID,&result));
		assert(result == 1);
		assert(p2ID >= 0);

		executeUnit(db.playerCreate("Mitch","Luther","Mitch","test","mitch@hotmail.com",3,&p3ID,&result));
		assert(result == 1);
		assert(p3ID >= 0);

		executeUnit(db.playerCreate("Fargo","Luther","Fargo","test","fargo@hotmail.com",3,&p4ID,&result));
		assert(result == 1);
		assert(p3ID >= 0);


		executeUnit(db.playerCreate("Marge","Simp","Billy","test","billy@hotmail.com",3,&p5ID,&result));
		assert(result == 1);
		assert(p3ID >= 0);

		//TEST FRIENDS
		executeUnit(db.playerAddFriend(pid,p2ID));
		executeUnit(db.playerAddFriend(pid,p3ID));

		executeUnit(db.getPlayer(pid,&player));
		executeUnit(db.playerGetFriendNames(pid,&player));
		assert(player.isFriend("Mitch"));
		assert(player.friends.size() == 2);
		executeUnit(db.playerRemoveFriend(pid,p2ID));
		executeUnit(db.playerRemoveFriend(pid,p3ID));
		executeUnit(db.playerGetFriendNames(pid,&player));
		assert(!player.isFriend("Mitch"));
		assert(player.friends.size() == 0);
		//END TEST FRIENDS

		//CHANGE PASSWORD
		executeUnit(db.playerChangePassword(pid,"lol","timber",&result));
		assert(result == -1);
		executeUnit(db.playerChangePassword(pid,"test","tested",&result));
		assert(result == 1);
		executeUnit(db.playerLogin("tom@hotmail.com","tested",&loginID,&result));
		assert(result == 1); //success (login count = 4)

		//CHANGE EMAIL
		executeUnit(db.playerChangeEmail(pid,"fargo@hotmail.com",&result));
		assert(result == -1);
		executeUnit(db.playerChangeEmail(pid,"tax@hotmail.com",&result));
		assert(result == 1);

		//CREATE A GAME
		executeUnit(db.gameCreate("cutthroat","normal","hand",5,true,false,true,30,&gid,&result));
		assert(result == 1);
		assert(gid >= 0);

		int64 pgid1 = 0;
		int64 pgid2 = 0;
		int64 pgid3 = 0;
		int64 pgid4 = 0;

		executeUnit(db.gameSetCompleted(gid));

		executeUnit(db.playerGameCreate(gid,1,300,false,3,pid,&pgid1,&result));
		assert(result == 1);
		executeUnit(db.playerGameCreate(gid,2,400,true,2,p2ID,&pgid2,&result));
		assert(result == 1);
		executeUnit(db.playerGameCreate(gid,3,600,false,1,p3ID,&pgid3,&result));
		assert(result == 1);

		executeUnit(db.gameLinkPlayerGames(gid,pgid3,pgid2,pgid1,-1));
		executeUnit(db.playerUpdateCompletedGame(pid,0,1,2,3,4,5,6,7,8,9,10,true));
		executeUnit(db.playerUpdateCompletedGame(p2ID,0,0,0,0,0,1,0,0,0,0,0,true));
		executeUnit(db.playerUpdateCompletedGame(p3ID,0,0,0,0,1,0,0,0,0,0,0,false));

		//END CREATE AGAME

		//ADJUST RATING
		executeUnit(db.playerUpdateRating(pid,1600,1550));
		executeUnit(db.getPlayer(pid,&player));
		assert(player.currentRating == 1550);
		assert(player.lifetimeGamesCompleted == 1);
		assert(player.currentHighestRating == 1550);
		assert(player.lifetimeHighestRating == 1600);
		executeUnit(db.playerResetRating(pid));
		executeUnit(db.getPlayer(pid,&player));
		assert(player.lastRatingResetDate != boost::none);
		assert(player.currentRating == 1500);
		assert(player.lifetimeHighestRating == 1600);
		assert(player.ratingResetCount == 1);

		//
		executeUnit(db.playerSubstitutedGame(pid));
		executeUnit(db.getPlayer(pid,&player));
		assert(player.currentGamesSubstituted == 1);
		assert(player.lifetimeGamesSubstituted == 1);

		//
		executeUnit(db.playerAbandonedGame(pid));
		executeUnit(db.getPlayer(pid,&player));
		assert(player.currentGamesAbandoned == 1);
		assert(player.lifetimeGamesAbandoned == 1);

		executeUnit((db.eventAddEntry(-1,DBEVT_NETWORK_ERROR)));

	}

}
