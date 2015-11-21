#include "Game/Dialog/TermsDialog.hpp"
namespace cge
{
	TermsDialog::~TermsDialog(void)
	{
	}

	TermsDialog::TermsDialog( GuiFactory* factory )
		: GeneralDialogBox(factory,factory->getLanguageManager(),factory->getGuiColorManager(),factory->getFontManager())
	{
		m_txtTerms = createFullTextBox();
		if(Platf::isRetina())
		{
			m_txtTerms->setFont(getFactory()->getFontManager()->getFont(2.0f));
		}
		setTitle("terms.title");
		m_txtTerms->setWordWrap(true);
		m_txtTerms->setAsLabel(true);
		setOkText("accept");
		setCancelText("accept.not");
	}

	void TermsDialog::setData( const std::string& terms )
	{
		m_txtTerms->setText(terms);
		m_txtTerms->positionCaret(0,0);
		m_txtTerms->scrollToCaret();
	}

	void TermsDialog::frameActionReceived( GeneralFrame* frame,const std::string& action )
	{
		if(action == "ok")
		{
			DISPATCH_LOBBY_EVENT
				(*it)->playerAcceptedTerms(true);
		}
		else
		{
			DISPATCH_LOBBY_EVENT
				(*it)->playerAcceptedTerms(false);

			DISPATCH_LOBBY_EVENT
				(*it)->logout(true);

		}

		hideGeneralFrame();
	}

}
