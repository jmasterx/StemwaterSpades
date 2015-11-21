#ifndef NEW_ACCOUNT_DIALOG_HPP
#define NEW_ACCOUNT_DIALOG_HPP
#include "Game/UI/GeneralDialogBox.hpp"
#include "Game/Handler/SceneEventProvider.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
namespace cge
{
	class NewAccountDialog : public GeneralDialogBox,public SceneEventProvider,
		public LobbyEventProvider,
		public MessageBoxResultReceiver
	{
		TextField* m_txtFirstName;
		TextField* m_txtLastName;
		TextField* m_txtUsername;
		TextField* m_txtEmail;
		//TextField* m_txtConfirmEmail;
		TextField* m_txtPassword;
		TextField* m_txtConfirmPassword;
		//TextField* m_txtPassphrase;
		//DropDown* m_gender;
		//std::vector<DropDown*> m_birthday;
		CheckBox* m_cbAge;
		bool validateData();
	public:
		NewAccountDialog(GuiFactory* factory);
		virtual void dialogWillAppear();
		virtual void frameActionReceived(GeneralFrame* frame,const std::string& action);
	    virtual void messageBoxResult(MessageBox* msgbox,MessageBox::MessageResult result);
		virtual void createAccountResponse(bool success, bool usernameTaken,bool badUsername, bool badPassword,bool badEmail, bool badGender, bool badBirthday, bool tooYoung,
			bool emailNeedsVerification);
		virtual agui::Widget* focusWidget();

		virtual ~NewAccountDialog(void);
	};
}
#endif
