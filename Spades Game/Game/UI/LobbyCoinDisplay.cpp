#include "Game/UI/LobbyCoinDisplay.hpp"
#include "Game/Utility/StringUtil.hpp"
namespace cge
{
	LobbyCoinDisplay::~LobbyCoinDisplay(void)
	{
	}

	LobbyCoinDisplay::LobbyCoinDisplay( GuiFactory* factory )
		: m_amount(NULL),m_container(NULL),m_moneyIcon(NULL),m_factory(factory)
	{
		m_container = factory->createFlowLayout();
		m_amount = factory->createLabel();
		m_moneyIcon = factory->createImageIcon("coin");
		m_moneyIcon->setScaleToFit(true);
        if(Platf::isRetina())
        m_moneyIcon->setSize(25 * 2.75f,1 + 28 * 2.6f);
        else if (Platf::isMini())
        m_moneyIcon->setSize(25 * 1.02f,1 + 28 * 1.01f);
        else
        m_moneyIcon->setSize(26,26);
		m_amount->setText("");
		m_amount->setFontColor(agui::Color(255,255,255));
        if(Platf::isRetina())
        m_amount->setFont(m_factory->getFontManager()->getFont(2.3f));
        
		m_container->setVerticalSpacing(0);
		m_container->setSingleRow(true);
		m_container->setResizeToParent(false);
        
        if(Platf::isRetina())
        m_moneyIcon->setMargins(4 + 7,5,1,1);
        else if(Platf::isMini())
        m_moneyIcon->setMargins(9,5,1,1);
        else
        m_moneyIcon->setMargins(5,5,1,1);
       
        if(Platf::isRetina())
        m_amount->setMargins(((2.8f * 30) - m_amount->getFont()->getLineHeight()) / 2,1,1,1);
        else if(Platf::isMini())
        m_amount->setMargins(((1.2f * 30) - m_amount->getFont()->getLineHeight()) / 2,1,1,1);
        else
        m_amount->setMargins((30 - m_amount->getFont()->getLineHeight()) / 2,1,1,1);
        
		m_container->add(m_moneyIcon);
		m_container->add(m_amount);

		m_container->setHorizontalSpacing(3);

		m_container->resizeToContents();
	}

	agui::FlowLayout* LobbyCoinDisplay::getWidget()
	{
		return m_container;
	}

	void LobbyCoinDisplay::coinAmountChanged( int64 amount, bool update, bool fromPurchase, bool spending )
	{
		m_amount->setText("$" + StringUtil::commaSeparate(StringUtil::toString(amount)));
		m_container->resizeToContents();
	}

	void LobbyCoinDisplay::setColor( const agui::Color& color )
	{
		m_amount->setFontColor(color);
	}

	void LobbyCoinDisplay::setCoinSize( int w )
	{
		m_moneyIcon->setMargins(0,0,0,0);
		m_moneyIcon->setSize(w,w);
		reMargin();
	}

	void LobbyCoinDisplay::setFontSize( agui::Font* f )
	{
		m_amount->setFont(f);
		reMargin();
	}

	void LobbyCoinDisplay::reMargin()
	{
		m_amount->setMargins(1,1,1,1);
		m_moneyIcon->setMargins((m_amount->getHeight() - m_moneyIcon->getInnerHeight()) / 2,
			(m_amount->getHeight() - m_moneyIcon->getInnerHeight()) / 2
			,1,1);

		m_container->resizeToContents();
	}

}