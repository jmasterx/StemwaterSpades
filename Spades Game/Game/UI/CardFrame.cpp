#include "Game/UI/CardFrame.hpp"
namespace cge
{
	CardFrame::~CardFrame(void)
	{
		removeFromFrame(m_topLayout);
		removeFromFrame(m_bottomLayout);
		m_okButton->removeActionListener(this);
		m_cancelButton->removeActionListener(this);
		m_exitButton->removeActionListener(this);
	}

	CardFrame::CardFrame( Sprite* shadow, agui::Image* cardImg,
		GraphicsContext* context, GuiFactory* factory )
		: m_shadowImg(shadow), m_cardImg(cardImg), m_context(context),
		m_resizeHeight(0),m_notifyWidget(NULL),
		m_bgOpacity(0.0),m_factory(factory)
	{
		setResizable(false);
		setMovable(false);
		setPaintingChildren(true);
		setBackColor(agui::Color(218,217,226));

		m_empty = m_factory->createEmptyWidget();

		m_topLayout = factory->createFlowLayout();
		m_topLayout->setResizeToParent(false);
		m_topLayout->setLeftToRight(false);
		m_topLayout->setSingleRow(true);

		m_bottomLayout = factory->createFlowLayout();
		m_bottomLayout->setResizeToParent(false);
		m_bottomLayout->setLeftToRight(false);
		m_bottomLayout->setSingleRow(true);

		m_exitButton = factory->createWhiteButton();
        if(Platf::isRetina())
        {
            m_exitButton->setFont(factory->getFontManager()->getFont(2.5f));
        }
		m_exitButton->setContentResizeScale(0.93f,1.1f);
		m_exitButton->setText("X");
		m_exitButton->resizeToContents();
		//m_topLayout->add(m_exitButton);
		m_topLayout->setSize(0,m_topLayout->getContentsHeight());
		
		m_okButton = factory->createWhiteButton();
        if(Platf::isRetina())
        {
            m_okButton->setFont(factory->getFontManager()->getFont(2.5f));
        }
		m_okButton->setText(factory->getLanguageManager()->getElement("ok"));
		m_okButton->resizeToContents();

		m_cancelButton = factory->createWhiteButton();
        if(Platf::isRetina())
        {
            m_cancelButton->setFont(factory->getFontManager()->getFont(2.5f));
        }
		m_cancelButton->setText(factory->getLanguageManager()->getElement("cancel"));
		m_cancelButton->resizeToContents();

		m_bottomLayout->add(m_cancelButton);
		m_bottomLayout->add(m_okButton);
		m_bottomLayout->setSize(0,m_bottomLayout->getContentsHeight());
		addToFrame(m_topLayout);
		addToFrame(m_bottomLayout);

		m_okButton->addActionListener(this);
		m_cancelButton->addActionListener(this);
		m_exitButton->addActionListener(this);

		hideDialog();

		setFontColor(agui::Color(1,44,100));
	}

	void CardFrame::paintBackground( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->clearClippingStack();

		paintEvent.graphics()->drawFilledRectangle(
			agui::Rectangle(-getLocation().getX(),
			-getLocation().getY(),getParent()->getWidth(),
			getParent()->getHeight()),
			agui::Color(0.0f,0.0f,0.0f,m_bgOpacity));

		int margT = getTopMargin() / 2;
		int margL = getLeftMargin() / 2;
		int margB = getBottomMargin() / 2;
		int margR = getRightMargin() / 2;

		agui::Rectangle r = agui::Rectangle(agui::Point(margL,margT),agui::Dimension(
			getWidth() - margL - margR, getHeight() - margT - margB));
		paintEvent.graphics()->drawFilledRectangle(r,getBackColor());

		paintPrivateChildren(paintEvent);
		clip(paintEvent);
		paintTop(paintEvent);
		paintPublicChildren(paintEvent);
		clip(paintEvent);

		int textLen = getFont()->getTextWidth(getText());
		int offsetX = (getWidth() - textLen) / 2;
		int offsetY = (getTopMargin() - getFont()->getLineHeight()) * 0.5f;
		offsetX -= getWidth() * 0.005f;
		paintEvent.graphics()->drawText(agui::Point(offsetX,offsetY),
			getText().c_str(),getFontColor(),getFont());
	}


	void CardFrame::paintTop( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawScaledImage(m_cardImg,agui::Point(),agui::Point(),
			agui::Dimension(m_cardImg->getWidth(), m_cardImg->getHeight()),getSize());
	}

	void CardFrame::paintComponent( const agui::PaintEvent &paintEvent )
	{

	}

	void CardFrame::setSizeFromHeight( int height )
	{
		float aspect = (float)m_cardImg->getWidth() / (float)m_cardImg->getHeight();
		int w = height * aspect;

		setSize(w,height);
	}

	void CardFrame::setSize( const agui::Dimension &size )
	{
		agui::Widget::setSize(size);
		float ratio = size.getWidth() / 350.0f;
		ratio *= 1.2f;
		setFont(m_factory->getFontManager()->getPrettyFont(ratio));
		resizeWidgetsInside();
	}

	void CardFrame::setSize( int width, int height )
	{
		agui::Frame::setSize(width,height);
	}

	void CardFrame::_recursivePaintPublicChildren( agui::Widget *root, bool enabled, agui::Graphics *graphicsContext )
	{
		//recursively calls itself to render widgets from back to front

		if(!root->isVisible())
		{
			return;
		}

		bool widgetEnabled = root->isEnabled();

		if(enabled == false)
		{
			widgetEnabled = false;
		}

		if(root != this)
		{
			root->clip(agui::PaintEvent(widgetEnabled,graphicsContext));

			root->paint(agui::PaintEvent(widgetEnabled,graphicsContext));

			if(root->isPaintingChildren())
			{
				return;
			}
		}

		if(root != this)
			for(agui::WidgetArray::iterator it = 
			root->getPrivateChildBegin();
			it != root->getPrivateChildEnd(); ++it)
		{
			_recursivePaintPublicChildren(*it,widgetEnabled,graphicsContext);
		}

			for(agui::WidgetArray::iterator it = 
			root->getChildBegin();
			it != root->getChildEnd(); ++it)
		{
			_recursivePaintPublicChildren(*it,widgetEnabled,graphicsContext);
		}

	}

	void CardFrame::_recursivePaintPrivateChildren( agui::Widget *root, bool enabled, agui::Graphics *graphicsContext )
	{
		//recursively calls itself to render widgets from back to front

		if(!root->isVisible())
		{
			return;
		}

		bool widgetEnabled = root->isEnabled();

		if(enabled == false)
		{
			widgetEnabled = false;
		}

		if(root != this)
		{
			root->clip(agui::PaintEvent(widgetEnabled,graphicsContext));

			root->paint(agui::PaintEvent(widgetEnabled,graphicsContext));

			if(root->isPaintingChildren())
			{
				return;
			}
		}

		for(agui::WidgetArray::iterator it = 
				root->getPrivateChildBegin();
				it != root->getPrivateChildEnd(); ++it)
			{
				_recursivePaintPrivateChildren(*it,widgetEnabled,graphicsContext);
			}

			if(root != this)
				for(agui::WidgetArray::iterator it = 
				root->getChildBegin();
				it != root->getChildEnd(); ++it)
			{
				_recursivePaintPrivateChildren(*it,widgetEnabled,graphicsContext);
			}
	}

	void CardFrame::paintPrivateChildren( const agui::PaintEvent &paintEvent )
	{
		m_stackOffset = paintEvent.graphics()->getOffset();
		m_stackRects = paintEvent.graphics()->getClippingStack();
		_recursivePaintPrivateChildren(this,isEnabled(),paintEvent.graphics());
		paintEvent.graphics()->setClippingStack(m_stackRects,m_stackOffset);
	}

	void CardFrame::paintPublicChildren( const agui::PaintEvent &paintEvent )
	{
		m_stackOffset = paintEvent.graphics()->getOffset();
		m_stackRects = paintEvent.graphics()->getClippingStack();
		_recursivePaintPublicChildren(this,isEnabled(),paintEvent.graphics());
		paintEvent.graphics()->setClippingStack(m_stackRects,m_stackOffset);
	}

	void CardFrame::parentSizeChanged()
	{
		agui::Widget* p = getParent();
		m_empty->setSize(p->getSize());

		int extraH = m_resizeHeight;

        if(!Platf::isMobile())
		setSizeFromHeight((p->getInnerHeight() - extraH) * 0.95f);
        else
        setSizeFromHeight((p->getInnerHeight() - extraH) * 1.07f);
		

        if(Platf::isRetina())
            extraH *= 0.6f;
        
		setLocation(createAlignedPosition(agui::ALIGN_MIDDLE_CENTER,
			agui::Rectangle(0,extraH,p->getWidth(),p->getHeight() - m_resizeHeight),getSize()));
	}

	void CardFrame::setResizeExtraHeight( int extraH )
	{
		m_resizeHeight = extraH;
		if(getParent())
		parentSizeChanged();
	}

	void CardFrame::setNotifyWidget( agui::ActionListener* listener )
	{
		m_notifyWidget  = listener;
	}

	void CardFrame::actionPerformed( const agui::ActionEvent& evt )
	{
		if((evt.getSource() == m_cancelButton  || evt.getSource() == m_exitButton)
			&& m_notifyWidget)
		{
			m_notifyWidget->actionPerformed(agui::ActionEvent(this,"cancel"));
		}
		else if(evt.getSource() == m_okButton && m_notifyWidget)
		{
			m_notifyWidget->actionPerformed(agui::ActionEvent(this,"ok"));
		}
	}

	void CardFrame::showDialog( bool modal /*= true*/ )
	{
		setVisibility(true);
		setGlobalOpacity(0.0f);
		if(modal)
		requestModalFocus();

		m_bgOpacity = 0.0;
		m_okButton->focus();

		if(getGui())
		{
			getGui()->remove(m_empty);
			getGui()->add(m_empty);
			m_empty->bringToFront();
			bringToFront();
		}
	}

	void CardFrame::hideDialog()
	{
		if(getGui())
		{
			getGui()->remove(m_empty);
		}

		setVisibility(false);
		releaseModalFocus();
	}

	void CardFrame::logic( double timeElapsed )
	{
		if(isVisible() && m_bgOpacity < 0.5f)
		{
			m_bgOpacity += 0.015f;
		}

		if(getGlobalOpacity() < 1.0f)
		{
			float o = getGlobalOpacity();
			o += 0.2;
			if(o > 1.0f)
				o = 1.0f;
			setGlobalOpacity(o);
		}
	}

	void CardFrame::setNames( 
		const std::string& okName, const std::string& cancelName )
	{
		if(okName != "")
		{
			m_okButton->setText(m_factory->getLanguageManager()->getElement(okName));
			m_okButton->resizeToContents();
		}

		if(cancelName != "")
		{
			m_cancelButton->setText(m_factory->getLanguageManager()->getElement(cancelName));
			m_cancelButton->resizeToContents();
		}

		resizeWidgetsInside(false);
	}

	void CardFrame::resizeWidgetsInside(bool remarginFrame)
	{
		float marScalY = 0.9f;
		float marScalX = 0.8f;
		float marT = 0.115 * getHeight() * marScalY;
		float marB = 0.105 * getHeight() * marScalY;
		float marL = 0.25 * getWidth() * marScalX;
        float marR = 0.22 * getWidth() * marScalX;
        
        if(Platf::isRetina())
        {
            marR = 0.21f * getWidth() * marScalX;
        }


		float padTB = 0.005 * getHeight();
		float padLR = 0.00 * getWidth();

		if(remarginFrame)
		setMargins(padTB,padLR,padTB,padLR);

		int topMargin = (marT - getMargin(agui::SIDE_TOP));
		int leftMargin = (marL - getMargin(agui::SIDE_LEFT));
		int bottomMargin = (marB - getMargin(agui::SIDE_BOTTOM));
		int rightMargin = (marR - getMargin(agui::SIDE_RIGHT));
		
		setFrameMargins(topMargin,leftMargin,bottomMargin,rightMargin);

		int topTopMargin = getHeight() * 0.009;
		int topRightMargin = getWidth() * 0.015;

		m_topLayout->setMargins(topTopMargin,topRightMargin,topTopMargin,topRightMargin);
		m_topLayout->updateLayout();
		m_topLayout->setSize(getInnerWidth(),m_topLayout->getContentsHeight());
		m_topLayout->setLocation(0,0);

		int botTBMarg = (marB - (m_bottomLayout->getContentsHeight() -
			m_bottomLayout->getMargin(agui::SIDE_TOP) - 
			m_bottomLayout->getMargin(agui::SIDE_BOTTOM))) / 2;

		m_bottomLayout->setMargins(botTBMarg - (getHeight() * 0.005),0,botTBMarg,marR - (getWidth() * 0.05));
		m_bottomLayout->updateLayout();
		m_bottomLayout->setSize(getInnerWidth(),marB);
		m_bottomLayout->setLocation(0,getInnerHeight() - m_bottomLayout->getHeight());

	}

}
