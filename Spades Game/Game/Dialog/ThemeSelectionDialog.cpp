#include "Game/Dialog/ThemeSelectionDialog.hpp"
#include <map>
#define  MSG_CONFIRM_SELECTION 1
namespace cge
{
	ThemeSelectionDialog::~ThemeSelectionDialog(void)
	{
		//m_theme->destroyThumbs();
	}

	void ThemeSelectionDialog::dialogWillAppear()
	{
		scrollToTop();
	}

	void ThemeSelectionDialog::messageBoxResult( MessageBox* msgbox,MessageBox::MessageResult result )
	{
		if(msgbox->getTag() == MSG_CONFIRM_SELECTION && result == MessageBox::MR_YES)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->changeTheme(m_selectedID);
			hideGeneralFrame();
		}
	}

	void ThemeSelectionDialog::frameActionReceived( GeneralFrame* frame,const std::string& action )
	{
		hideGeneralFrame();
	}

	bool ThemeSelectionDialog::wantCloseButton() const
	{
		return false;
	}

	ThemeSelectionDialog::ThemeSelectionDialog( GuiFactory* factory, ThemeManager* cache )
		: GeneralDialogBox(factory,factory->getLanguageManager(),factory->getGuiColorManager(),
		factory->getFontManager()),m_selectedID(-1),m_theme(cache)
	{
		//m_theme->loadThumbs();
		FlowLayout* flow = addFlow();
		flow->setHorizontallyCentered(true);
		flow->setAlignLastRow(true);
		flow->setHorizontalSpacing(15);
		flow->setVerticalSpacing(15);
		flow->setMargins(15,1,15,1);

		std::map<int,ThemeData*> dat;

		for(std::list<ThemeData>::iterator it = cache->getThemes()->begin(); it != cache->getThemes()->end(); it++)
		{
			dat[it->vid] = &(*it);
		}

		for(std::map<int,ThemeData*>::iterator iter = dat.begin(); iter != dat.end(); iter++)
		{
			ThemeData* it = iter->second;

			Button* btn = getFactory()->createWhiteButton();
			btn->setScaleIcon(true);
			btn->setImage(&it->m_img);
			btn->setImageScale(1.7f);
			btn->addMouseListener(this);
			btn->addActionListener(this);
			btn->setEnterCursor(agui::CursorProvider::LINK_CURSOR);
			btn->setOnlyRenderOnScreen(true);

            float retScal = 1.3f;
            agui::Font* f = NULL;
            if(!Platf::isRetina())
           f = getFactory()->getFontManager()->getFont(12);
            else
           f = getFactory()->getFontManager()->getFont((int)(12 * retScal * 1.1f));
            
			int lineH = f->getLineHeight();

			int b = btn->getMargin(agui::SIDE_BOTTOM);
			btn->setFont(f);
			btn->setMargins(
				btn->getMargin(agui::SIDE_TOP),
				btn->getMargin(agui::SIDE_LEFT),
				btn->getMargin(agui::SIDE_BOTTOM) + lineH,
				btn->getMargin(agui::SIDE_RIGHT)
				);

            if(!Platf::isRetina())
			btn->setSize(450,256 + lineH + 3);
            else
            btn->setSize(450 * retScal,(256 * retScal) + lineH + 3);
            
			btn->setDrawTextOutsideWithOffset(true,-b - 3);
			btn->setTextAlignment(agui::ALIGN_BOTTOM_CENTER);
			btn->setText(getFactory()->getLanguageManager()->getElement(it->m_name));
			btn->setWantEntireIntersection(true);

			flow->add(btn);
			m_ids[btn] = it->id;
		}

		setTitle("select.theme");
		setOkText("close");
	}

	void ThemeSelectionDialog::actionPerformed( const agui::ActionEvent &evt )
	{
		GeneralDialogBox::actionPerformed(evt);

		if(m_ids.count(evt.getSource()) > 0)
		{
			int id = m_ids[evt.getSource()];
			m_selectedID = id;

				DISPATCH_SCENE_EVENT
					(*it)->showMessageBox(getFactory()->getLanguageManager()->getElement("confirm.theme.title"),
					getFactory()->getLanguageManager()->getElement("confirm.theme.text"),
					MessageBox::MT_YES_NO,this,MSG_CONFIRM_SELECTION);
		}
	}

	bool ThemeSelectionDialog::wantOkButton() const
	{
		return true;
	}

}
