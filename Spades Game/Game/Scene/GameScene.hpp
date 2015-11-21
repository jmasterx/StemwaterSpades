#ifndef CGE_GAME_SCENE_HPP
#define CGE_GAME_SCENE_HPP
#include "Game/Engine/Scene.hpp"
#include "Game/Element/Background.hpp"
#include "Game/Element/Table.hpp"
#include "Game/Element/Chair.hpp"
#include "Game/CardView/CardView.hpp"
#include "Game/Handler/LightMeterHandler.hpp"
#include "Game/Handler/DataWidgetsHandler.hpp"
#include "Game/SpadesGame/LocalSpadesGame.hpp"
#include "Game/Handler/PassCardsHandler.hpp"
#include "Game/Client/HumanSpadesClient.hpp"
#include "Game/UI/ChatWidget.hpp"
#include "Game/UI/IOButton.hpp"
#include "Game/Engine/GraphicsContext.hpp"
#include "Game/Handler/GameEventProvider.hpp"
#include "Game/Net/NetMediator.hpp"
#include "Game/Handler/LobbyContextHandler.hpp"
#include "Game/Shader/Shader.hpp"
#include "Game/Avatar/AvatarComposite.hpp"
#include "Game/Avatar/ArcText.hpp"
#include "Game/Avatar/AvatarManager.hpp"
#include "Game/Dialog/ViewProfileDialog.hpp"
#include "Game/Dialog/GiftSelectionDialog.hpp"
#include "Game/Element/GiftRenderer.hpp"
#include "Game/Dialog/PaymentDialog.hpp"

namespace cge
{
	class GameScene : public Scene, public GameEventProvider, public ClientEventListener,
		MessageBoxResultReceiver
	{
		std::vector<DynamicElement*> m_dynamicElements;
		std::vector<GameEventProvider*> m_providers;
		std::vector<GameEventListener*> m_listeners;
		void addDynamicElement(DynamicElement* element);
		void addClientListener(ClientEventListener* listener);
		void _addGameListener(GameEventListener* listener);
		void _addGameProvider(GameEventProvider* listener);
		void linkListeners();
		void unlinkListeners();

		LocalSpadesGame m_localGame;
		HumanSpadesClient m_client;
		LocalMessageMediator* m_mediator; //Net or Local
		NetMediator m_netMediator;

		GraphicsContext g;
		Shader m_shader;

		ArcText arc;

		ProportionsManager m_proportions;
		//dynamic elements
		Background m_background;
		Table m_table;
		Table m_tableShadow;
		Chair m_leftChair;
		Chair m_rightChair;
		Chair m_topChair;
		Chair m_bottomChair;
		CardView m_cardView; //Provide to client
		LightMeterHandler* m_lightMeterHandler; //Listener

		//gui elements
		DataWidgetsHandler* m_dataWidgets; //Dynamic, Listener
		BidWidget* m_bidWidget; //DO NOT FREE, Dynamic, Listener, Provider 
		PassCardsHandler* m_passHandler; //Dynamic, Listener, Provider
		ChatWidget* m_chatWidget; //Dynamic
		GiftRenderer* m_giftRenderer;
		IOButton* m_ioButton;//free it
		GameInformation* m_infoBox; //DO NOT free it
		GameOverScreen* m_gameOver; //DO NOT free it

		LobbyContextHandler* m_contextHandler; //free it

		PaymentDialog* m_paymentDialog;

		bool m_needsGameOver;

		bool m_needGoToLobby;
		AvatarManager* m_avatarMan;
		ViewProfileDialog* m_viewProfileDialog;
		GiftSelectionDialog* m_giftSelectionDialog;

		bool m_needsToRefreshCoinAmount;
		float m_coinAmountRefreshTime;
		float m_coinAmountRefreshInterval;
        
        bool m_waitedForSceneChange;
        bool m_needsSceneChange;
		bool m_canLogic;
		int m_timeout;

		Button* m_leaveButton;
		void _initElements();
		void _linkElements();
		void _startGame();

		std::string toString(int i) const;
		std::string toString(int64 i) const;
		void createLeaveButton();
	public:
		GameScene(void);
		virtual void sceneBegin();
		virtual void render();
		virtual void resizeEvent(int width, int height);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void processEvent(ALLEGRO_EVENT* evt, bool& handled);
		virtual void restoreClientState(const GameClientState& state);
		virtual void playerAbandonedGame(const std::string& username);
		virtual void playerJoinedGame(const std::string& player,int seatNo);
		virtual void receiveGift(int fromPlayer, int toPlayer, int giftID);
		virtual void playerLeftGame(const std::string& player, int seatNo);
		virtual bool getGamePlayerColorImpl(const std::string& playerName, agui::Color* c);
		virtual bool getLobbyPlayerColorImpl(const std::string& playerName, agui::Color* c);
		virtual void determineGamePlayerColor(TableMessage& msg);		
		TableMessageColorEnum aquirePlayerColor(const std::string& playerName, agui::Color* c);

		virtual void gameIsOver(const std::vector<int>& winningTeamOrder, const std::vector<int>& finalTeamScores, 
			const std::vector<int>& newPlayerRatings, const std::vector<std::string>& players, int64 netChane, int64 newAmount, int timeout);

		virtual void gameOverScreenHidden(bool playAgainResult);

		virtual void readyToPlayAgain();
		virtual void gameBegin();
		virtual void leaveFromTable();
				void setAvatarNames();
		virtual void activePlayerChanged(int newActivePlayer);
		virtual void playerBidChanged(int player, int newBid);
		virtual void roundBegan();
		virtual void roundEnded();
		virtual void logic();
		void doGameover(int timeout);
		virtual void playSound(const std::string& sound);
		virtual void bidStateChanged(const std::vector<SpadesPointEnum> &b, const std::vector<SpadesPointEnum> &l, const std::vector<SpadesPointEnum> &t, const std::vector<SpadesPointEnum> &r, std::vector<int> bids);
		virtual void playerMadeTrick(int player, SpadesPointEnum trickType);
		virtual void playerFailedNil(int player);
		virtual agui::Widget* getIOButton();
		virtual void displayMessageFromServer(const std::string& title, const std::string& message, bool titleLocalized, bool messageLocalized);
		virtual void showProfile(
			int avatar, const std::vector<std::string>& data);
		void setPlayerActiveColor(int pos, TableMessageColorEnum color);
		virtual void showGiftDialog(int playerPos, const std::string& playerName);
		virtual void displayGiftDialog(int playerPos, const std::string& cost);
		virtual void giftChanged(int player, int giftID, int frame);
		virtual void showCoinsDialog(
			const std::vector<std::string>& coinAmounts, const std::vector<std::string>& prices,
			const std::vector<std::string>& salePercents, const std::vector<std::string>& coinPercents, const std::vector<std::string>& coinUrls);
		virtual void messageBoxResult(MessageBox* msgbox,MessageBox::MessageResult result);
		void dispatchGiftMessage(int fromPlayer,int toPlayer, int giftID);
		virtual bool processQuitRequest();
		virtual void spadesBroken(int playerWhoBroke);
		virtual void contractCompleted(int playerCompleter, bool partnership, bool watching);
		virtual void windowGotFocus();
        virtual void accelerometerChanged(float x, float y, float z);
        virtual void sceneReady();
		virtual void badAsyncG();
		virtual void showPlayerStrike(int player, int numStrikes, int time, bool beginAction);
		virtual void sceneEnd();
		virtual void disconnectionReturned(bool leave);
		void determineLobbyPlayerColor( TableMessage& msg );
		cge::TableMessageColorEnum aquirePlayerColorLobby( const std::string& playerName, agui::Color* c );

		virtual ~GameScene(void);
	};
}

#endif
