#ifndef CGE_SPADES_GAME_MESSAGE_ENUM_HPP
#define CGE_SPADES_GAME_MESSAGE_ENUM_HPP
namespace cge
{
	enum SpadesCSMessageEnum
	{	
		SET_GAME_INFO_C = 0,					
		//game type, game end condition, game end value, pass value, moon shot, nil value, deck cut value
		SEND_GAME_INFO_D_S = 1,				//see above + int host (sent when a player joins
	    PLAYER_JOINED_C = 2,				//nothing
		PLAYER_DISCONNECTED_C = 3,			//nothing
		REPLY_INIT_GAME_C = 4, 				//nothing, From Host
		REQUEST_GAME_INIT_S = 5,			//To host
		REQUEST_DECK_CUT_POS_S = 6,			//nothing
		SEND_DECK_CUT_POS_C = 7,			//int pos
		DISPATCH_CARDS_A_D_S = 8,			//int dealer, int p2 count, int p3 count, int p4 count, vector of Card, 
		FINISH_DISPATCH_CARDS_C = 9,        //nothing
		REQUEST_CARD_SWAP_S = 10,			//int number of cards
		SEND_SWAPPED_CARDS_C = 11,			//Vector of card number
		PERFORM_SWAP_S = 12,					//state 0 = BOTH 1 = TOP-BOTTOM 2 = LEFT-RIGHT (if 2, no more args),Vector of card number incoming, Vector of card number outgoing
		FINISHED_CARD_SWAP_C = 13,			//nothing
		FILTER_CARDS_S = 14,					//vector of disabled Cards
		PLAYER_MADE_TRICK_A_D_S = 15,		//int sendCardsTo, int givePointTo, point enum
		FINISHED_PLAYER_MADE_TRICK_C = 16,	//nothing
		REQUEST_PLAYER_TRICK_COUNT_S = 17,	//nothing
		SEND_PLAYER_TRICK_COUNT_C = 18,		//int numTricks
		SEND_PLAYER_TRICK_COUNT_D_S = 19,	//int player, int trickCount
		SEND_ROUND_SCORES_D_S = 20,			//int teamXScore, ... int teamXBags ...
		REQUEST_SELECTED_CARD_S = 21,		//nothing
		SEND_SELECTED_CARD_C = 22,			//Card that player chose (index)
		SEND_ACTIVE_PLAYER_S = 23,			//int active player
		PERFORM_CARD_ON_TABLE_A_D_S = 24,	//Card card to throw, int playerNum 
		FINISH_PERFORM_CARD_ON_TABLE_C = 25, //nothing
		ROUND_BEGIN_S = 26,					//nothing
		GAME_OVER_S = 27					//win team, bool moonshot win
	};

	enum SpadesGameTypeEnum
	{
		INDIVIDUAL = 0,
		INDIVIDUAL_MIRROR = 1,
		PARTNERSHIP = 2,
		PARTNERSHIP_MIRROR = 3,
		PARTNERSHIP_SUICIDE = 4,
		CUTTHROAT = 5,
		CUTTHROAT_MIRROR = 6
	};

	enum SpadesGameEndConditionEnum
	{
		SCORE_REACHED = 0,
		TIME_OUT = 1,
		HAND_LIMIT_REACHED = 2
	};

	enum SpadesGameMoonShotPolicyEnum
	{
		DISALLOW_MOON_SHOT  = 0,
		ALLOW_MOON_SHOT = 1
	};

	enum SpadesGamePassEnum
	{
		NO_PASS = 0,
		PASS_TWO = 1
	};

	enum SpadesGameDeckCutPolicyEnum
	{
		NO_DECK_CUT = 0,
		WANT_DECK_CUT = 1
	};

	enum SpadesPointEnum
	{
		REGULAR_POINT = 0,
		POINT_FROM_PARTNER = 1,
		OVERTRICK_POINT = 2,
		NOT_A_POINT = 3,
		CONTRACT_POINT = 4
	};
}

#endif