#include "Game/Dialog/PaymentDialog.hpp"
#include "Game/Utility/StringUtil.hpp"
#include "Game/platform.hpp"
#ifdef CGE_IPHONE
#include "Game/Platform/IAPHandler.h"
#endif
#ifdef CGE_OSX
#include "Game/Platform/OSXHandler.h"
#endif
namespace cge
{
	PaymentDialog::PaymentDialog( GuiFactory* factory )
		: GeneralDialogBox(factory,factory->getLanguageManager(),factory->getGuiColorManager(),factory->getFontManager()),
    m_lastClickTime(0.0)
	{
		m_container = addFlow();
		m_container->setResizeRowToWidth(true);
		setTitle("coins.title");
		setOkText("close");
	}

	PaymentDialog::~PaymentDialog(void)
	{
	}

	bool PaymentDialog::wantCloseButton() const
	{
		return false;
	}

	void PaymentDialog::frameActionReceived( GeneralFrame* frame,const std::string& action )
	{
		hideGeneralFrame();
	}

	void PaymentDialog::setData( 
		const std::vector<std::string>& coinAmounts, const std::vector<std::string>& prices, const std::vector<std::string>& salePercents, const std::vector<std::string>& coinPercents, const std::vector<std::string>& urls )
	{
		m_cellData.clear();
		for(int i = 0; i < coinAmounts.size(); i++)
		{
			std::string numWithCommas = StringUtil::commaSeparate(coinAmounts[i]);

#ifdef CGE_IPHONE
			std::string price = "";
#else
            std::string price = "$";
#endif
			price += prices[i];

			PaymentCellData data;
			
#ifdef CGE_IPHONE
			data.bonusAmount = coinPercents[i];
            data.packName = coinAmounts[i];
#else
            data.packName = getFactory()->getLanguageManager()->getElementWithArg("n.coins",numWithCommas);

			float price0 = 0;
			float priceI = 0;
			float coin0 = 0;
			float coinI = 0;
			float delta0 = 0;
			float deltaI = 0;
			
			std::stringstream ss;
			ss << prices[0];
			ss >> price0;
			ss.clear();
			ss.str("");
			ss << prices[i];
			ss >> priceI;
			ss.clear();
			ss.str("");
			ss << coinAmounts[0];
			ss >> coin0;
			ss.clear();
			ss.str("");
			ss << coinAmounts[i];
			ss >> coinI;

			if(priceI > 0 && price0 > 0)
			{
				delta0 = coin0 / price0;
				deltaI = coinI / priceI;

				if(delta0 > 0)
				{
					float more = deltaI / delta0;
					int moreI = (int)((more - 1.0f) * 100.0f);
					if(moreI > 0)
					data.bonusAmount = StringUtil::toString(moreI);
				}
			}
#endif
			data.price = price;
			data.salePercent = salePercents[i];
			data.url = urls[i];
			m_cellData.push_back(data);
		}
	}

	Label* PaymentDialog::createPaymentLabel( const std::string& text, agui::Widget* parent, float fontScalar /*= 1.0f*/ )
	{
		Label* label = getFactory()->createLabel();
		label->setText(text);
		label->addMouseListener(this);
		label->setFont(getFactory()->getFontManager()->getFont((agui::Font*)label->getFont(),fontScalar));
		parent->add(label);
		return label;
	}


	void PaymentDialog::addCell( 
		const std::string& packName, const std::string& salePercent, const std::string& bonusAmount, const std::string& price )
	{
		PaymentCell cell;
		ImageIcon* ico = getFactory()->createImageIcon("coin");
		ico->addMouseListener(this);
		cell.container = getFactory()->createToolContainer("tooltip.bg");
		cell.container->addMouseListener(this);
		cell.container->setMargins(5,7,7,7);
		cell.imgCoin = ico;
		ico->setScaleToFit(true);
		cell.container->add(ico);

		float baseFontScalar = 1.0f;
        float addedButton = 1.0f;
        if(Platf::isRetina()) {
            baseFontScalar = 2.0f;
            addedButton = 1.25f;
        }

#ifdef CGE_IPHONE
		cell.lblPackName = createPaymentLabel(packName,cell.container,baseFontScalar * 1.1f);
#else
        cell.lblPackName = createPaymentLabel("$" + packName,cell.container,baseFontScalar * 1.1f);
#endif
		cell.lblPrice = createPaymentLabel(price,cell.container,baseFontScalar);
		cell.lblBonusAmount = createPaymentLabel("",cell.container,baseFontScalar * 0.8f);
		cell.lblSalePercent = createPaymentLabel("",cell.container,baseFontScalar * 1.1f);
		cell.lblBonusAmount->setFontColor(agui::Color(0.9f,0.0f,0.0f));
		//cell.lblSalePercent->setStrikeout(true);
		cell.lblSalePercent->setFontColor(agui::Color(0.9f,0.0f,0.0f));
		cell.btnBuy =  getFactory()->createButton();
        cell.btnBuy->setFont(getFactory()->getFontManager()->getFont(baseFontScalar * addedButton));
		cell.btnBuy->setContentResizeScale(1.5f,1.5f);
		cell.btnBuy->setText(getFactory()->getLanguageManager()->getElement("buy"));
		cell.btnBuy->resizeToContents();
		cell.btnBuy->addActionListener(this);
		cell.btnBuy->setFocusable(false);
		cell.container->add(cell.btnBuy);

		cell.container->setSize(0,65 * baseFontScalar);
		m_container->add(cell.container);

		if(salePercent.length() > 0 && salePercent != "0")
		{
			cell.lblBonusAmount->setText(getFactory()->getLanguageManager()->getElementWithArg("n.off",salePercent));
		}

		if(bonusAmount.length() > 0 && bonusAmount != "0")
		{
			cell.lblSalePercent->setText(getFactory()->getLanguageManager()->getElementWithArg("n.save",bonusAmount));
		}

		m_cells.push_back(cell);
	}

	void PaymentDialog::repositionCells()
	{
        float baseFontScalar = 1.0f;
        if(Platf::isRetina())
            baseFontScalar = 2.0f;
        
		int pad = 10 * baseFontScalar;
		for(int i = 0; i < m_cells.size(); i++)
		{
			int w = m_cells[i].container->getInnerWidth();
			int h = m_cells[i].container->getInnerHeight();

			Button* b =  m_cells[i].btnBuy;
			b->alignToParent(agui::ALIGN_MIDDLE_RIGHT);
			m_cells[i].imgCoin->setSize(30 * baseFontScalar,30 * baseFontScalar);
			m_cells[i].imgCoin->setLocation(0,
				(h - m_cells[i].imgCoin->getWidth()) / 2);

			int lbStartX = m_cells[i].imgCoin->getWidth() + pad;

			m_cells[i].lblPackName->setLocation(lbStartX,(h - m_cells[i].lblPackName->getHeight()) / 2);

			m_cells[i].lblSalePercent->setLocation(lbStartX +
				m_cells[i].lblPackName->getWidth()
				 + pad,(h - m_cells[i].lblSalePercent->getHeight()) / 2);

			m_cells[i].lblBonusAmount->setLocation(lbStartX,
				m_cells[i].lblPackName->getLocation().getY() +
				m_cells[i].lblPackName->getHeight());

			m_cells[i].lblPrice->setLocation(
				m_cells[i].btnBuy->getLocation().getX() -
				m_cells[i].lblPrice->getWidth() - pad,
				(h - m_cells[i].lblPrice->getHeight()) / 2);
		}
	}

	void PaymentDialog::clearCells()
	{
		for(int i = 0; i < m_cells.size(); i++)
		{
			PaymentCell cell = m_cells[i];

			m_container->remove(cell.container);
			cell.container->clear();
			cell.container->removeMouseListener(this);
			cell.btnBuy->removeMouseListener(this);
			cell.imgCoin->removeMouseListener(this);
			cell.lblBonusAmount->removeMouseListener(this);
			cell.lblPackName->removeMouseListener(this);
			cell.lblPrice->removeMouseListener(this);
			cell.lblSalePercent->removeMouseListener(this);
			cell.btnBuy->removeActionListener(this);

			getFactory()->deleteWidget(cell.btnBuy);
			getFactory()->deleteWidget(cell.imgCoin);
			getFactory()->deleteWidget(cell.lblBonusAmount);
			getFactory()->deleteWidget(cell.lblPackName);
			getFactory()->deleteWidget(cell.lblPrice);
			getFactory()->deleteWidget(cell.lblSalePercent);
			getFactory()->deleteWidget(cell.container);
		}
		m_cells.clear();
	}

	void PaymentDialog::actionPerformed( const agui::ActionEvent &evt )
	{
		GeneralDialogBox::actionPerformed(evt);

		for(int i = 0; i < m_cells.size();i++)
		{
			if(m_cells[i].btnBuy == evt.getSource())
			{
                if(m_lastClickTime == 0)
                {
                    m_lastClickTime = al_get_time();
                }
                else
                {
                    if(al_get_time() - m_lastClickTime < 3.0f)
                    {
                        return;
                    }
                    else
                    {
                        m_lastClickTime = al_get_time();
                    }
                }
#ifdef CGE_IPHONE
                IAPHandler::getInstance().buy(m_cellData[i].url);
#else
				DISPATCH_LOBBY_EVENT
					(*it)->sendStoreLink(m_cellData[i].url);
#endif
				break;
			}
		}
	}

	void PaymentDialog::dialogWillAppear()
	{
		clearCells();
		for(int i = 0; i < m_cellData.size(); i++)
		{
			addCell(m_cellData[i].packName,m_cellData[i].salePercent,m_cellData[i].bonusAmount,m_cellData[i].price);
		}
		forceResize();
	}

	void PaymentDialog::onParentSizeChanged( agui::Widget* source, const agui::Dimension &parentInnerSize )
	{
		GeneralDialogBox::onParentSizeChanged(source,parentInnerSize);
		repositionCells();
	}

	void PaymentDialog::receiveStoreLink( const std::string& link )
	{
#ifdef CGE_WINDOWS
		ShellExecuteA(NULL, "open", link.c_str(), NULL, NULL, SW_SHOWNORMAL);
#endif
#ifdef CGE_OSX
        OSXHandler::getInstance().launchURL(link);
#endif
	}

}