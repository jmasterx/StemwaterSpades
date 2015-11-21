#include "Game/UI/TextField.hpp"

namespace cge
{
	TextField::~TextField(void)
	{
	}

	TextField::TextField( agui::Image* bgImage, agui::Image* focusImage, PopUpMenu* menu,
		LanguageManager* lang)
		:m_bgImage(bgImage), m_focusImage(focusImage), m_menu(menu),
		m_cutItem("Cut", "Ctrl + X"), m_copyItem("Copy","Ctrl + C"), 
		m_pasteItem("Paste","Ctrl + V"), m_selectAllItem("Select All","Ctrl + A"),
		m_deleteItem("Delete"),
		m_separatorItem(agui::PopUpMenuItem::SEPARATOR),m_needsEmoticons(false),
    m_wantKeyboardNotification(false)
	{

		if(lang)
		{
			m_cutItem.setText(lang->getElement("popup.cut"));
			m_copyItem.setText(lang->getElement("popup.copy"));
			m_pasteItem.setText(lang->getElement("popup.paste"));
			m_selectAllItem.setText(lang->getElement("popup.select.all"));
			m_deleteItem.setText(lang->getElement("popup.delete"));
		}

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
    
    void TextField::setNeedsEmoticons(bool needs) {
        m_needsEmoticons = needs;
    }
    
    bool TextField::needsEmoticons() const {
        return m_needsEmoticons;
    }

	void TextField::paintBackground( const agui::PaintEvent &paintEvent )
	{
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

	void TextField::paintComponent( const agui::PaintEvent &paintEvent )
	{
		int caretLoc = getCaretLocation();
		int textLoc = getTextOffset();

		agui::Rectangle sideclip = getInnerRectangle();
		sideclip = agui::Rectangle(sideclip.getX() + getLeftPadding() ,
			sideclip.getY() + 2,sideclip.getSize().getWidth() - getLeftPadding()
			- getRightPadding() + 1, sideclip.getHeight() - 4);

		paintEvent.graphics()->pushClippingRect(sideclip);

		if(getSelectionStart() != getSelectionEnd() && (isFocused() || !isHidingSelection()) )
		{
			agui::Rectangle selRect = agui::Rectangle(
				getSelectionLocation(),
				(getInnerHeight() / 2) - 
				(getFont()->getLineHeight() / 2),
				getSelectionWidth(),
				getFont()->getLineHeight());

			paintEvent.graphics()->drawFilledRectangle(
				selRect,getSelectionBackColor());
		}


		paintEvent.graphics()->drawText(agui::Point(textLoc, +
			((getInnerSize().getHeight() - getFont()->getLineHeight()) / 2)),getText().c_str(),
			getFontColor(),getFont());


		if(isFocused())
		{
			if(isBlinking())
				paintEvent.graphics()->drawLine(agui::Point(caretLoc + 1,
				((getInnerSize().getHeight() / 2) + (getFont()->getLineHeight() / 2))),
				agui::Point(caretLoc + 1, ((getInnerSize().getHeight() / 2) - 
				(getFont()->getLineHeight() / 2))),
				agui::Color(0,0,0));
		}

		paintEvent.graphics()->popClippingRect();

	}

	void TextField::actionPerformed( const agui::ActionEvent &evt )
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

	void TextField::mouseDown( agui::MouseEvent &mouseEvent )
	{
		agui::TextField::mouseDown(mouseEvent);
		if(mouseEvent.getButton() == agui::MOUSE_BUTTON_RIGHT)
		{
			if(m_menu)
			{
				m_pasteItem.setEnabled(!isReadOnly());
				m_cutItem.setEnabled(!isReadOnly() && !(getSelectionStart() == getSelectionEnd()));
				m_copyItem.setEnabled(!(getSelectionStart() == getSelectionEnd()));
				m_deleteItem.setEnabled(!isReadOnly() && !(getSelectionStart() == getSelectionEnd()));
				m_menu->showPopUp(this,mouseEvent.getX(),mouseEvent.getY());
			}
		}
	}
    
    void TextField::setFieldName(const std::string& name) {
        m_fieldName = name;
    }
    const std::string& TextField::getFieldName() const {
        return m_fieldName;
    }
    
    void TextField::setWantKeyboardNotification(bool needs) {
        m_wantKeyboardNotification = needs;
    }
    bool TextField::wantsKeyboardNotification() const {
        return  m_wantKeyboardNotification;
    }
    
    void TextField::keyboardNotification() {
        dispatchActionEvent(agui::ActionEvent(this));
    }

}
