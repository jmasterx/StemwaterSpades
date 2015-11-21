#include "Game/UI/PopUpMenu.hpp"
#include "Game/Platform/Platf.hpp"
namespace cge
{
	PopUpMenu::PopUpMenu( 
		agui::Image* popUpBg, agui::Image* highlightImg, agui::Image* subArrowImg )
		: m_popUpBg(popUpBg),m_highlightImg(highlightImg),m_subArrowImg(subArrowImg),
		m_opacityRate(0.08f),m_disabledFontColor(0.4f,0.4f,0.4f)
	{
	}

	PopUpMenu::~PopUpMenu(void)
	{
	}

	void PopUpMenu::showPopUp( agui::Widget* invoker, int x, int y, agui::PopUpMenu* parentPopUp )
	{
		
		agui::PopUpMenu::showPopUp(invoker,x,y,parentPopUp);
		setOpacity(0.0f);
	}

	void PopUpMenu::logic( double timeElapsed )
	{
		agui::PopUpMenu::logic(timeElapsed);
		if(getOpacity() < 1.0f)
		{
			setOpacity(getOpacity() + m_opacityRate);
		}
	}

	void PopUpMenu::paintBackground( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawNinePatchImage(
			m_popUpBg,agui::Point(),getSize(),getOpacity());
	}

	void PopUpMenu::paintComponent( const agui::PaintEvent &paintEvent )
	{
		int totalHeight = 0;
		agui::Color regularColor = agui::Color(
			getFontColor().getR(),getFontColor().getG(),getFontColor().getB(),getOpacity());
		agui::Color disabledColor = agui::Color(
			getDisabledFontColor().getR(),getDisabledFontColor().getG(),
			getDisabledFontColor().getB(),getOpacity());

		for(int i = 0; i < getLength(); ++i)
		{
			int w = 0;
			agui::PopUpMenuItem* item = getItemAt(i);

			if(i == getSelectedIndex() && item->getItemType() != agui::PopUpMenuItem::SEPARATOR)
			{
				paintEvent.graphics()->drawNinePatchImage(
					m_highlightImg,agui::Point(0,totalHeight),
					agui::Dimension(getInnerWidth(),getItemHeight()),getOpacity());
			}

			//draw the icon
			if(isShowingIcon())
			{
				if(item->getIcon())
				{
					paintEvent.graphics()->
						drawImage(item->getIcon(),getIconPosition(i,totalHeight),getOpacity());
				}

				w += getIconWidth();
			}

			if(item->isSeparator())
			{
				paintEvent.graphics()->drawLine(
					agui::Point(w,totalHeight + (getItemHeight(item) / 2)),
					agui::Point(getInnerWidth(),totalHeight + (getItemHeight(item) / 2)),
					agui::Color(90,90,90));

				paintEvent.graphics()->drawLine(
					agui::Point(w,totalHeight + (getItemHeight(item) / 2) + 1),
					agui::Point(getInnerWidth(),totalHeight + (getItemHeight(item) / 2) + 1),
					agui::Color(200,200,200));
			}

			w += getStartTextGap();

			agui::Color* fontColor = item->isEnabled() ? &regularColor : &disabledColor;

			paintEvent.graphics()->drawText(agui::Point(w,getTextCenter() + totalHeight),
				item->getText().c_str(),
				*fontColor,
				getFont());

			w+= getMiddleTextGap();

			agui::Point shortcutPoint = alignString(item->getShortcutText(),agui::ALIGN_MIDDLE_RIGHT);
			shortcutPoint.setX(shortcutPoint.getX() - getEndTextGap());
			shortcutPoint.setY(getTextCenter() + totalHeight);


			paintEvent.graphics()->drawText(shortcutPoint,
				item->getShortcutText().c_str(),*fontColor,
				getFont());

			if(item->isSubMenu())
			{
				paintEvent.graphics()->drawImage(m_subArrowImg,
					createAlignedPosition(agui::ALIGN_MIDDLE_CENTER,
					agui::Rectangle(getInnerWidth() - getEndTextGap(),
					totalHeight,getEndTextGap(),getItemHeight()),
					agui::Dimension(m_subArrowImg->getWidth(),m_subArrowImg->getHeight())),
					getOpacity());
			}

			totalHeight += getItemHeight(item);
		}
	}

	void PopUpMenu::setDisabledFontColor( const agui::Color& color )
	{
		m_disabledFontColor = color;
	}

	const agui::Color& PopUpMenu::getDisabledFontColor() const
	{
		return m_disabledFontColor;
	}

	int PopUpMenu::getItemHeight() const
	{
        if(!Platf::isRetina())
		return agui::PopUpMenu::getItemHeight() * 1.25f;
        else
        return agui::PopUpMenu::getItemHeight() * 4.0f;
	}

	int PopUpMenu::getItemHeight( agui::PopUpMenuItem* item ) const
	{
		return agui::PopUpMenu::getItemHeight(item);
	}

	void PopUpMenu::mouseDrag( agui::MouseEvent &mouseEvent )
	{
		mouseMove(mouseEvent);
	}

}
