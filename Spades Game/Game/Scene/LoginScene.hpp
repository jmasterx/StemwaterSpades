#ifndef CGE_LOGIN_SCENE_HPP
#define CGE_LOGIN_SCENE_HPP
#include "Game/Engine/Scene.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
#include "Game/UI/IOButton.hpp"
#include "Game/Avatar/ArcText.hpp"
#include "Game/Dialog/NewAccountDialog.hpp"
#include "Game/Particle/RainParticleSystem.hpp"
#include <Agui/SelectionListener.hpp>
#include "Game/Element/Background.hpp"
namespace cge
{
	class LoginScene : public Scene, public LobbyEventProvider,
		public MessageBoxResultReceiver,public agui::SelectionListener
	{
		TextField* m_username;
		TextField* m_password;
		CheckBox* m_rememberMe;
		DropDown*  m_langDrop;
		Button*  m_login;
		FlowLayout* m_flow;
		Label* m_lblName;
		Label* m_lblPassword;
		Label* m_lblError;
		Label* m_lblCopyright;
		Sprite* m_gear;
		agui::Widget* m_beginSpacerA;
		agui::Widget* m_beginSpacerB;
		agui::Widget* m_loginSpacer;
		agui::Widget* m_loginSpacerA;
		agui::Widget* m_loginSpacerB;
		agui::Widget* m_newUserSpacer;
		agui::Widget* m_forgotSpacer;
		bool m_controlsEnabled;
		IOButton* m_ioButton;//free it
		std::vector<LobbyEventProvider*> m_providers;
		std::vector<LobbyEventListener*> m_listeners;
		Sprite* m_bg;

		ArcText m_stemwater;
		ArcText m_spades;

		Button* m_newUserBtn;
		Button* m_forgotPasswordBtn;

		NewAccountDialog* m_newAccountDialog;

		bool m_showDarkness;
		bool m_needToPlaySound;
		agui::Font* m_headerFont;
		Background m_mainbg;
		ProportionsManager m_proportions;
		float m_gearAngle;
		float m_connectTime;
	public:
		LoginScene(void);
		void addProvider(LobbyEventProvider* provider);
		void addListener(LobbyEventListener* listener);
		void linkListeners();
		void unlinkListeners();
		virtual void sceneBegin();
		virtual void textChanged(agui::Widget* textbox, const std::string & change);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void processEvent(ALLEGRO_EVENT* evt, bool& handled);
		virtual void render();
		virtual void logic();
		virtual void resizeEvent(int width, int height);
		virtual void loginResult(bool success, const std::string& sessionID, const std::string& username);
		void renderBG(GraphicsContext* g);
		void changeErrorLabel();
		virtual void connectionFailed();
		virtual agui::Widget* getIOButton();
		virtual void displayMessageFromServer(const std::string& title, const std::string& message, bool titleLocalized, bool messageLocalized);
		void hideDarkness();
		void showDarkness();
		virtual void messageBoxResult(MessageBox* msgbox,MessageBox::MessageResult result);
		virtual bool processQuitRequest();
		virtual void selectionChanged(agui::Widget *source, const std::string &item, int index, bool selected);
		virtual void receiveLoginServer(const std::string& username, const std::string& name, const std::string& ip, int port);
		void generateLoginRequest(const std::string& username);
		void setControlsEnabled(bool enabled);
		virtual void createAccount(const std::string& username, const std::string& password, const std::string& email, const std::string& firstName, const std::string& lastName);
        virtual void appHasReturned(bool wasSignedIn);
		virtual ~LoginScene(void);
	};
}

#endif

