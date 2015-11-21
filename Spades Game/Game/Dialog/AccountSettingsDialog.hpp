#ifndef CGE_ACCOUNT_SETTINGS_DIALOG_HPP
#define CGE_ACCOUNT_SETTINGS_DIALOG_HPP
#include "Game/UI/GeneralDialogBox.hpp"
#include "Game/Handler/SceneEventProvider.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
#include "Game/Utility/DataValidator.hpp"
namespace cge
{
	class AccountSettingsDialog :
		public GeneralDialogBox,public SceneEventProvider,
		public LobbyEventProvider,
		public MessageBoxResultReceiver
	{
		DataValidator m_dataValidator;
		TextField* m_txtFirstName;
		TextField* m_txtLastName;
		Button* m_btnSaveName;

		TextField* m_txtOldPassword;
		TextField* m_txtNewPassword;
		TextField* m_txtConfirmPassword;
		Button* m_btnSavePassword;

		TextField* m_txtNewEmail;
		TextField* m_txtConfirmNewEmail;
		Button* m_btnSaveEmail;

		Button* m_btnResetRating;

		CheckBox* m_cbShowEmail;
		Button* m_btnSaveShowEmail;
        ClientShared* m_shared;
	public:
		AccountSettingsDialog(GuiFactory* factory,ClientShared* shared);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void frameActionReceived(GeneralFrame* frame,const std::string& action);
		virtual void messageBoxResult(MessageBox* msgbox,MessageBox::MessageResult result);
		void setData(const std::string& firstName, const std::string& lastName, bool showEmail);
		virtual bool wantCloseButton() const;
		virtual void accountSettingsChanged(int dialog);
		virtual ~AccountSettingsDialog(void);
	};
}
#endif

