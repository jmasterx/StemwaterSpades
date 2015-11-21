#include "Game/Dialog/AccountSettingsDialog.hpp"
#define MB_RESET_RATING 2
namespace cge
{
	AccountSettingsDialog::~AccountSettingsDialog(void)
	{
	}

	AccountSettingsDialog::AccountSettingsDialog( GuiFactory* factory, ClientShared* shared )
		: GeneralDialogBox(factory,factory->getLanguageManager(),factory->getGuiColorManager(),factory->getFontManager()),
    m_shared(shared)
	{
		int spacing = 15;
		setTitle("self.edit.profile");
		setOkText("close");

		addLabel("account.settings.full.name");
		m_txtFirstName = createTextField("first.name");
		m_txtLastName = createTextField("last.name");
		addWhitespace(spacing / 2);
		m_btnSaveName = createActionButton("save.changes");
		addWhitespace(spacing);

		addLabel("account.settings.change.password");
		m_txtOldPassword = createTextField("account.settings.current.password");
		m_txtNewPassword = createTextField("account.settings.new.password");
		m_txtConfirmPassword = createTextField("account.settings.confirm.password");
		addWhitespace(spacing / 2);
		m_btnSavePassword = createActionButton("save.changes");
		addWhitespace(spacing);

		addLabel("account.settings.change.email");
		m_txtNewEmail = createTextField("account.settings.new.email",DIALOG_TEXT_LONG);
		m_txtConfirmNewEmail = createTextField("account.settings.confirm.email",DIALOG_TEXT_LONG);
		addWhitespace(spacing / 2);
		m_btnSaveEmail = createActionButton("save.changes");
		addWhitespace(spacing);

		addLabel("account.settings.reset.stats");
		m_btnResetRating = createActionButton("account.settings.reset.stats");
		addWhitespace(spacing);

		addLabel("account.settings.show.email");
		m_cbShowEmail = createCheckBox("account.settings.show.email");
		addWhitespace(spacing / 2);
		m_btnSaveShowEmail = createActionButton("save.changes");
		addWhitespace(spacing / 2);

		m_btnResetRating->addActionListener(this);
		m_btnSaveName->addActionListener(this);
		m_btnSavePassword->addActionListener(this);
		m_btnSaveEmail->addActionListener(this);
		m_btnSaveShowEmail->addActionListener(this);

		m_txtFirstName->setMaxLength(25);
		m_txtLastName->setMaxLength(25);

		m_txtOldPassword->setIsPassword(true);
		m_txtOldPassword->setMaxLength(20);
		m_txtNewPassword->setIsPassword(true);
		m_txtNewPassword->setMaxLength(20);
		m_txtConfirmPassword->setIsPassword(true);
		m_txtConfirmPassword->setMaxLength(20);

		m_txtNewEmail->setMaxLength(35);
		m_txtConfirmNewEmail->setMaxLength(35);
	}

	void AccountSettingsDialog::actionPerformed( const agui::ActionEvent &evt )
	{
		GeneralDialogBox::actionPerformed(evt);

		if(evt.getSource() == m_btnSaveName)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->requestChangeFullName(m_txtFirstName->getText(),
				m_txtLastName->getText());
		}
		else if(evt.getSource() == m_btnSavePassword)
		{
			if(!m_dataValidator.isPasswordValid(m_txtOldPassword->getPassword())
				|| !m_dataValidator.isPasswordValid(m_txtNewPassword->getPassword())
				|| !m_dataValidator.isPasswordValid(m_txtConfirmPassword->getPassword()))
			{
				DISPATCH_SCENE_EVENT
					(*it)->showMessageBox(
					getFactory()->getLanguageManager()->getElement("newaccount.validate"),
					getFactory()->getLanguageManager()->getElement("account.invalid.password"),MessageBox::MT_OK,this,1);
			}
			else if(m_txtNewPassword->getPassword() != m_txtConfirmPassword->getPassword())
			{
				DISPATCH_SCENE_EVENT
					(*it)->showMessageBox(
					getFactory()->getLanguageManager()->getElement("newaccount.validate"),
					getFactory()->getLanguageManager()->getElement("account.password.mismatch"),MessageBox::MT_OK,this,1);
			}
			else
			{
				DISPATCH_LOBBY_EVENT
					(*it)->requestChangePassword(m_txtOldPassword->getPassword(),m_txtNewPassword->getPassword());
			}
		}
		else if(evt.getSource() == m_btnResetRating)
		{
			DISPATCH_SCENE_EVENT
				(*it)->showMessageBox(
				getFactory()->getLanguageManager()->getElement("reset.rating.title"),
				getFactory()->getLanguageManager()->getElement("reset.rating.text"),MessageBox::MT_YES_NO,this,MB_RESET_RATING);
		}
		else if(evt.getSource() == m_btnSaveEmail)
		{
			if(m_txtNewEmail->getText() != m_txtConfirmNewEmail->getText())
			{
				DISPATCH_SCENE_EVENT
					(*it)->showMessageBox(
					getFactory()->getLanguageManager()->getElement("newaccount.validate"),
					getFactory()->getLanguageManager()->getElement("email.mismatch.validate"),MessageBox::MT_OK,this,1);
			}
			else if(!m_dataValidator.isEmailValid(m_txtNewEmail->getText()))
			{
				DISPATCH_SCENE_EVENT
					(*it)->showMessageBox(
					getFactory()->getLanguageManager()->getElement("newaccount.validate"),
					getFactory()->getLanguageManager()->getElement("email.invalid.validate"),MessageBox::MT_OK,this,1);
			}
			else
			{
				DISPATCH_LOBBY_EVENT
					(*it)->requestChangeEmail(m_txtNewEmail->getText());
			}
		}
		else if(evt.getSource() == m_btnSaveShowEmail)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->requestChangeShowEmail(m_cbShowEmail->checked());
		}

	}

	void AccountSettingsDialog::frameActionReceived( GeneralFrame* frame,const std::string& action )
	{
		hideGeneralFrame();
	}

	void AccountSettingsDialog::messageBoxResult( MessageBox* msgbox,MessageBox::MessageResult result )
	{
		if(msgbox->getTag() == MB_RESET_RATING && result == MessageBox::MR_YES)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->requestResetRating();
		}
	}

	void AccountSettingsDialog::setData( const std::string& firstName, const std::string& lastName, bool showEmail )
	{
		m_txtFirstName->setText(firstName);
		m_txtLastName->setText(lastName);
		m_cbShowEmail->setChecked(showEmail);
		m_txtNewEmail->setText("");
		m_txtConfirmNewEmail->setText("");
		m_txtOldPassword->setText("");
		m_txtNewPassword->setText("");
		m_txtConfirmPassword->setText("");
	}

	bool AccountSettingsDialog::wantCloseButton() const
	{
		return false;
	}

	void AccountSettingsDialog::accountSettingsChanged( int dialog )
	{
		if(dialog == 0) //change in name
		{
			//no need to do anything
		}
		else if(dialog == 1) //change in password
		{
            m_shared->setCachedPassword(m_txtNewPassword->getPassword());
            m_shared->getSettingsManager()->setStringSetting("auth.hash",m_txtNewPassword->getPassword());
			m_txtOldPassword->setText("");
			m_txtNewPassword->setText("");
			m_txtConfirmPassword->setText("");
		}
		else if(dialog == 2) //change in email
		{
			m_txtNewEmail->setText("");
			m_txtConfirmNewEmail->setText("");
		}
	}

}
