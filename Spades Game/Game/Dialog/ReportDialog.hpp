#ifndef CGE_REPORT_DIALOG_HPP
#define CGE_REPORT_DIALOG_HPP
#include "Game/UI/GeneralDialogBox.hpp"
#include "Game/Handler/SceneEventProvider.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
namespace cge
{
	class ReportDialog :
		public GeneralDialogBox,public SceneEventProvider,
		public LobbyEventProvider
	{
		TextBox* m_txtComment;
		TextField* m_txtPlayer;
	public:
		ReportDialog(GuiFactory* factory);
		virtual void dialogWillAppear();
		virtual void frameActionReceived(GeneralFrame* frame,const std::string& action);
		virtual agui::Widget* focusWidget();
		std::string aquireData();
		virtual ~ReportDialog(void);
	};
}
#endif
