#include "Game/Dialog/GiftSelectionDialog.hpp"
#include "Game/Utility/StringUtil.hpp"
#define  MSG_CONFIRM_SELECTION 1
namespace cge
{
	GiftSelectionDialog::~GiftSelectionDialog(void)
	{
		m_categories->removeSelectionListener(this);
		//m_theme->destroyThumbs();
	}

	void GiftSelectionDialog::dialogWillAppear()
	{
		scrollToTop();
		forceResize();
	}

	void GiftSelectionDialog::messageBoxResult( MessageBox* msgbox,MessageBox::MessageResult result )
	{
		if(msgbox->getTag() == MSG_CONFIRM_SELECTION && result == MessageBox::MR_YES)
		{
			DISPATCH_GAME_EVENT
				(*it)->sendGift(m_receiver,m_selectedID);
			hideGeneralFrame();
		}
	}
    
    float GiftSelectionDialog::getWidthScalar() const {
        float a = GeneralDialogBox::getWidthScalar();
        if(Platf::isRetina() || Platf::isMini())
            return a * 1.2f;
        return a;
    }
    float GiftSelectionDialog::getHeightScalar() const {
        float a = GeneralDialogBox::getHeightScalar();
        
        if(Platf::isRetina() || Platf::isMini())
            return a * 1.2f;
        return a;
    }

	void GiftSelectionDialog::frameActionReceived( GeneralFrame* frame,const std::string& action )
	{
		hideGeneralFrame();
	}

	bool GiftSelectionDialog::wantCloseButton() const
	{
		return false;
	}

	GiftSelectionDialog::GiftSelectionDialog( GuiFactory* factory, GiftManager* cache )
		: GeneralDialogBox(factory,factory->getLanguageManager(),factory->getGuiColorManager(),
		factory->getFontManager()),m_selectedID(-1),m_theme(cache),m_receiver(-1)
	{
		//m_theme->loadThumbs();
		FlowLayout* flow = addFlow();
		flow->setHorizontallyCentered(true);
		flow->setAlignLastRow(true);
		flow->setHorizontalSpacing(15);
		flow->setVerticalSpacing(15);
		flow->setMargins(15,1,15,1);

		std::map<std::string,GiftData*> giftMap;

		for(std::list<GiftData>::iterator it = cache->getGifts()->begin(); it != cache->getGifts()->end(); it++)
		{
			giftMap[getFactory()->getLanguageManager()->getElement(it->m_name)] = &(*it);
		}

		for(std::map<std::string,GiftData*>::iterator it = giftMap.begin(); it != giftMap.end(); it++)
		{
			GiftData* dat = it->second;
			Button* btn = getFactory()->createWhiteButton();
            if(!Platf::isRetina())
			btn->setSize(185,185);
            else
            btn->setSize(185 * 1.5f,185 * 1.5f);
			btn->setScaleIcon(true);
			btn->setImage(&dat->m_img);
			btn->setImageScale(0.95f);
			btn->addMouseListener(this);
			btn->addActionListener(this);
			btn->setEnterCursor(agui::CursorProvider::LINK_CURSOR);
            agui::Font* f = NULL;
            if(!Platf::isRetina())
			f = getFactory()->getFontManager()->getFont(12);
            else
            f = getFactory()->getFontManager()->getFont((int)(12 * 1.75f));
			int lineH = f->getLineHeight();

			int b = btn->getMargin(agui::SIDE_BOTTOM);
			btn->setFont(f);
			btn->setMargins(
				btn->getMargin(agui::SIDE_TOP),
				btn->getMargin(agui::SIDE_LEFT),
				btn->getMargin(agui::SIDE_BOTTOM) + lineH,
				btn->getMargin(agui::SIDE_RIGHT)
				);
			btn->setDrawTextOutsideWithOffset(true,-b - 3);
			btn->setTextAlignment(agui::ALIGN_BOTTOM_CENTER);
			btn->setText(it->first);
			btn->setWantEntireIntersection(true);
			btn->setOnlyRenderOnScreen(true);
			flow->add(btn);
			m_ids[btn] = dat->id;
			m_categoryMap[btn] = dat->m_category;
		}

		m_emptyPadding = getFactory()->createEmptyWidget();
		m_emptyPadding->setSize(10,1);
		m_rawCategories.push_back("category.all");
		m_rawCategories.push_back("category.food");
		m_rawCategories.push_back("category.drink");
		m_rawCategories.push_back("category.fun");
		m_rawCategories.push_back("category.animated");

		m_categories = getFactory()->createDropDown();
		m_categories->setText(getFactory()->getLanguageManager()->getElement("category.category"));

		for(int i = 0; i < m_rawCategories.size(); i++)
		{
			std::string text = getFactory()->getLanguageManager()->getElement(m_rawCategories[i]);
			m_categories->addItem(text);
		}

		m_categories->resizeToContents();
		m_categories->addSelectionListener(this);
		m_parentWidgets.push_back(m_emptyPadding);
		m_parentWidgets.push_back(m_categories);
		setTitle("select.gift");
		setOkText("close");
	}

	void GiftSelectionDialog::actionPerformed( const agui::ActionEvent &evt )
	{
		GeneralDialogBox::actionPerformed(evt);

		if(m_ids.count(evt.getSource()) > 0)
		{
			int id = m_ids[evt.getSource()];
			m_selectedID = id;

			DISPATCH_GAME_EVENT
				(*it)->sendGift(m_receiver,m_selectedID);
			hideGeneralFrame();
	
		}
	}

	bool GiftSelectionDialog::wantOkButton() const
	{
		return true;
	}

	void GiftSelectionDialog::setReceiver( int rec )
	{
		m_receiver = rec;
	}

	std::string GiftSelectionDialog::getSideLabelText() const
	{
		return m_cost;
	}

	void GiftSelectionDialog::setCost( const std::string& cost )
	{
		if(cost != "1")
		m_cost = getFactory()->
		getLanguageManager()->getElementWithArg("gift.cost",StringUtil::commaSeparate(cost));
		else
			m_cost = getFactory()->
			getLanguageManager()->getElement("gift.cost.one");
	}

	const std::vector<agui::Widget*>& GiftSelectionDialog::parentBottomWidgets()
	{
		return m_parentWidgets;
	}

	void GiftSelectionDialog::selectionChanged( agui::Widget *source, const std::string &item, int index, bool selected )
	{
		if(source == m_categories)
		{
			int index = m_categories->getSelectedIndex();
			if(index < 0)
			{
				m_categories->setText(getFactory()->getLanguageManager()->getElement("category.category"));
				return;
			}
			std::string keyStr;
			if(index <= 0)
			{
				keyStr = m_rawCategories[0];
			}
			else if(index < m_rawCategories.size())
			{
				keyStr = m_rawCategories[index];
			}

			for(std::map<agui::Widget*,std::string>::iterator iterator = m_categoryMap.begin(); iterator != m_categoryMap.end(); iterator++) {
				agui::Widget* btn = iterator->first;
				std::string btnCategory = iterator->second;

				bool visible = keyStr == btnCategory;
				if(keyStr == "category.all")
					visible = true;

				btn->setVisibility(visible);
			}

			forceResize();
			scrollToTop();
		}
	}

}
