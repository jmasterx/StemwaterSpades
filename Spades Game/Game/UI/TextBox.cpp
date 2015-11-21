#include "Game/UI/TextBox.hpp"
#include <sstream>
#include <iostream>
#include <Game/platform/Platf.hpp>
namespace cge
{
	TextBox::~TextBox(void)
	{
	}

	TextBox::TextBox( 
		agui::VScrollBar* vscroll, agui::HScrollBar* hscroll,
		agui::Widget* scrollInset, agui::Image* bgImage, agui::Image* focusImage,
		PopUpMenu* menu,LanguageManager* lang )
		: agui::ExtendedTextBox(hscroll,vscroll,scrollInset), m_asLabel(false),
		m_vscroll(vscroll), m_hscroll(hscroll), m_scrollInset(scrollInset),
		m_bgImage(bgImage), m_focusImage(focusImage), m_menu(menu),
		m_cutItem("Cut", "Ctrl + X"), m_copyItem("Copy","Ctrl + C"), 
		m_pasteItem("Paste","Ctrl + V"), m_selectAllItem("Select All","Ctrl + A"),
		m_deleteItem("Delete"),
		m_separatorItem(agui::PopUpMenuItem::SEPARATOR),
		m_keepingTableNumbers(false),m_lastTableNum(-1),m_cursorObject(CURSOR_ON_NOTHING),
		m_keepingNames(false),	lastMouseX(-1),lastMouseY(-1),
		m_isTypingFont(NULL),m_isTypingImg(NULL),
		m_isTypingTimeout(60),m_isTypingColor(agui::Color(0,0,0)), m_isTypingCounter(0),
		m_pencilAnimCounter(0),m_pencilX(0),m_pencilY(-7),m_elipsisCounter(0)

	{
		if(lang)
		{
			m_cutItem.setText(lang->getElement("popup.cut"));
			m_copyItem.setText(lang->getElement("popup.copy"));
			m_pasteItem.setText(lang->getElement("popup.paste"));
			m_selectAllItem.setText(lang->getElement("popup.select.all"));
			m_deleteItem.setText(lang->getElement("popup.delete"));
		}

		m_vscroll->setTopArrowAmount(10);
		m_vscroll->setBottomArrowAmount(10);
#ifdef CGE_MOBILE

		m_vscroll->setStickToBottom();
#endif
		if(menu)
		{
			menu->addItem(&m_cutItem);
			menu->addItem(&m_copyItem);
			menu->addItem(&m_pasteItem);
			menu->addItem(&m_deleteItem);
			menu->addItem(&m_separatorItem);
			menu->addItem(&m_selectAllItem);
			menu->addActionListener(this);
		}
	}

	void TextBox::paintBackground( const agui::PaintEvent &paintEvent )
	{
		if(m_asLabel)
		{
			return;
		}

		agui::Image* img = NULL;
		if(isFocused())
		{
			img = m_focusImage;
		}
		else
		{
			img = m_bgImage;
		}

		paintEvent.graphics()->drawNinePatchImage(img,agui::Point(0,0),getSize());
	}

	void TextBox::paintComponent( const agui::PaintEvent &paintEvent )
	{
		int textX = getHorizontalOffset();
		int textY = getVerticalOffset();
		int voffset = 0;
		int hoffset = 0;
		if(isHScrollVisible())
		{
			hoffset = getHSrollSize().getHeight();
		}
		if(isVScrollVisible())
		{

			voffset = getVScrollSize().getWidth();
		}


		paintEvent.graphics()->pushClippingRect(agui::Rectangle(getLeftPadding(),
			getTopPadding(), getAdjustedWidth() - voffset + 1, 
			getAdjustedHeight() - hoffset));

		//only show selection if it's not hidden
		if(!isHidingSelection() || (isHidingSelection() && isFocused()))
			for(int i = 0; i < getSelLineCount(); ++i)
			{
				paintEvent.graphics()->drawFilledRectangle(agui::Rectangle(
					getSelLineAt(i).first.getX() + textX,
					getSelLineAt(i).first.getY() + textY,
					getSelLineAt(i).second.getX(),
					getSelLineAt(i).second.getY()),getSelectionBackColor());
			}

			drawText(paintEvent);

			if(isFocused() && isBlinking() && !m_asLabel)
				paintEvent.graphics()->drawLine(agui::Point(
				getCaretColumnLocation() + 1 , getCaretRowLocation()),
				agui::Point(getCaretColumnLocation() + 1, getCaretRowLocation() + getLineHeight()),
				agui::Color(0,0,0));

			paintEvent.graphics()->popClippingRect();

			if(m_isTyping)
			{
				int initialOffsetX = 0;
				int initialOffsetY = 0;
				int pencilWidth = 12;
                if(Platf::isRetina())
                    pencilWidth *= 2.0f;
				int leftGap = pencilWidth + 20 + initialOffsetX;

				paintEvent.graphics()->drawText(
					agui::Point(getLeftPadding() + leftGap,getInnerHeight() - getIsTypingFont()->getLineHeight() - initialOffsetY - 2),
					m_fullPencil.c_str(),getIsTypingColor(),getIsTypingFont());


				paintEvent.graphics()->drawScaledImage(m_isTypingImg,
					agui::Point(initialOffsetX + m_pencilX,getInnerHeight() - pencilWidth + m_pencilY - initialOffsetY - 2),
					agui::Point(0,0),agui::Dimension(m_isTypingImg->getWidth(),
					m_isTypingImg->getHeight()),agui::Dimension(pencilWidth,pencilWidth));
			}
		
	}

	bool TextBox::intersectionWithPoint( const agui::Point &p ) const
	{
		int padding = 0;
		return agui::Rectangle(getMargin(agui::SIDE_LEFT) - padding,
			getMargin(agui::SIDE_TOP) -
			padding,getInnerWidth() + (padding * 2),getInnerHeight() + (padding * 2)).pointInside(p);
	}

	void TextBox::actionPerformed( const agui::ActionEvent &evt )
	{
		if(evt.getSource() == &m_selectAllItem)
		{
			selectAll();
			focus();
		}
		else if(evt.getSource() == &m_copyItem)
		{
			copy();
			focus();
		}
		else if(evt.getSource() == &m_pasteItem)
		{
			if(!isReadOnly())
			{
				paste();
				focus();
			}
		}
		else if(evt.getSource() == &m_cutItem)
		{
			cut();
			focus();
		}
		else if(evt.getSource() == &m_deleteItem)
		{
			if(!isReadOnly())
			{
				deleteSelection();
				focus();
			}
		}
		
	}

	void TextBox::mouseClick( agui::MouseEvent &mouseEvent )
	{
		if(mouseEvent.getButton() == agui::MOUSE_BUTTON_LEFT)
		{
			//relative mouse position
			agui::Point p = agui::Point(mouseEvent.getX(),
				mouseEvent.getY());

			agui::Point columnRow = columnRowFromRelPosition(p);
			int	mouseDownIndex = indexFromColumnRow(columnRow.getX(),columnRow.getY());

			findLine(mouseDownIndex,mouseEvent.getY());

			if(m_cursorObject != CURSOR_ON_NOTHING)
			{
				if(m_cursorObject == CURSOR_ON_TABLE)
				{
					generateAction(TC_GO_TO_TABLE);
				}
				else if(m_cursorObject == CURSOR_ON_NAME)
				{
                    if(!Platf::isMobile())
					generateAction(TC_VIEW_USER_PROFILE);
                    else
                    generateAction(TC_SHOW_USER_CONTEXT);
				}

				return;
			}
		}

		agui::ExtendedTextBox::mouseClick(mouseEvent);
	
		if(mouseEvent.getButton() == agui::MOUSE_BUTTON_RIGHT && m_cursorObject == CURSOR_ON_NOTHING)
		{
			if(m_menu)
			{
				m_pasteItem.setEnabled(!isReadOnly());
				m_cutItem.setEnabled(!isReadOnly() && !isSelectionEmpty());
				m_copyItem.setEnabled(!isSelectionEmpty());
				m_deleteItem.setEnabled(!isReadOnly() && !isSelectionEmpty());
				m_menu->showPopUp(this,mouseEvent.getX(),mouseEvent.getY());
			}
		}
        else if(!Platf::isMobile() && mouseEvent.getButton() == agui::MOUSE_BUTTON_RIGHT)
		{
			if(m_cursorObject == CURSOR_ON_NAME)
			{
				generateAction(TC_SHOW_USER_CONTEXT);
			}
		}
	}

	int TextBox::getLineHeight() const
	{
		return agui::TextBox::getLineHeight() + 1;
	}

	void TextBox::setAsLabel( bool asLabel )
	{
		m_asLabel = asLabel;
		setReadOnly(true);
	}

	NameParseResultEnum TextBox::findLine( int index, int y ) 
	{
		int indexBytesOnStr = 0;
		m_cursorObject = CURSOR_ON_NOTHING;

		y -= getTopPadding();
		y -= getMargin(agui::SIDE_TOP);

		bool tooFarDown = y > (getTextLineCount() * getLineHeight());
		if(getTextLength() == 0 || tooFarDown || !m_keepingTableNumbers)
		{
			m_lastTableNum = -1;
			m_lastUserName = "";
			
			return NAME_PARSED_NOTHING;
		}
		int bytesSkipped = 0;
		int curCharLen = 0;
		std::string curChar;

		size_t ind = 0;

		int len = getTextLength();
		for(int i = 0; i < len && i < index; ++i)
		{
			curCharLen = unicodeFunctions.bringToNextUnichar(ind,getText());
			bytesSkipped += curCharLen;
		}
		int prevBytes = bytesSkipped;
		int nextBytes = bytesSkipped;

		while(prevBytes > 0 && getText()[prevBytes] != '\n')
		{
			prevBytes--;
		}

		int beginningMinus = 0;
		if(prevBytes == 0)
		{
			beginningMinus = -1;
		}

		indexBytesOnStr = bytesSkipped - prevBytes;

		while(nextBytes < getTextLength() && getText()[nextBytes] != '\n')
		{
			nextBytes++;
		}

		std::string result;

		for(int i = prevBytes; i < nextBytes; ++i)
		{
			//I do not need the message
			if(getText()[i] == ':')
			{
				break;
			}

			if(getText()[i] != '\n')
			{
				result += getText()[i];
			}
		}

		//parse in the table if any and the name
		if(result.length() > 0)
		{
			int tindex = 1;
			if(result[0] == '(')
			{
				//find the number
				while(tindex < (int)result.length() && result[tindex] != ')')
				{
					tindex++;
				}

				std::stringstream ss;

				for(int i = 1; i < tindex; ++i)
				{
					ss << result[i];
				}

				ss >> m_lastTableNum;

				tindex++; //after the ')' is a space

				if(indexBytesOnStr < tindex && m_keepingTableNumbers)
				{
					m_cursorObject = CURSOR_ON_TABLE;
				}
				else if(indexBytesOnStr == tindex)
				{
					//no good, on the space
				}
				else if(indexBytesOnStr <= result.length() + beginningMinus && m_keepingNames)
				{
					m_cursorObject = CURSOR_ON_NAME;
				}
			}
			else
			{
				tindex = 0;
				m_lastTableNum = -1;
				if(indexBytesOnStr <= result.length() + beginningMinus)
				{
					m_cursorObject = CURSOR_ON_NAME;
				}
			}

			//the name should be from tindex to the end
			if(tindex == 0)
			{
				m_lastUserName = result;
			}
			else
			{
				m_lastUserName = "";
				for(int i = tindex + 1; i < (int)result.length(); ++i)
				{
					m_lastUserName += result[i];
				}
			}
		
		}

		if(m_lastTableNum == -1)
		{
			return NAME_PARSED_NAME;
		}
		else
		{
			return NAME_PARSED_NAME_AND_TABLE;
		}

	}

	bool TextBox::isKeepingTableNumbers() const
	{
		return m_keepingTableNumbers;
	}

	void TextBox::setKeepingTableNumbers( bool keeping )
	{
		m_keepingTableNumbers = keeping;
	}

	void TextBox::mouseLeave(agui:: MouseEvent &mouseEvent )
	{
		agui::ExtendedTextBox::mouseLeave(mouseEvent);
		lastMouseX = -1;
		lastMouseY = -1;
		setCursor(getEnterCursor());
	}

	void TextBox::mouseMove( agui::MouseEvent &mouseEvent )
	{
		//relative mouse position
		agui::Point p = agui::Point(mouseEvent.getX(),
			mouseEvent.getY());

		lastMouseX = mouseEvent.getX();
		lastMouseY = mouseEvent.getY();

		agui::Point columnRow = columnRowFromRelPosition(p);
		int	mouseDownIndex = indexFromColumnRow(columnRow.getX(),columnRow.getY());

		findLine(mouseDownIndex,mouseEvent.getY());

		if(m_cursorObject != CURSOR_ON_NOTHING)
		{
			setCursor(agui::CursorProvider::LINK_CURSOR);
		}
		else
		{
			setCursor(getEnterCursor());
		}

		agui::ExtendedTextBox::mouseMove(mouseEvent);
	}

	void TextBox::valueChanged( agui::VScrollBar* source, int val )
	{
		agui::ExtendedTextBox::valueChanged(source,val);
		if(lastMouseY != -1 && lastMouseX != -1)
		{
			//relative mouse position
			agui::Point p = agui::Point(lastMouseX,
				lastMouseY);

			agui::Point columnRow = columnRowFromRelPosition(p);
			int	mouseDownIndex = indexFromColumnRow(columnRow.getX(),columnRow.getY());

			findLine(mouseDownIndex,lastMouseY);

			if(m_cursorObject != CURSOR_ON_NOTHING)
			{
				setCursor(agui::CursorProvider::LINK_CURSOR);
			}
			else
			{
				setCursor(getEnterCursor());
			}
		}
	}

	void TextBox::setText( const std::string &text )
	{
		agui::ExtendedTextBox::setText(text);
		if(lastMouseY != -1 && lastMouseX != -1)
		{
			//relative mouse position
			agui::Point p = agui::Point(lastMouseX,
				lastMouseY);

			agui::Point columnRow = columnRowFromRelPosition(p);
			int	mouseDownIndex = indexFromColumnRow(columnRow.getX(),columnRow.getY());

			findLine(mouseDownIndex,lastMouseY);

			if(m_cursorObject != CURSOR_ON_NOTHING)
			{
				setCursor(agui::CursorProvider::LINK_CURSOR);
			}
			else
			{
				setCursor(getEnterCursor());
			}
		}
	}

	void TextBox::mouseDrag( agui::MouseEvent &mouseEvent )
	{
		if(m_cursorObject == CURSOR_ON_NOTHING)
		{
			agui::ExtendedTextBox::mouseDrag(mouseEvent);
		}
	}

	void TextBox::setKeepingNames( bool keeping )
	{
		m_keepingNames = keeping;
	}

	bool TextBox::isKeepingNames() const
	{
		return m_keepingNames;
	}

	int TextBox::getLastMouseX() const
	{
		return lastMouseX;
	}

	int TextBox::getLastMouseY() const
	{
		return lastMouseY;
	}

	void TextBox::generateAction( TextCommandEnum c )
	{
		std::stringstream ss;
		ss << (int)c;
		dispatchActionEvent(agui::ActionEvent(this,ss.str()));
	}

	int TextBox::getTable() const
	{
		return m_lastTableNum;
	}

	const std::string& TextBox::getName() const
	{
		return m_lastUserName;
	}

	void TextBox::setIsTypingFont( agui::Font* f )
	{
		m_isTypingFont = f;
	}

	agui::Font* TextBox::getIsTypingFont() const
	{
		return m_isTypingFont;
	}

	void TextBox::setIsTypingImage( agui::Image* img )
	{
		m_isTypingImg = img;
	}

	agui::Image* TextBox::getIsTypingImage() const
	{
		return m_isTypingImg;
	}

	void TextBox::setIsTypingColor( const agui::Color& color )
	{
		m_isTypingColor = color;
	}

	const agui::Color& TextBox::getIsTypingColor() const
	{
		return m_isTypingColor;
	}

	void TextBox::activateIsTyping()
	{
		if(!m_isTyping)
		{
			m_pencilAnimCounter = 0;
			m_pencilX = 0;
			m_pencilY = -7;
			m_elipsisCounter = 0;
		}
		m_isTyping = true;
		m_isTypingCounter = m_isTypingTimeout;
	}

	void TextBox::setIsTypingTimeout( int timeout )
	{
		m_isTypingTimeout = timeout;
	}

	int TextBox::getIsTypingTimeout() const
	{
		return m_isTypingTimeout;
	}

	void TextBox::setIsTypingText( const std::string& text )
	{
		m_isTypingText = text;
	}

	const std::string& TextBox::getIsTypingText() const
	{
		return m_isTypingText;
	}

	void TextBox::setPaddingForIsTyping()
	{
        if(!Platf::isRetina())
            setBottomPadding(18);
        else
            setBottomPadding(18 * 2.0f);
	}

	void TextBox::logic(double time)
	{
		agui::ExtendedTextBox::logic(time);
		if(m_pencilAnimCounter >= 80)
		{
			m_pencilAnimCounter = 0;
			m_pencilX = 0;
			m_pencilY = -7;
		}

		if(m_pencilAnimCounter % 3 == 0)
		{
			if(m_pencilAnimCounter < 60)
			m_pencilX++;

			if(m_pencilAnimCounter >= 0 && m_pencilAnimCounter <= 20)
				m_pencilY++;
			if(m_pencilAnimCounter >= 21 && m_pencilAnimCounter <= 40)
				m_pencilY--;
			if(m_pencilAnimCounter >= 40 && m_pencilAnimCounter <= 60)
				m_pencilY++;
		}
	

		m_pencilAnimCounter++;


		if(m_elipsisCounter == 0)
		{
			m_fullPencil = getIsTypingText();
		}
		else if(m_elipsisCounter == 60)
		{
			m_fullPencil = getIsTypingText();
			m_fullPencil += ".";
		}
		else if(m_elipsisCounter == 120)
		{
			m_fullPencil = getIsTypingText();
			m_fullPencil += "..";
		}
		else if(m_elipsisCounter == 180)
		{
			m_fullPencil = getIsTypingText();
			m_fullPencil += "...";
		}

		m_elipsisCounter++;

		if(m_elipsisCounter >= 240)
		{
			m_elipsisCounter = 0;
		}
		if(m_isTyping)
		{
			if(m_isTypingCounter <= 0)
			{
				m_isTypingCounter = 0;
				haltIsTyping();
			}
			else
			{
				m_isTypingCounter--;
			}
		}
	}
    
    void TextBox::setFieldName(const std::string& name) {
        m_fieldName = name;
    }
    const std::string& TextBox::getFieldName() const {
        return m_fieldName;
    }

	void TextBox::haltIsTyping()
	{
		m_isTyping = false;
	}

}
