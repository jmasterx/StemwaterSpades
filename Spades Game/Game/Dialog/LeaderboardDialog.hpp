#ifndef CGE_LEADERBOARD_DIALOG_HPP
#define CGE_LEADERBOARD_DIALOG_HPP
#include "Game/UI/GeneralDialogBox.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
#include <Agui/SelectionListener.hpp>
#include <vector>
namespace cge
{
	struct LeaderboardCell
	{
		Label* rank;
		Label* name;
		Label* rating;
		agui::EmptyWidget* container;
		LeaderboardCell()
			: rank(NULL),
			name(NULL),
			rating(NULL),
			container(NULL)
		{

		}
	};

	struct LeaderboardCellData
	{
		std::string rank;
		std::string name;
		std::string rating;
	};
	class LeaderboardDialog : public GeneralDialogBox, public LobbyEventProvider,
		public agui::SelectionListener
	{
		FlowLayout* m_container;
		DropDown* m_drop;
		std::vector<agui::Widget*> m_parentWidgets;
		std::vector<LeaderboardCell> m_cells;
		std::vector<LeaderboardCellData> m_cellData;
		std::vector<std::string> m_columnList;
		std::string m_selColumn;
		Label* createLeaderboardLabel(const std::string& text, agui::Widget* parent, bool header, bool odd);
		void clearCells();
		void addHeaderCell();
		void addCell(const std::string& rank, const std::string& name, const std::string& rating, bool header, bool odd);
		void repositionCells();
		virtual void dialogWillAppear();
	public:
		LeaderboardDialog(GuiFactory* factory);
		void setData(const std::vector<std::string>& rank,
			const std::vector<std::string>& name,
			const std::vector<std::string>& rating,
			int numPerPage,
			int pageNo,
			int count,
			const std::string& dataColumn,
			bool alreadyShowing
			);
		virtual bool wantCloseButton() const;
		virtual void frameActionReceived(GeneralFrame* frame,const std::string& action);
		const std::string& getSelectedDataColumn() const;
		virtual const std::vector<agui::Widget*>& parentBottomWidgets();
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void selectionChanged(agui::Widget *source, const std::string &item, int index, bool selected);
		float getHeightScalar() const;
		float getWidthScalar() const;
		virtual ~LeaderboardDialog(void);
	};
}
#endif
