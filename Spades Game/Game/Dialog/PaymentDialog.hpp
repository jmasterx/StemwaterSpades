#ifndef CGE_PAYMENT_DIALOG_HPP
#define CGE_PAYMENT_DIALOG_HPP
#include "Game/UI/GeneralDialogBox.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
#include <vector>
namespace cge
{
	struct PaymentCell
	{
		Label* lblPackName;
		Label* lblSalePercent;
		Label* lblBonusAmount;
		Label* lblPrice;
		Button* btnBuy;
		ImageIcon* imgCoin;
		agui::Widget* container;
		PaymentCell()
			: lblBonusAmount(NULL),
			lblPackName(NULL),
			lblPrice(NULL),
			lblSalePercent(NULL),
			btnBuy(NULL),
			imgCoin(NULL),
			container(NULL)
		{

		}
	};

	struct PaymentCellData
	{
		std::string packName;
		std::string salePercent;
		std::string bonusAmount;
		std::string price;
		std::string url;
	};

	class PaymentDialog : public GeneralDialogBox, public LobbyEventProvider
	{
		FlowLayout* m_container;
		std::vector<PaymentCell> m_cells;
		std::vector<PaymentCellData> m_cellData;
        double m_lastClickTime;
		Label* createPaymentLabel(const std::string& text, agui::Widget* parent, float fontScalar = 1.0f);
		void clearCells();
		void addCell(const std::string& packName,
			const std::string& salePercent,
			const std::string& bonusAmount, 
			const std::string& price);
		void repositionCells();
		virtual void dialogWillAppear();
	public:
		PaymentDialog(GuiFactory* factory);
		void setData(const std::vector<std::string>& coinAmounts,
			const std::vector<std::string>& prices,
			const std::vector<std::string>& salePercents,
			const std::vector<std::string>& coinPercents,
			const std::vector<std::string>& urls
			);
		virtual bool wantCloseButton() const;
		virtual void frameActionReceived(GeneralFrame* frame,const std::string& action);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void onParentSizeChanged(agui::Widget* source, const agui::Dimension &parentInnerSize);
		virtual void receiveStoreLink(const std::string& link);
		virtual ~PaymentDialog(void);
	};
}
#endif
