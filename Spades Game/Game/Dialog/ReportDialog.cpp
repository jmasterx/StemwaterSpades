#include "Game/Dialog/ReportDialog.hpp"
#include "Game/Utility/DataValidator.hpp"

namespace cge
{
	ReportDialog::~ReportDialog(void)
	{
	}

	ReportDialog::ReportDialog( GuiFactory* factory )
		: GeneralDialogBox(factory,factory->getLanguageManager(),factory->getGuiColorManager(),
		factory->getFontManager())
	{
		DataValidator dv;
		setTitle("report.problem");
		setOkText("submit");
		Label* l = addLabel("other.username");
		l->setSingleLine(false);
		m_txtPlayer = createTextField("");
		m_txtPlayer->setMaxLength(dv.getMaxUsernameLength());
		addLabel("describe.problem");
		m_txtComment = createTextBox(true);
		m_txtComment->setCurrentColor(m_txtComment->getFontColor());
		m_txtComment->setWordWrap(true);
		m_txtComment->setMaxLength(700);

		m_txtPlayer->setFieldName(getFactory()->getLanguageManager()->getElement("player.name"));
		m_txtComment->setFieldName(getFactory()->getLanguageManager()->getElement("problem.description"));
	}

	void ReportDialog::frameActionReceived( GeneralFrame* frame,const std::string& action )
	{
		if(action == "ok")
		{
			if(m_txtComment->getTextLength() > 2 && m_txtComment->getTextLength() < 1000)
			{
				DISPATCH_LOBBY_EVENT
					(*it)->sendFeedback(m_txtPlayer->getText(),m_txtComment->getText(),aquireData());
				DISPATCH_SCENE_EVENT
					(*it)->showMessageBox(
					getFactory()->getLanguageManager()->getElement("feedback.thanks.title"),
					getFactory()->getLanguageManager()->getElement("feedback.thanks.text"),MessageBox::MT_OK,NULL,99);
				hideGeneralFrame();
			}
			else
			{
				DISPATCH_SCENE_EVENT
					(*it)->showMessageBox(
					getFactory()->getLanguageManager()->getElement("description.required.title"),
					getFactory()->getLanguageManager()->getElement("description.required.text"),MessageBox::MT_OK,NULL,99);
			}
		}
		else
		{
			hideGeneralFrame();
		}
	}

	void ReportDialog::dialogWillAppear()
	{
		scrollToTop();
		m_txtComment->setText("");
		m_txtPlayer->setText("");
	}

	agui::Widget* ReportDialog::focusWidget()
	{
		return m_txtComment;
	}

	std::string ReportDialog::aquireData()
	{
#ifdef _WIN32
		return "OS = Windows";
#else
		return "Unknown OS";
#endif
	}

}
