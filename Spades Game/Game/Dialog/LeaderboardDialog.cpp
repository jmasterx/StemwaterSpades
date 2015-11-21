#include "Game/Dialog/LeaderboardDialog.hpp"
#include "Game/Utility/StringUtil.hpp"
namespace cge
{
	LeaderboardDialog::LeaderboardDialog( GuiFactory* factory )
		: GeneralDialogBox(factory,factory->getLanguageManager(),factory->getGuiColorManager(),factory->getFontManager())
	{
		m_container = addFlow();
		m_container->setResizeRowToWidth(true);
		setOkText("close");
		m_columnList.push_back("leaderboard.rating");
		m_columnList.push_back("leaderboard.coins");
		m_columnList.push_back("leaderboard.games.completed");
		m_columnList.push_back("leaderboard.games.substituted");
		m_columnList.push_back("leaderboard.games.abandoned");
		m_drop = getFactory()->createDropDown();
		m_drop->addSelectionListener(this);
		for(int i = 0; i < m_columnList.size(); i++)
		{
			m_drop->addItem(getFactory()->getLanguageManager()->getElement(m_columnList[i]));
		}
		m_drop->resizeToContents();
		m_drop->setSelectedIndex(0);
		m_selColumn = m_columnList[m_drop->getSelectedIndex()];

		m_parentWidgets.push_back(m_drop);
	}

	LeaderboardDialog::~LeaderboardDialog(void)
	{
		m_drop->removeSelectionListener(this);
	}

	void LeaderboardDialog::addHeaderCell()
	{
		addCell(getFactory()->getLanguageManager()->getElement("leaderboard.rank"),
			getFactory()->getLanguageManager()->getElement("leaderboard.name")
			,getFactory()->getLanguageManager()->getElement(m_selColumn),true,false);
	}

	Label* LeaderboardDialog::createLeaderboardLabel( const std::string& text, agui::Widget* parent, bool header, bool odd )
	{
		Label* label = getFactory()->createLabel();
        if(Platf::isRetina())
            label->setFont(getFactory()->getFontManager()->getFont(2.0f));
		if(header)
		{
			label->setFont(getFactory()->getFontManager()->getFont(label->getFont(),1.4f));
			label->setFontColor(agui::Color(
				label->getFontColor().getR() - 0.1f,
				label->getFontColor().getG() - 0.1f,
				label->getFontColor().getB() - 0.1f
				));
		}
		if(odd)
		{
			label->setFontColor(agui::Color(
				label->getFontColor().getR() + 0.15f,
				label->getFontColor().getG() + 0.15f,
				label->getFontColor().getB() + 0.15f
				));
		}
		label->setText(text);
		label->addMouseListener(this);
		parent->add(label);
		return label;
	}

	void LeaderboardDialog::addCell( const std::string& rank, const std::string& name, const std::string& rating, bool header, bool odd )
	{
		LeaderboardCell cell;
		cell.container = getFactory()->createEmptyWidget();
		cell.container->addMouseListener(this);
		cell.container->setMargins(1,1,1,5);
		cell.rank = createLeaderboardLabel(rank,cell.container,header,odd);
		cell.name = createLeaderboardLabel(name,cell.container,header,odd);
		cell.rating = createLeaderboardLabel(rating,cell.container,header,odd);

		int nameOffset = 0;
		if(header)
			nameOffset = cell.rank->getWidth();
		else if(m_cells.size() > 0)
			nameOffset = m_cells[0].rank->getWidth();
		else
			nameOffset = 80;

		nameOffset += 20;
		int headerExtra = header ? 5 : 0;
		cell.container->setSize(0,cell.rank->getHeight() + headerExtra + 5);
		m_container->add(cell.container);
		cell.container->add(cell.rank);
		cell.container->add(cell.name);
		cell.container->add(cell.rating);
		int yPos = 0;
		cell.rank->setLocation(0,yPos);
		cell.name->setLocation(nameOffset,yPos);
		cell.rating->setLocation(cell.container->getInnerWidth() - cell.rating->getWidth(),yPos);
		m_cells.push_back(cell);
	}

	void LeaderboardDialog::clearCells()
	{
		for(int i = 0; i < m_cells.size(); i++)
		{
			LeaderboardCell cell = m_cells[i];

			m_container->remove(cell.container);
			cell.container->clear();
			cell.container->removeMouseListener(this);
			cell.rank->removeMouseListener(this);
			cell.name->removeMouseListener(this);
			cell.rating->removeMouseListener(this);

			getFactory()->deleteWidget(cell.rank);
			getFactory()->deleteWidget(cell.name);
			getFactory()->deleteWidget(cell.rating);
			getFactory()->deleteWidget(cell.container);
		}
		m_cells.clear();
	}

	void LeaderboardDialog::dialogWillAppear()
	{
		clearCells();
		addHeaderCell();
		for(int i = 0; i < m_cellData.size(); i++)
		{
			addCell(m_cellData[i].rank,m_cellData[i].name,m_cellData[i].rating,false, !(i % 2 == 0));
		}
		forceResize();
		repositionCells();
		scrollToTop();
	}

	void LeaderboardDialog::repositionCells()
	{
		for(int i = 0; i < m_cells.size(); i++)
		{
			m_cells[i].rating->setLocation(
				m_cells[i].container->getInnerWidth() - 
				m_cells[i].rating->getWidth(),
				m_cells[i].rating->getLocation().getY());
		}
	}

	void LeaderboardDialog::setData( const std::vector<std::string>& rank, const std::vector<std::string>& name, 
		const std::vector<std::string>& rating, int numPerPage, int pageNo, int count, const std::string& dataColumn, bool alreadyShowing )
	{
		setTitle("leaderboard.title",StringUtil::toString(numPerPage));
		m_cellData.clear();
		for(int i = 0; i < rank.size(); i++)
		{
			LeaderboardCellData data;
			data.rank = rank[i];
			data.name = name[i];
			if(dataColumn == "leaderboard.rating")
				data.rating = rating[i];
			else if(dataColumn == "leaderboard.coins")
				data.rating = "$" + StringUtil::commaSeparate(rating[i]);
			else 
				data.rating = StringUtil::commaSeparate(rating[i]);
				
			m_cellData.push_back(data);
		}

		m_selColumn = dataColumn;

		if(alreadyShowing)
			dialogWillAppear();
	}

	bool LeaderboardDialog::wantCloseButton() const
	{
		return false;
	}

	void LeaderboardDialog::frameActionReceived( GeneralFrame* frame,const std::string& action )
	{
		hideGeneralFrame();
	}

	const std::string& LeaderboardDialog::getSelectedDataColumn() const
	{
		if(m_drop->getSelectedIndex() == -1)
			return m_selColumn;
		else
		return m_columnList[m_drop->getSelectedIndex()];
	}

	const std::vector<agui::Widget*>& LeaderboardDialog::parentBottomWidgets()
	{
		return m_parentWidgets;
	}

	void LeaderboardDialog::actionPerformed( const agui::ActionEvent &evt )
	{
		GeneralDialogBox::actionPerformed(evt);
	}

	void LeaderboardDialog::selectionChanged( agui::Widget *source, const std::string &item, int index, bool selected )
	{
		if(source == m_drop)
		{
			if(m_drop->getSelectedIndex() != -1)
			{
				DISPATCH_LOBBY_EVENT
					(*it)->showLeaderboard(0,0,m_columnList[m_drop->getSelectedIndex()],true);
			}
		}
	}

	float LeaderboardDialog::getHeightScalar() const
	{
		return 0.8f;
	}

	float LeaderboardDialog::getWidthScalar() const
	{
		return 0.8f;
	}

}