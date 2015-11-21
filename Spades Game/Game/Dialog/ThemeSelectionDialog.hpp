#ifndef CGE_THEME_SELECTION_DIALOG_HPP
#define CGE_THEME_SELECTION_DIALOG_HPP
#include "Game/UI/GeneralDialogBox.hpp"
#include "Game/Element/ThemeManager.hpp"
#include "Game/Handler/SceneEventProvider.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
#include <map>
#include <Agui/Backends/Allegro5/Allegro5Image.hpp>
namespace cge
{
	class ThemeSelectionDialog   : public GeneralDialogBox,
		public SceneEventProvider,
		public LobbyEventProvider,
		public MessageBoxResultReceiver
	{
		std::map<agui::Widget*,int> m_ids;
		int m_selectedID;
		ThemeManager* m_theme;
	public:
		ThemeSelectionDialog(GuiFactory* factory, ThemeManager* cache);
		virtual void dialogWillAppear();
		virtual void frameActionReceived(GeneralFrame* frame,const std::string& action);
		virtual void messageBoxResult(MessageBox* msgbox,MessageBox::MessageResult result);
		virtual bool wantCloseButton() const;
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual bool wantOkButton() const;
		~ThemeSelectionDialog(void);
	};
}
#endif
