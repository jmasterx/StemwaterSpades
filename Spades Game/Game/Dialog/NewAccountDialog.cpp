#include "Game/Dialog/NewAccountDialog.hpp"
#include "Game/UI/GeneralDialogBox.hpp"
#include "Game/Utility/DataValidator.hpp"
namespace cge
{


	NewAccountDialog::~NewAccountDialog(void)
	{
	}

	NewAccountDialog::NewAccountDialog( GuiFactory* factory )
		: GeneralDialogBox(factory,factory->getLanguageManager(),factory->getGuiColorManager(),factory->getFontManager())
	{
		setTitle("account.create");
		setOkText("account.create");
		addLabel("fields.required");
		m_txtUsername = createTextField("username");
		m_txtPassword = createTextField("password");
		m_txtConfirmPassword = createTextField("confirm.password");
		m_txtEmail = createTextField("email",DIALOG_TEXT_LONG);
	//	m_txtConfirmEmail = createTextField("email.confirm",DIALOG_TEXT_LONG);
		//std::vector<std::string> items;
		//items.push_back("gender.male");
		//items.push_back("gender.female");
		//m_gender = createDropdown("gender.gender","gender.gender",items);
		//m_birthday = createDate("birthday");
		//m_txtPassphrase = createTextField("security.phrase");
		m_cbAge = createCheckBox("");
		m_cbAge->setText(getFactory()->getLanguageManager()->getElementWithArg("over.age","13"));

		addLabel("fields.optional");
		m_txtFirstName = createTextField("first.name");
		m_txtLastName = createTextField("last.name");

		DataValidator dv;

		m_txtFirstName->setMaxLength(25);
		m_txtLastName->setMaxLength(25);
		m_txtUsername->setMaxLength(dv.getMaxUsernameLength());
		m_txtPassword->setIsPassword(true);
		m_txtPassword->setMaxLength(20);
		m_txtConfirmPassword->setIsPassword(true);
		m_txtConfirmPassword->setMaxLength(20);
		m_txtEmail->setMaxLength(35);
		//m_txtConfirmEmail->setMaxLength(35);

	}

	void NewAccountDialog::dialogWillAppear()
	{
		m_txtFirstName->setText("");
		m_txtLastName->setText("");
		m_txtUsername->setText("");
		m_txtPassword->setText("");
		m_txtConfirmPassword->setText("");
		m_txtEmail->setText("");
		m_cbAge->setChecked(false);
		//m_txtConfirmEmail->setText("");
		//m_txtPassphrase->setText("");
		//m_gender->setSelectedIndex(-1);
		//for(int i = 0; i < m_birthday.size(); i++)
		//{
	//		m_birthday[i]->setSelectedIndex(-1);
	//	}
	}

	void NewAccountDialog::frameActionReceived( GeneralFrame* frame,const std::string& action )
	{
		if(action == "cancel")
		{
			hideGeneralFrame();
		}
		else
		{
			if(validateData())
			{
				/*
				std::string gender;
				if(m_gender->getSelectedIndex() == 0)
					gender = "m";
				else
				gender = "f";

				std::stringstream ss;
				int year = 0;
				int month = m_birthday[0]->getSelectedIndex();
				month++;
				ss.clear();
				ss.str("");
				ss << m_birthday[1]->getItemAt(m_birthday[1]->getSelectedIndex());
				int day = 0;
				ss >> day;
				ss.clear();
				ss.str("");
				ss << m_birthday[2]->getItemAt(m_birthday[2]->getSelectedIndex());
				ss >> year;
*/
				DISPATCH_LOBBY_EVENT
					(*it)->createAccount(m_txtUsername->getText(),
					m_txtPassword->getPassword(),
					m_txtEmail->getText(),m_txtFirstName->getText(),m_txtLastName->getText());
			}
		}
		
	}

	void NewAccountDialog::messageBoxResult( MessageBox* msgbox,MessageBox::MessageResult result )
	{

	}

	bool NewAccountDialog::validateData()
	{
		std::vector<std::string> errors;
		DataValidator dv;
		std::stringstream ss;

		/*
		if(m_txtFirstName->getTextLength() < 3)
		{
			errors.push_back("firstname.validate");
		}

		if(m_txtLastName->getTextLength() < 3)
		{
			errors.push_back("lastname.validate");
		}
		*/

		if(m_txtUsername->getTextLength() < 3)
		{
			errors.push_back("username.validate");
		}

		if(!dv.usernameIsValid(m_txtUsername->getText()))
		{
			errors.push_back("username.invalid.validate");
		}

		if(!dv.isPasswordValid(m_txtPassword->getPassword()))
		{
			errors.push_back("password.validate");
		}
		else if(m_txtPassword->getPassword() != m_txtConfirmPassword->getPassword())
		{
			errors.push_back("account.password.mismatch");
		}

		if(m_txtEmail->getTextLength() == 0)
		{
			errors.push_back("email.validate");
		}
		else if(!dv.isEmailValid(m_txtEmail->getText()))
		{
			errors.push_back("email.invalid.validate");
		}

		if(!m_cbAge->checked()) 
		{
			errors.push_back("confirm.age");
		}

/*
		if(m_txtEmail->getText() != m_txtConfirmEmail->getText())
		{
			errors.push_back("email.mismatch.validate");
		}
		*/
/*
		if(m_gender->getSelectedIndex() < 0)
		{
			errors.push_back("gender.validate");
		}

		if(m_txtPassphrase->getTextLength() == 0)
		{
			errors.push_back("security.phrase.validate");
		}
		

		bool anythingEmpty = false;

		for(int i = 0; i < m_birthday.size(); i++)
		{
			if(m_birthday[i]->getSelectedIndex() < 0)
				anythingEmpty = true;
		}
		*/
/*
		if(anythingEmpty)
		{
			errors.push_back("birthday.validate");
		}
		else
		{
			int month = m_birthday[0]->getSelectedIndex();
			month++;
			ss.clear();
			ss.str("");
			ss << m_birthday[1]->getItemAt(m_birthday[1]->getSelectedIndex());
			int day = 0;
			ss >> day;
			if(!dv.dateIsValid(2000,month,day))
			{
				errors.push_back("birthday.invalid.validate");
			}
		}
		*/

		std::string errorText;
		std::string bullet;
		bullet += 0xe2;
		bullet += 0x80;
		bullet += 0xa2;

		for(int i = 0; i < errors.size(); i++)
		{
			errorText += bullet;
			errorText += " ";
			errorText += getFactory()->getLanguageManager()->getElement(errors[i]);
			if(i + 1 < errors.size())
			errorText += "\n\n";
		}

		if(errorText.length() > 0)
		{
			DISPATCH_SCENE_EVENT
				(*it)->showMessageBox(
				getFactory()->getLanguageManager()->getElement("newaccount.validate"),errorText,MessageBox::MT_OK,this,1);
		}

		return errorText.length() == 0;
	}

	void NewAccountDialog::createAccountResponse( 
		bool success, bool usernameTaken,bool badUsername,bool badPassword,bool badEmail, 
		bool badGender, bool badBirthday, bool tooYoung, bool emailNeedsVerification )
	{
		std::vector<std::string> errors;

		if(usernameTaken)
		{
			errors.push_back("account.username.taken");
		}
		if(badUsername)
		{
			errors.push_back("username.invalid.validate");
		}
		if(badPassword)
		{
			errors.push_back("account.invalid.password");
		}
		if(badEmail)
		{
			errors.push_back("account.invalid.email");
		}
		if(badGender)
		{
			errors.push_back("account.invalid.gender");
		}
		if(badBirthday)
		{
			errors.push_back("account.invalid.birthday");
		}
		if(tooYoung)
		{
			errors.push_back("account.invalid.young");
		}
		if(errors.empty() && !success)
		{
			errors.push_back("database.access.error");
		}

		std::string errorText;
		std::string bullet;
		bullet += 0xe2;
		bullet += 0x80;
		bullet += 0xa2;

		for(int i = 0; i < errors.size(); i++)
		{
			errorText += bullet;
			errorText += " ";
			errorText += getFactory()->getLanguageManager()->getElement(errors[i]);
			if(i + 1 < errors.size())
				errorText += "\n\n";
		}


		if(success)
		DISPATCH_SCENE_EVENT
		(*it)->showMessageBox("Success","Account has been created!",MessageBox::MT_OK,this,2);
		else
			DISPATCH_SCENE_EVENT
			(*it)->showMessageBox(getFactory()->getLanguageManager()->getElement("account.problem.title"),
			errorText,MessageBox::MT_OK,this,2);

		if(success)
		{
			DISPATCH_SCENE_EVENT
				(*it)->setBoolSetting("account.created",true);
			hideGeneralFrame();
		}
	}

	agui::Widget* NewAccountDialog::focusWidget()
	{
		return m_txtUsername;
	}

}
