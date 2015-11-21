#ifndef CGE_TERMS_DIALOG_HPP
#define CGE_TERMS_DIALOG_HPP
#include "Game/UI/GeneralDialogBox.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
namespace cge
{
	class TermsDialog : public GeneralDialogBox, public LobbyEventProvider
	{
		TextBox* m_txtTerms;
	public:
		TermsDialog(GuiFactory* factory);
		void setData(const std::string& terms);
		virtual void frameActionReceived(GeneralFrame* frame,const std::string& action);
		virtual ~TermsDialog(void);
	};
}

#endif
