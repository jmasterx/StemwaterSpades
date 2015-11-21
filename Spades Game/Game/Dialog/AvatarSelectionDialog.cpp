#include "Game/Dialog/AvatarSelectionDialog.hpp"
#include <set>
#define  MSG_CONFIRM_SELECTION 1
namespace cge
{
	AvatarSelectionDialog::~AvatarSelectionDialog(void)
	{
	}

	void AvatarSelectionDialog::dialogWillAppear()
	{
		scrollToTop();
	}

	void AvatarSelectionDialog::messageBoxResult( MessageBox* msgbox,MessageBox::MessageResult result )
	{
		if(msgbox->getTag() == MSG_CONFIRM_SELECTION && result == MessageBox::MR_YES)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->changeAvatar(m_selectedID);
			hideGeneralFrame();
		}
	}

	void AvatarSelectionDialog::frameActionReceived( GeneralFrame* frame,const std::string& action )
	{
		hideGeneralFrame();
	}

	bool AvatarSelectionDialog::wantCloseButton() const
	{
		return false;
	}

	AvatarSelectionDialog::AvatarSelectionDialog( GuiFactory* factory, AvatarCache* cache )
		: GeneralDialogBox(factory,factory->getLanguageManager(),factory->getGuiColorManager(),
		factory->getFontManager()),m_selectedID(-1),m_force(false)
	{
		FlowLayout* flow = addFlow();
		flow->setHorizontallyCentered(true);
		flow->setAlignLastRow(true);
		flow->setHorizontalSpacing(15);
		flow->setVerticalSpacing(15);
		flow->setMargins(15,1,15,1);

		bool mustBeMale = true;
		bool hasAny = true;
		bool hasMale = true;
		bool hasFemale = true;
		bool selectAny = false;
		std::set<int> ids;

		while(hasAny)
		{
			hasAny = false;
			hasMale = false;
			hasFemale = false;
			for(std::map<int,CachedAvatarImage>::iterator it = cache->getCachedAvatars()->begin(); it != cache->getCachedAvatars()->end(); it++)
			{
				if(it->second.reserved)
					continue;

				if(ids.find(it->second.id) != ids.end())
					continue;

				if(it->second.gender == AVATAR_CACHE_GENDER_MALE)
				{
					hasMale = true;
				}
				else
				{
					hasFemale = true;
				}

				if(selectAny || (it->second.gender == AVATAR_CACHE_GENDER_MALE && mustBeMale) || 
					(it->second.gender == AVATAR_CACHE_GENDER_FEMALE && !mustBeMale))
				{
					Button* btn = getFactory()->createWhiteButton();
                    if(!Platf::isRetina())
					btn->setSize(256,256);
                    else
                        btn->setSize(330, 330);
					btn->setScaleIcon(true);
					btn->setImage(&it->second.guiImg);
					btn->setImageScale(1.0f);
					btn->addMouseListener(this);
					btn->addActionListener(this);
					btn->setEnterCursor(agui::CursorProvider::LINK_CURSOR);
					btn->setOnlyRenderOnScreen(true);
					flow->add(btn);
					m_ids[btn] = it->first;

					mustBeMale = !mustBeMale;
					hasAny = true;
					ids.insert(it->second.id);
				}
			}

			if(!hasFemale || !hasMale)
			{
				selectAny = true;
			}
		}

		setTitle("select.avatar");
		setOkText("close");
	}

	void AvatarSelectionDialog::actionPerformed( const agui::ActionEvent &evt )
	{
		GeneralDialogBox::actionPerformed(evt);

		if(m_ids.count(evt.getSource()) > 0)
		{
			int id = m_ids[evt.getSource()];
			m_selectedID = id;
			DISPATCH_SCENE_EVENT
				(*it)->showMessageBox(getFactory()->getLanguageManager()->getElement("confirm.selection.title"),
				getFactory()->getLanguageManager()->getElement("confirm.selection.text"),
				MessageBox::MT_YES_NO,this,MSG_CONFIRM_SELECTION);
		}
	}

	void AvatarSelectionDialog::setForce( bool force )
	{
		m_force = force;
	}

	bool AvatarSelectionDialog::wantOkButton() const
	{
		return !m_force;
	}

	float AvatarSelectionDialog::getWidthScalar() const
	{
		return 0.8f;
	}

	float AvatarSelectionDialog::getHeightScalar() const
	{
		return 0.8f;
	}

}