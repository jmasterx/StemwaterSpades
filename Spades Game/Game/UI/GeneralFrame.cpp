#include "Game/UI/GeneralFrame.hpp"
namespace cge
{
	GeneralFrame::~GeneralFrame(void)
	{
		removeFromFrame(m_topLayout);
		removeFromFrame(m_bottomLayout);
		m_okButton->removeActionListener(this);
		m_cancelButton->removeActionListener(this);
		m_exitButton->removeActionListener(this);
		m_top->removeWidgetListener(this);
	}

	GeneralFrame::GeneralFrame( Sprite* shadow, agui::Image* bg, agui::Image* bolt, agui::Image* frameFill,
		GraphicsContext* context, GuiFactory* factory )
		: m_shadowImg(shadow), m_bg(bg), m_context(context),
		m_resizeHeight(0),m_notifyWidget(NULL),
		m_bgOpacity(0.0),m_factory(factory),m_bolt(bolt),m_frameFill(frameFill),
		m_tabbingEnabled(true),m_widthScalar(0.7f),m_heightScalar(0.7f),m_sideLabel(NULL)
	{
		setResizable(false);
		setMovable(false);
		setBackColor(agui::Color(218,217,226));
		setPaintingChildren(true);

		m_topLayout = factory->createFlowLayout();
		m_topLayout->setResizeToParent(false);
		m_topLayout->setLeftToRight(false);
		m_topLayout->setSingleRow(true);

		m_bottomLayout = factory->createFlowLayout();
		m_bottomLayout->setResizeToParent(false);
		m_bottomLayout->setLeftToRight(false);
		m_bottomLayout->setSingleRow(true);

		m_exitButton = factory->createWhiteButton();
		m_exitButton->setContentResizeScale(0.93f,1.1f);
		m_exitButton->setText("X");
		m_exitButton->resizeToContents();
	//	m_topLayout->add(m_exitButton);
		m_topLayout->setSize(0,m_topLayout->getContentsHeight());

		m_sideLabel = factory->createLabel();
        
		m_okButton = factory->createWhiteButton();
		m_okButton->setText(factory->getLanguageManager()->getElement("ok"));
        if(Platf::isRetina())
        {
            m_okButton->setFont(factory->getFontManager()->getFont(2.5f));
        }
		m_okButton->resizeToContents();

		m_cancelButton = factory->createWhiteButton();
		m_cancelButton->setText(factory->getLanguageManager()->getElement("cancel"));
        if(Platf::isRetina())
        {
            m_cancelButton->setFont(factory->getFontManager()->getFont(2.5f));
        }
		m_cancelButton->resizeToContents();

        if(Platf::isRetina())
            m_sideLabel->setFont(m_factory->getFontManager()->getFont(2.0f));
		m_sideLabel->setMargins(1,1,1,6);
		m_sideLabel->setAlignment(agui::ALIGN_MIDDLE_RIGHT);
		m_sideLabel->setAutosizing(false);
		//m_cancelButton->setFocusable(false);
		m_bottomLayout->add(m_cancelButton);
		m_bottomLayout->add(m_okButton);
		m_bottomLayout->add(m_sideLabel);
		m_bottomLayout->setSize(0,m_bottomLayout->getContentsHeight());
		addToFrame(m_topLayout);
		addToFrame(m_bottomLayout);

		m_okButton->addActionListener(this);
		m_cancelButton->addActionListener(this);
		//m_exitButton->addActionListener(this);
		m_exitButton->setVisibility(false);

		m_maskingFlow = m_factory->createFlowLayout();
		m_maskingFlow->setVisibility(false);
		setFontColor(agui::Color(1,44,100));
	}

	void GeneralFrame::paintBackground( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->clearClippingStack();

		/*
		paintEvent.graphics()->drawFilledRectangle(
			agui::Rectangle(-getLocation().getX(),
			-getLocation().getY(),getParent()->getWidth(),
			getParent()->getHeight()),
			agui::Color(0.0f,0.0f,0.0f,m_bgOpacity));
			*/

		int margT = getTopMargin() / 2;
		int margL = getLeftMargin() / 2;
		int margB = getBottomMargin() / 2;
		int margR = getRightMargin() / 2;


		agui::Rectangle r = agui::Rectangle(agui::Point(margL,margT),agui::Dimension(
			getWidth() - margL - margR, getHeight() - margT - margB));
		paintEvent.graphics()->drawFilledRectangle(r,getBackColor());

		
		clip(paintEvent);
		paintTop(paintEvent);
		paintEvent.graphics()->drawFilledRectangle(
			agui::Rectangle(getLeftMargin() + getMargin(agui::SIDE_LEFT),
			getTopMargin() + getMargin(agui::SIDE_TOP),getInnerWidth() - getLeftMargin() - getRightMargin(),
			getInnerHeight() - getTopMargin() - getBottomMargin()),
			agui::Color(0.0f,0.0f,0.0f,0.1f));
		paintPrivateChildren(paintEvent);
		int offX = 13;
		int offY = 10;
		paintEvent.graphics()->drawNinePatchImage(m_frameFill,agui::Point(getLeftMargin() + getMargin(agui::SIDE_LEFT) - offX,
			getTopMargin() + getMargin(agui::SIDE_TOP) - offY),agui::Dimension(offX + 10 + getInnerWidth() - getLeftMargin() - getRightMargin(),
			getInnerHeight() - getTopMargin() - getBottomMargin() + offY + 12));

		paintPublicChildren(paintEvent);
		clip(paintEvent);

		int textLen = getFont()->getTextWidth(getText());
		int offsetX = (getWidth() - textLen) / 2;
		int offsetY = (getTopMargin() - getFont()->getLineHeight()) * 0.5f;
		offsetX -= getWidth() * 0.005f;
		paintEvent.graphics()->drawText(agui::Point(offsetX,offsetY),
			getText().c_str(),getFontColor(),getFont());
	}
	void GeneralFrame::paintComponent( const agui::PaintEvent &paintEvent )
	{

	}

	void GeneralFrame::paintTop( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawNinePatchImage(m_bg,agui::Point(),getSize());
		int sz = 17;
		int borderThick = -6;
		paintEvent.graphics()->drawScaledImage(m_bolt,agui::Point(-2 - borderThick,-borderThick - 2),agui::Point(),agui::Dimension(
			m_bolt->getWidth(),m_bolt->getHeight()),agui::Dimension(sz,sz));


		paintEvent.graphics()->drawScaledImage(m_bolt,agui::Point(getWidth() - sz + 4 + borderThick,-borderThick - 2),agui::Point(),agui::Dimension(
			m_bolt->getWidth(),m_bolt->getHeight()),agui::Dimension(sz,sz));

		paintEvent.graphics()->drawScaledImage(m_bolt,agui::Point(-2 - borderThick,getHeight() - sz + 2 + borderThick),agui::Point(),agui::Dimension(
			m_bolt->getWidth(),m_bolt->getHeight()),agui::Dimension(sz,sz));

		paintEvent.graphics()->drawScaledImage(m_bolt,agui::Point(getWidth() - sz + 4 + borderThick,getHeight() - sz + 2 + borderThick),agui::Point(),agui::Dimension(
			m_bolt->getWidth(),m_bolt->getHeight()),agui::Dimension(sz,sz));
	}


	void GeneralFrame::setSizeFromHeight( int height )
	{
		float aspect = (float)m_bg->getWidth() / (float)m_bg->getHeight();
		int w = height * aspect;

		setSize(w,height);
	}

	void GeneralFrame::setSize( const agui::Dimension &size )
	{
		agui::Widget::setSize(size);
		float ratio = size.getWidth() / 350.0f;
		setFont(m_factory->getFontManager()->getPrettyFont(ratio));
		resizeWidgetsInside();
	}

	void GeneralFrame::setSize( int width, int height )
	{
		agui::Frame::setSize(width,height);
	}

	void GeneralFrame::_recursivePaintPublicChildren( agui::Widget *root, bool enabled, agui::Graphics *graphicsContext )
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

	void GeneralFrame::_recursivePaintPrivateChildren( agui::Widget *root, bool enabled, agui::Graphics *graphicsContext )
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

	void GeneralFrame::paintPrivateChildren( const agui::PaintEvent &paintEvent )
	{
		m_stackOffset = paintEvent.graphics()->getOffset();
		m_stackRects = paintEvent.graphics()->getClippingStack();
		_recursivePaintPrivateChildren(this,isEnabled(),paintEvent.graphics());
		paintEvent.graphics()->setClippingStack(m_stackRects,m_stackOffset);
	}

	void GeneralFrame::paintPublicChildren( const agui::PaintEvent &paintEvent )
	{
		m_stackOffset = paintEvent.graphics()->getOffset();
		m_stackRects = paintEvent.graphics()->getClippingStack();
		_recursivePaintPublicChildren(this,isEnabled(),paintEvent.graphics());
		paintEvent.graphics()->setClippingStack(m_stackRects,m_stackOffset);
	}

	void GeneralFrame::parentSizeChanged()
	{
		agui::Widget* p = getParent();

		int extraH = m_resizeHeight;

		setSizeFromHeight((p->getInnerHeight() - extraH) * 0.9);

		setLocation(createAlignedPosition(agui::ALIGN_MIDDLE_CENTER,
			agui::Rectangle(0,extraH,p->getWidth(),p->getHeight() - m_resizeHeight),getSize()));
	}

	void GeneralFrame::setResizeExtraHeight( int extraH )
	{
		m_resizeHeight = extraH;
		if(getParent())
			parentSizeChanged();
	}

	void GeneralFrame::setNotifyWidget( agui::ActionListener* listener )
	{
		m_notifyWidget  = listener;
	}

	void GeneralFrame::actionPerformed( const agui::ActionEvent& evt )
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

	void GeneralFrame::showDialog( bool modal /*= true*/ )
	{
		m_maskingFlow->setVisibility(true);
		m_frame->setVisibility(true);
		m_maskingFlow->bringToFront();
		m_frame->bringToFront();
		m_frame->setGlobalOpacity(0.0f);
		if(modal)
			m_frame->requestModalFocus();

		m_bgOpacity = 0.0;

		if(m_frame->getGui())
		{
			m_tabbingEnabled = m_frame->getGui()->isTabbingEnabled();
			m_frame->getGui()->setTabbingEnabled(true);
		}

		m_okButton->focus();
	}

	void GeneralFrame::hideDialog()
	{
		m_frame->setVisibility(false);
		m_maskingFlow->setVisibility(false);
		m_frame->releaseModalFocus();
		if(m_frame->getGui())
		{
			m_frame->getGui()->setTabbingEnabled(m_tabbingEnabled);
		}
	}

	void GeneralFrame::logic( double timeElapsed )
	{
		if(isVisible() && m_bgOpacity < 0.5f)
		{
			m_bgOpacity += 0.015f;
		}

		if(m_frame->getGlobalOpacity() < 1.0f && isVisible())
		{
			float o = m_frame->getGlobalOpacity();
			o += 0.07;
			if(o > 1.0f)
				o = 1.0f;
			m_frame->setGlobalOpacity(o);
		}
	}

	void GeneralFrame::setNames( 
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

	void GeneralFrame::resizeWidgetsInside(bool remarginFrame)
	{
		float marT = 0.115 * getHeight();
		float marB = 65;
        if(Platf::isRetina())
            marB *= 2.0f;
        
		float marL = 0.06 * getWidth();
		float marR = 0.06 * getWidth();


		float padTB = 0.005 * getHeight();
		float padLR = 0.00 * getWidth();

		if(remarginFrame)
			setMargins(padTB,padLR,padTB,padLR);

		int topMargin = (marT - getMargin(agui::SIDE_TOP));
		int leftMargin = (marL - getMargin(agui::SIDE_LEFT));
		int bottomMargin = (marB - getMargin(agui::SIDE_BOTTOM));
		int rightMargin = (marR - getMargin(agui::SIDE_RIGHT));

		setFrameMargins(topMargin,leftMargin,bottomMargin,rightMargin);

		int topTopMargin = 20;
		int topRightMargin = 25;

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

	agui::Widget* GeneralFrame::getWidget()
	{
		return m_frame;
	}

	void GeneralFrame::setFrame( Frame* frame )
	{
		m_frame = frame;
		m_frame->setMovable(false);
		m_frame->setResizable(false);

		hideDialog();
	}

	void GeneralFrame::setTop( agui::Widget* top )
	{
		m_top = top;
		m_top->addWidgetListener(this);
		sizeChanged(m_top,m_top->getSize());
	}

	void GeneralFrame::sizeChanged( agui::Widget* w, const agui::Dimension & size )
	{
		m_frame->setClientSize(size.getWidth() * m_widthScalar,size.getHeight() * m_heightScalar);
		m_frame->alignToParent(agui::ALIGN_MIDDLE_CENTER);
		setSize(getSize());
	}

	FlowLayout* GeneralFrame::getMaskingFlow()
	{
		return m_maskingFlow;
	}

	void GeneralFrame::setCloseButtonVisibility( bool show )
	{
		m_cancelButton->setVisibility(show);
	}

	void GeneralFrame::setSizeConstants( float width, float height )
	{
		m_widthScalar = width;
		m_heightScalar = height;

		if(getTopWidget())
			sizeChanged(getTopWidget(),getTopWidget()->getSize());
	}

	void GeneralFrame::setOkButtonVisibility( bool show )
	{
		m_okButton->setVisibility(show);
	}

	void GeneralFrame::setSideLabelText( const std::string& text )
	{
		m_sideLabel->setText(text);
		m_sideLabel->resizeToContents();
		m_sideLabel->setSize(m_sideLabel->getWidth(),m_okButton->getHeight());
	}

	void GeneralFrame::addWidgetsToBottom( const std::vector<agui::Widget*>& widgets )
	{
		for(int i = 0; i < widgets.size(); i++)
		{
			m_bottomLayout->add(widgets[i]);
		}
	}

	void GeneralFrame::removeWidgetsFromBottom( const std::vector<agui::Widget*>& widgets )
	{
		for(int i = 0; i < widgets.size(); i++)
		{
			m_bottomLayout->remove(widgets[i]);
		}
	}

}
