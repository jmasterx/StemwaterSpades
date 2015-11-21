#include "Game/Dialog/FriendLobbyChangeDialog.hpp"
#include "Game/Utility/StringUtil.hpp"
namespace cge
{
	FriendLobbyChangeDialog::FriendLobbyChangeDialog( GuiFactory* factory )
		: GeneralDialogBox(factory,factory->getLanguageManager(),factory->getGuiColorManager(),factory->getFontManager()),
		m_acceptChanges(true),m_port(0)
	{
		m_container = addFlow();
		m_container->setResizeRowToWidth(true);
		setTitle("find.friends");
		setOkText("close");
	}

	FriendLobbyChangeDialog::~FriendLobbyChangeDialog(void)
	{
	}

	bool FriendLobbyChangeDialog::wantCloseButton() const
	{
		return false;
	}

	void FriendLobbyChangeDialog::frameActionReceived( GeneralFrame* frame,const std::string& action )
	{
		hideGeneralFrame();
	}

	void FriendLobbyChangeDialog::setData( const std::vector<std::string>& names, const std::vector<std::string>& ips, const std::vector<int>& ports, const std::vector<int>& capacities, const std::vector<int>& numUsers, const std::vector<std::string>& serverNames )
	{
		m_cellData.clear();
		for(int i = 0; i < names.size(); i++)
		{
			FriendLobbyChangeCellData data;
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
			data.friendName = names[i];
			data.ip = ips[i];
			data.port = ports[i];

				data.serverName = serverNames[i];

			m_cellData.push_back(data);
		}
	}

	Label* FriendLobbyChangeDialog::createFriendLobbyChangeLabel( const std::string& text, agui::Widget* parent, float fontScalar /*= 1.0f*/ )
	{
		Label* label = getFactory()->createLabel();
		label->setText(text);
		label->addMouseListener(this);
		label->setFont(getFactory()->getFontManager()->getFont((agui::Font*)label->getFont(),fontScalar));
		parent->add(label);
		return label;
	}

	void FriendLobbyChangeDialog::addCell( const std::string& lobbyName,
		const std::string& friendCount, 
		const std::string& numUsers, bool full, bool currentLobby )
	{
		FriendLobbyChangeCell cell;
		cell.container = getFactory()->createToolContainer("tooltip.bg");
		cell.container->addMouseListener(this);
		cell.container->setMargins(5,7,7,7);
		float baseFontScalar = 1.0f;
        float addedButton = 1.0f;
        if(Platf::isRetina()) {
            baseFontScalar = 2.0f;
            addedButton = 1.25f;
        }

		cell.lblFriendName = createFriendLobbyChangeLabel(lobbyName,cell.container,baseFontScalar * 1.1f);
		cell.lblNumUsers = createFriendLobbyChangeLabel(numUsers,cell.container,baseFontScalar);
		cell.lblServerName = createFriendLobbyChangeLabel(friendCount,cell.container,baseFontScalar * 0.8f);
		cell.lblServerName->setFontColor(agui::Color(cell.lblFriendName->getFontColor().getR() - 0.1f,
			cell.lblFriendName->getFontColor().getG() - 0.1f,cell.lblFriendName->getFontColor().getB() - 0.1f));
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

	void FriendLobbyChangeDialog::repositionCells()
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

			m_cells[i].lblFriendName->setLocation(lbStartX,(h - m_cells[i].lblFriendName->getHeight()) / 2);

			m_cells[i].lblServerName->setLocation(lbStartX,
				m_cells[i].lblFriendName->getLocation().getY() +
				m_cells[i].lblFriendName->getHeight());

			m_cells[i].lblNumUsers->setLocation(
				m_cells[i].btnGo->getLocation().getX() -
				m_cells[i].lblNumUsers->getWidth() - pad,
				(h - m_cells[i].lblNumUsers->getHeight()) / 2);
		}
	}

	void FriendLobbyChangeDialog::clearCells()
	{
		for(int i = 0; i < m_cells.size(); i++)
		{
			FriendLobbyChangeCell cell = m_cells[i];

			m_container->remove(cell.container);
			cell.container->clear();
			cell.container->removeMouseListener(this);
			cell.btnGo->removeMouseListener(this);
			cell.lblServerName->removeMouseListener(this);
			cell.lblFriendName->removeMouseListener(this);
			cell.lblNumUsers->removeMouseListener(this);
			cell.btnGo->removeActionListener(this);

			getFactory()->deleteWidget(cell.btnGo);
			getFactory()->deleteWidget(cell.lblServerName);
			getFactory()->deleteWidget(cell.lblFriendName);
			getFactory()->deleteWidget(cell.lblNumUsers);
			getFactory()->deleteWidget(cell.container);
		}
		m_cells.clear();
	}

	void FriendLobbyChangeDialog::actionPerformed( const agui::ActionEvent &evt )
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

	void FriendLobbyChangeDialog::dialogWillAppear()
	{
		setAcceptChanges(true);
		clearCells();
		for(int i = 0; i < m_cellData.size(); i++)
		{
			bool currentLobby = (m_cellData[i].ip == m_address &&
				m_cellData[i].port == m_port);
			addCell(m_cellData[i].friendName,m_cellData[i].serverName,m_cellData[i].numUsers,m_cellData[i].full,
				currentLobby);
		}
		forceResize();
	}

	void FriendLobbyChangeDialog::onParentSizeChanged( agui::Widget* source, const agui::Dimension &parentInnerSize )
	{
		GeneralDialogBox::onParentSizeChanged(source,parentInnerSize);
		repositionCells();
	}

	void FriendLobbyChangeDialog::setAcceptChanges( bool accept )
	{
		m_acceptChanges = accept;
	}

	bool FriendLobbyChangeDialog::isAcceptingChanges() const
	{
		return m_acceptChanges;
	}

	void FriendLobbyChangeDialog::receiveServerFull( const std::string& serverName, const std::string& ip, int port, bool full )
	{
		setAcceptChanges(true);
		if(getWidget()->isVisible())
		{
			if(!full)
				hideGeneralFrame();
		}
	}

	void FriendLobbyChangeDialog::setCurrentAddressAndPort( const std::string& address, int port )
	{
		m_address = address;
		m_port = port;
	}

}
