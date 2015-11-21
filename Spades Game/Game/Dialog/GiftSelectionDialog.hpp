#ifndef CGE_GIFT_SELECTION_DIALOG_HPP
#define CGE_GIFT_SELECTION_DIALOG_HPP
#include "Game/UI/GeneralDialogBox.hpp"
#include "Game/Element/GiftManager.hpp"
#include "Game/Handler/SceneEventProvider.hpp"
#include "Game/Handler/GameEventProvider.hpp"
#include <map>
#include <Agui/Backends/Allegro5/Allegro5Image.hpp>
#include <Agui/SelectionListener.hpp>

namespace cge
{
	class GiftSelectionDialog  : public GeneralDialogBox,
		public SceneEventProvider,
		public GameEventProvider,
		public MessageBoxResultReceiver,
		public agui::SelectionListener
	{
		std::map<agui::Widget*,int> m_ids;
		std::map<agui::Widget*,std::string> m_categoryMap;
		int m_selectedID;
		GiftManager* m_theme;
		int m_receiver;
		std::string m_cost;
		DropDown* m_categories;
		agui::Widget* m_emptyPadding;
		std::vector<agui::Widget*> m_parentWidgets;
		std::vector<std::string> m_rawCategories;
	public:
		GiftSelectionDialog(GuiFactory* factory, GiftManager* cache);
		virtual void dialogWillAppear();
		virtual void frameActionReceived(GeneralFrame* frame,const std::string& action);
		virtual void messageBoxResult(MessageBox* msgbox,MessageBox::MessageResult result);
		virtual bool wantCloseButton() const;
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual bool wantOkButton() const;
		void setReceiver(int rec);
		virtual std::string getSideLabelText() const;
		void setCost(const std::string& cost);
		virtual const std::vector<agui::Widget*>& parentBottomWidgets();
		virtual void selectionChanged(agui::Widget *source, const std::string &item, int index, bool selected);
        virtual float getWidthScalar() const;
        virtual float getHeightScalar() const;
		virtual ~GiftSelectionDialog(void);
	};
}
#endif