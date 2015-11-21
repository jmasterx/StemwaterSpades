#ifndef CGE_AVATAR_SELECTION_DIALOG_HPP
#define CGE_AVATAR_SELECTION_DIALOG_HPP
#include "Game/UI/GeneralDialogBox.hpp"
#include "Game/Avatar/AvatarCache.hpp"
#include "Game/Handler/SceneEventProvider.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
#include <map>
#include <Agui/Backends/Allegro5/Allegro5Image.hpp>
namespace cge
{
	class AvatarSelectionDialog  : public GeneralDialogBox,
		public SceneEventProvider,
		public LobbyEventProvider,
		public MessageBoxResultReceiver
	{
		std::map<agui::Widget*,int> m_ids;
		int m_selectedID;
		bool m_force;
	public:
		AvatarSelectionDialog(GuiFactory* factory, AvatarCache* cache);
		virtual void dialogWillAppear();
		virtual void frameActionReceived(GeneralFrame* frame,const std::string& action);
		virtual void messageBoxResult(MessageBox* msgbox,MessageBox::MessageResult result);
		virtual bool wantCloseButton() const;
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void setForce(bool force);
		virtual bool wantOkButton() const;
		virtual float getWidthScalar() const;
		virtual float getHeightScalar() const;
		virtual ~AvatarSelectionDialog(void);
	};
}
#endif
