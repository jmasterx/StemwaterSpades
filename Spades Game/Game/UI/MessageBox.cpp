#include "Game/UI/MessageBox.hpp"
#include "Game/Handler/MessageBoxResultReceiver.hpp"
namespace cge
{
	MessageBox::MessageBox(FlowLayout* buttonLayout, FlowLayout* containLayout, Button* btn1, 
		Button* btn2, Button* btn3, Button* btn4, Label* textLabel, Label* titleLabel, GuiFontManager* font,
		LanguageManager* language, agui::Image* bolt, agui::Image* bg )
		: m_buttonLayout(buttonLayout),m_btnYes(btn1),m_btnNo(btn2), m_btnOk(btn3),m_btnCancel(btn4),
		m_textLabel(textLabel),m_titleLabel(titleLabel),m_font(font),m_language(language),
		m_fontRatio(1.1f),m_bolt(bolt),m_bg(bg),m_container(NULL),
		m_lrpadding(20),m_tbSpacing(15),m_minMsgHeight(20),m_minMsgBoxWidth(200),
		m_extraTitlePad(8),m_type(MT_OK_CANCEL),m_receiver(NULL),m_tag(-1),
		m_containLayout(containLayout)
	{
        
        if(Platf::isRetina()) {
            m_fontRatio *= 2.1f;
            m_tbSpacing *= 2.1f;
            m_minMsgBoxWidth = 300;
            m_minMsgHeight *= 2.0f;
            m_extraTitlePad *= 2.0f;
            m_buttonLayout->setHorizontalSpacing(30);
        }
        
        
		setMargins(3,12,5,7);
		m_buttonLayout->setSingleRow(true);
		m_buttonLayout->setHorizontallyCentered(true);
		m_buttonLayout->add(m_btnYes);
		m_buttonLayout->add(m_btnNo);
		m_buttonLayout->add(m_btnCancel);
		m_buttonLayout->add(m_btnOk);
		m_titleLabel->setAlignment(agui::ALIGN_MIDDLE_CENTER);
		m_titleLabel->setSingleLine(true,true);
		m_titleLabel->setAutosizing(false);
		m_textLabel->setAutosizing(false);
		m_textLabel->setSingleLine(false);

		m_titleLabel->setFontColor(agui::Color(1,44,100));

		m_textLabel->setFontColor(agui::Color(1,48,96));

		setFontRatio(m_fontRatio);

		m_btnYes->setText(language->getElement("yes"));
		m_btnNo->setText(language->getElement("no"));
		m_btnCancel->setText(language->getElement("cancel"));
		m_btnOk->setText(language->getElement("ok"));

		m_btnYes->addActionListener(this);
		m_btnNo->addActionListener(this);
		m_btnCancel->addActionListener(this);
		m_btnOk->addActionListener(this);
	}

	MessageBox::~MessageBox(void)
	{
	}

	void MessageBox::setFontRatio( float ratio )
	{
		m_fontRatio = ratio;
		m_titleLabel->setFont(m_font->getPrettyFont(1.2f * m_fontRatio));
		m_textLabel->setFont(m_font->getFont(0.75f * m_fontRatio));

		m_btnYes->setFont(m_font->getFont(1.05f * m_fontRatio));
		m_btnNo->setFont(m_font->getFont(1.05f * m_fontRatio));
		m_btnCancel->setFont(m_font->getFont(1.05f * m_fontRatio));
		m_btnOk->setFont(m_font->getFont(1.05f * m_fontRatio));
	}

	void MessageBox::resize( Frame* frame )
	{
		if(!frame || !frame->getTopWidget())
		{
			return;
		}

		int clientWidth = frame->getTopWidget()->getWidth();
		int clientHeight = frame->getTopWidget()->getHeight();
		int msgWidth = 0;
		
		m_textLabel->resizeToWidth(clientWidth);

		int textX = m_textLabel->getLongestLineWidth() + (2 * m_lrpadding);
		int textY = getHeightFromWidth(textX);

		int text38X = clientWidth * (3.0f / 8.0f);
		int text38Y = getHeightFromWidth(text38X);

		int text48X = clientWidth * (4.0f / 8.0f);
		int text48Y = getHeightFromWidth(text48X);

		int text58X = clientWidth * (5.0f / 8.0f);
		int text58Y = getHeightFromWidth(text58X);

		int text78X = clientWidth * (7.0f / 8.0f);
		int text78Y = getHeightFromWidth(text78X);

		int h =  0;
		
		if(textX < text38X && textY < clientHeight)
		{
			h = textY;
			msgWidth = textX;
		}
		else if(text38Y < clientHeight)
		{
			h = text38Y;
			msgWidth = text38X;
		}
		else if(text48Y < clientHeight)
		{
			h = text48Y;
			msgWidth = text48X;
		}
		else if(text58Y < clientHeight)
		{
			h = text58Y;
			msgWidth = text58X;
		}
		else
		{
			h = text78Y;
			msgWidth = text78X;
		}

		if(msgWidth < m_minMsgBoxWidth)
		{
			msgWidth = m_minMsgBoxWidth;
		}

		if(msgWidth < m_titleLabel->getLongestLineWidth() + (2 * m_lrpadding) + m_extraTitlePad)
		{
			msgWidth = m_titleLabel->getLongestLineWidth() + (2 * m_lrpadding) + m_extraTitlePad;
		}

		h = getHeightFromWidth(msgWidth);
		int msgMarginW = msgWidth + getMargin(agui::SIDE_LEFT) + getMargin(agui::SIDE_RIGHT);
		if(frame)
		{
			frame->setClientSize(msgMarginW,h);
		}
	}

	void MessageBox::showMessage( const std::string& title, const std::string& message, Frame* frame, MessageType type, MessageBoxResultReceiver* rec, int tag )
	{
		m_titleLabel->setText(title);
		m_textLabel->setText(message);
		m_receiver = rec;
		m_type = type;
		m_tag = tag;
		resize(frame);

		if(m_type == MT_YES_NO)
		{
			m_btnYes->focus();
		}
		else if(m_type == MT_YES_NO_CANCEL)
		{
			m_btnYes->focus();
		}
		else if(m_type == MT_OK_CANCEL)
		{
			m_btnOk->focus();
		}
		else if(m_type == MT_OK)
		{
			m_btnOk->focus();
		}

	}

	void MessageBox::paintBackground( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->clearClippingStack();

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

	void MessageBox::setContainer( Widget* container )
	{
		m_container = container;
	}

	void MessageBox::hideMessage()
	{
		if(m_container)
		{
			m_container->setVisibility(false);
			m_container->releaseModalFocus();
			getModalContainer()->setVisibility(false);
		}
	}

	void MessageBox::actionPerformed( const agui::ActionEvent& evt )
	{
		if(m_receiver)
		{
			MessageBoxResultReceiver* r = m_receiver;
			m_receiver = NULL;
			if(evt.getSource() == m_btnOk)
			{
				hideMessage();
				r->messageBoxResult(this,MR_OK);
				r->setMessageResultTag(-1);
			}
			else if(evt.getSource() == m_btnCancel)
			{
				hideMessage();
				r->messageBoxResult(this,MR_CANCEL);
				r->setMessageResultTag(-1);
			}
			else if(evt.getSource() == m_btnYes)
			{
				hideMessage();
				r->messageBoxResult(this,MR_YES);
				r->setMessageResultTag(-1);
			}
			else if(evt.getSource() == m_btnNo)
			{
				hideMessage();
				r->messageBoxResult(this,MR_NO);
				r->setMessageResultTag(-1);
			}
		}
		else
		{
			hideMessage();
		}
	}

	int MessageBox::getHeightFromWidth( int width )
	{
		int h = 0;

		clear();

		add(m_titleLabel);
		add(m_textLabel);
		add(m_buttonLayout);

		m_titleLabel->setMargins(0,0,0,0);
		m_textLabel->setMargins(0,0,0,0);
		m_titleLabel->resizeToWidth(width - (2 * m_lrpadding) - m_extraTitlePad);

		if(m_type == MT_YES_NO)
		{
			m_btnYes->setVisibility(true);
			m_btnYes->resizeToContents();
			m_btnNo->setVisibility(true);
			m_btnNo->resizeToContents();
			m_btnCancel->setVisibility(false);
			m_btnOk->setVisibility(false);
		}
		else if(m_type == MT_YES_NO_CANCEL)
		{
			m_btnYes->setVisibility(true);
			m_btnYes->resizeToContents();
			m_btnNo->setVisibility(true);
			m_btnNo->resizeToContents();
			m_btnCancel->setVisibility(true);
			m_btnCancel->resizeToContents();
			m_btnOk->setVisibility(false);
		}
		else if(m_type == MT_OK_CANCEL)
		{
			m_btnYes->setVisibility(false);
			m_btnNo->setVisibility(false);
			m_btnCancel->setVisibility(true);
			m_btnCancel->resizeToContents();
			m_btnOk->setVisibility(true);
			m_btnOk->resizeToContents();
		}
		else if(m_type == MT_OK)
		{
			m_btnYes->setVisibility(false);
			m_btnNo->setVisibility(false);
			m_btnCancel->setVisibility(false);
			m_btnOk->setVisibility(true);
			m_btnOk->resizeToContents();
		}

		m_buttonLayout->setMargins(0,0,0,0);
		m_buttonLayout->resizeToContents();
		m_buttonLayout->setSize(width,m_buttonLayout->getHeight());

		m_textLabel->setMargins(0,0,0,0);
		m_textLabel->resizeToWidth(width - (2 * m_lrpadding));

		h += m_tbSpacing;
		m_titleLabel->setLocation(m_lrpadding + (m_extraTitlePad / 2),h);
		h += m_titleLabel->getHeight();
		h += m_tbSpacing;
		m_textLabel->setLocation(m_lrpadding,h);
		if(m_textLabel->getHeight() < m_minMsgHeight)
		{
			h += m_minMsgHeight;
		}
		else
		{
			h += m_textLabel->getHeight();
		}

		h += m_tbSpacing;
		m_buttonLayout->setLocation(0,h);
		h += m_buttonLayout->getHeight();
		h += m_tbSpacing;
		return h;

	}

	int MessageBox::getTag() const
	{
		return m_tag;
	}

	agui::FlowLayout* MessageBox::getModalContainer()
	{
		return m_containLayout;
	}

}
