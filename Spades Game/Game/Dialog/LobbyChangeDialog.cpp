#include "Game/Dialog/LobbyChangeDialog.hpp"
#include "Game/Utility/StringUtil.hpp"
namespace cge
{
	LobbyChangeDialog::LobbyChangeDialog( GuiFactory* factory )
		: GeneralDialogBox(factory,factory->getLanguageManager(),factory->getGuiColorManager(),factory->getFontManager()),
		m_acceptChanges(true),m_port(0)
	{
		m_container = addFlow();
		m_container->setResizeRowToWidth(true);
		setTitle("select.lobby");
		setOkText("close");
	}

	LobbyChangeDialog::~LobbyChangeDialog(void)
	{
	}

	bool LobbyChangeDialog::wantCloseButton() const
	{
		return false;
	}

	void LobbyChangeDialog::frameActionReceived( GeneralFrame* frame,const std::string& action )
	{
		hideGeneralFrame();
	}

	void LobbyChangeDialog::setData( const std::vector<std::string>& names, const std::vector<std::string>& ips, const std::vector<int>& ports, const std::vector<int>& capacities, const std::vector<int>& numUsers, const std::vector<int>& numFriends )
	{
		m_cellData.clear();
		for(int i = 0; i < names.size(); i++)
		{
			LobbyChangeCellData data;
			if(capacities[i] == numUsers[i])
			{
				data.full = true;
				data.numUsers = getFactory()->getLanguageManager()->getElement("full");
			}
			else
			{
				std::stringstream ss;
				data.full = false;
				ss << numUsers[i] << "/" << capacities[i];
				data.numUsers = ss.str();
			}
			data.lobbyName = names[i];
			data.ip = ips[i];
			data.port = ports[i];
			
			if(numFriends[i] > 0)
			{
				data.friendCount = 
					getFactory()->getLanguageManager()->getElementWithArg("n.friends",StringUtil::toString(numFriends[i]));
			}

			m_cellData.push_back(data);
		}
	}

	Label* LobbyChangeDialog::createLobbyChangeLabel( const std::string& text, agui::Widget* parent, float fontScalar /*= 1.0f*/ )
	{
		Label* label = getFactory()->createLabel();
		label->setText(text);
		label->addMouseListener(this);
		label->setFont(getFactory()->getFontManager()->getFont((agui::Font*)label->getFont(),fontScalar));
		parent->add(label);
		return label;
	}

	void LobbyChangeDialog::addCell( const std::string& lobbyName,
		const std::string& friendCount, 
		const std::string& numUsers, bool full, bool currentLobby )
	{
		LobbyChangeCell cell;
		cell.container = getFactory()->createToolContainer("tooltip.bg");
		cell.container->addMouseListener(this);
		cell.container->setMargins(5,7,7,7);
        float baseFontScalar = 1.0f;
        float addedButton = 1.0f;
        if(Platf::isRetina()) {
            baseFontScalar = 2.0f;
            addedButton = 1.25f;
        }

		cell.lblLobbyName = createLobbyChangeLabel(lobbyName,cell.container,baseFontScalar * 1.1f);
		cell.lblNumUsers = createLobbyChangeLabel(numUsers,cell.container,baseFontScalar);
		cell.lblFriendCount = createLobbyChangeLabel(friendCount,cell.container,baseFontScalar * 0.8f);
		cell.lblFriendCount->setFontColor(agui::Color(0.0f,0.4f,0.1f));
		cell.btnGo =  getFactory()->createButton();
        cell.btnGo->setFont(getFactory()->getFontManager()->getFont(baseFontScalar * addedButton));
		cell.btnGo->setContentResizeScale(1.5f,1.5f);
		cell.btnGo->setText(getFactory()->getLanguageManager()->getElement("go"));
		cell.btnGo->resizeToContents();
		cell.btnGo->addActionListener(this);
		cell.btnGo->setFocusable(false);
		cell.btnGo->setVisibility(!full);

		if(currentLobby)
			cell.btnGo->setVisibility(false);

		cell.container->add(cell.btnGo);

		cell.container->setSize(0,65 * baseFontScalar);
		m_container->add(cell.container);

		m_cells.push_back(cell);
	}

	void LobbyChangeDialog::repositionCells()
	{
        float baseFontScalar = 1.0f;
        if(Platf::isRetina())
            baseFontScalar = 2.0f;
        
		int pad = 10 * baseFontScalar;
		for(int i = 0; i < m_cells.size(); i++)
		{
			int w = m_cells[i].container->getInnerWidth();
			int h = m_cells[i].container->getInnerHeight();

			Button* b =  m_cells[i].btnGo;
			b->alignToParent(agui::ALIGN_MIDDLE_RIGHT);

			int lbStartX = 1 + pad;

			m_cells[i].lblLobbyName->setLocation(lbStartX,(h - m_cells[i].lblLobbyName->getHeight()) / 2);

			m_cells[i].lblFriendCount->setLocation(lbStartX,
				m_cells[i].lblLobbyName->getLocation().getY() +
				m_cells[i].lblLobbyName->getHeight());

			m_cells[i].lblNumUsers->setLocation(
				m_cells[i].btnGo->getLocation().getX() -
				m_cells[i].lblNumUsers->getWidth() - pad,
				(h - m_cells[i].lblNumUsers->getHeight()) / 2);
		}
	}

	void LobbyChangeDialog::clearCells()
	{
		for(int i = 0; i < m_cells.size(); i++)
		{
			LobbyChangeCell cell = m_cells[i];

			m_container->remove(cell.container);
			cell.container->clear();
			cell.container->removeMouseListener(this);
			cell.btnGo->removeMouseListener(this);
			cell.lblFriendCount->removeMouseListener(this);
			cell.lblLobbyName->removeMouseListener(this);
			cell.lblNumUsers->removeMouseListener(this);
			cell.btnGo->removeActionListener(this);

			getFactory()->deleteWidget(cell.btnGo);
			getFactory()->deleteWidget(cell.lblFriendCount);
			getFactory()->deleteWidget(cell.lblLobbyName);
			getFactory()->deleteWidget(cell.lblNumUsers);
			getFactory()->deleteWidget(cell.container);
		}
		m_cells.clear();
	}

	void LobbyChangeDialog::actionPerformed( const agui::ActionEvent &evt )
	{
		GeneralDialogBox::actionPerformed(evt);

		if(isAcceptingChanges())
		{
			for(int i = 0; i < m_cells.size();i++)
			{
				if(m_cells[i].btnGo == evt.getSource())
				{
					DISPATCH_LOBBY_EVENT
						(*it)->requestServerFull(m_cellData[i].ip,m_cellData[i].port);
					setAcceptChanges(false);
					break;
				}
			}
		}
	}

	void LobbyChangeDialog::dialogWillAppear()
	{
		setAcceptChanges(true);
		clearCells();
		for(int i = 0; i < m_cellData.size(); i++)
		{
			bool currentLobby = (m_cellData[i].ip == m_address &&
				m_cellData[i].port == m_port);
			addCell(m_cellData[i].lobbyName,m_cellData[i].friendCount,m_cellData[i].numUsers,m_cellData[i].full,
				currentLobby);
		}
		forceResize();
	}

	void LobbyChangeDialog::onParentSizeChanged( agui::Widget* source, const agui::Dimension &parentInnerSize )
	{
		GeneralDialogBox::onParentSizeChanged(source,parentInnerSize);
		repositionCells();
	}

	void LobbyChangeDialog::setAcceptChanges( bool accept )
	{
		m_acceptChanges = accept;
	}

	bool LobbyChangeDialog::isAcceptingChanges() const
	{
		return m_acceptChanges;
	}

	void LobbyChangeDialog::receiveServerFull( const std::string& serverName, const std::string& ip, int port, bool full )
	{
		setAcceptChanges(true);
		if(getWidget()->isVisible())
		{
			if(!full)
				hideGeneralFrame();
		}
	}

	void LobbyChangeDialog::setCurrentAddressAndPort( const std::string& address, int port )
	{
		m_address = address;
		m_port = port;
	}

}
