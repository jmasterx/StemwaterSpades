#include "Game/UI/GeneralDialogBox.hpp"
#include "Game/Utility/TimeOfDay.hpp"
#include "Game/UI/GeneralFrame.hpp"
#include <iostream>
#include <sstream>
namespace cge
{
	GeneralDialogBox::~GeneralDialogBox(void)
	{
		m_flow->removeWidgetListener(this);
		m_container->removeWidgetListener(this);
		m_vscroll->removeVScrollBarListener(this);
	}

	GeneralDialogBox::GeneralDialogBox(
		GuiFactory* factory, 
		LanguageManager* language, 
		GuiColorManager* color, 
		GuiFontManager* font )
		:VScrollController(NULL), m_factory(factory),m_language(language),
		m_color(color), m_font(font), m_genFrame(NULL)
	{
		setOkText("ok");
		m_vscroll = factory->createVScrollBar();
		m_vscroll->addVScrollBarListener(this);

		setVScrollBar(m_vscroll);
		m_container = factory->createEmptyWidget();
		m_container->addWidgetListener(this);
		m_flow = m_factory->createFlowLayout();
		m_flow->setMargins(0,0,0,0);
		m_flow->addWidgetListener(this);
		m_flow->setResizeRowToWidth(true);
		m_flow->setVerticalSpacing(3);
		m_flow->setResizeToParent(false);
		m_container->add(m_flow);
		m_container->add(m_vscroll);
		m_container->addMouseListener(this);
		m_flow->addMouseListener(this);
	}

	Label* GeneralDialogBox::addLabel(
		const std::string& text, const std::string& color /*= ""*/ )
	{
		Label* label = m_factory->createLabel();

        if(Platf::isRetina())
            label->setFont(m_font->getFont(2.0f));
        
		label->setAutosizing(false);
		if(text != "")
		{
			label->setText(m_language->getElement(text));
		}

		if(m_labels.size() > 0)
		{
			label->setMargins(5,1,5,1);
		}
		else
		{
			label->setMargins(1,1,5,1);
		}
		m_labels.push_back(label);

		FlowLayout* flow = addFlow();
		flow->add(label);
		resizeLabels();
		resizeFlowsToContents();

		if(color != "")
		{
			label->setFontColor(m_color->getColor(color));
			float scalar = 0.7f;
			label->setFontColor(
				agui::Color(label->getFontColor().getR() * scalar, label->getFontColor().getG() *
				scalar, label->getFontColor().getB() * scalar));
		}

		label->addMouseListener(this);
		return label;
	}

	FlowLayout* GeneralDialogBox::addFlow()
	{
		FlowLayout* flow = m_factory->createFlowLayout();
		flow->setHorizontalSpacing(6);
		flow->setVerticalSpacing(2);
		m_flows.push_back(flow);
		m_flow->add(flow);
		flow->addMouseListener(this);
		return flow;
	}

	void GeneralDialogBox::resizeFlowsToContents()
	{
		for(size_t i = 0; i < m_flows.size(); ++i)
		{
			m_flows[i]->setSize(m_flows[i]->getWidth(),
				m_flows[i]->getContentsHeight());
		}
	}

	agui::EmptyWidget* GeneralDialogBox::getWidget()
	{
		return m_container;
	}

	void GeneralDialogBox::resizeLabels()
	{
		for(size_t i = 0; i < m_labels.size(); ++i)
		{
            if(!Platf::isRetina())
			m_labels[i]->setFont(m_font->getFont(1.1f));
            else
              m_labels[i]->setFont(m_font->getFont(2.0f * 1.1f));
			m_labels[i]->resizeToContents();
		}
	}

	Button* GeneralDialogBox::createButton( const std::string& text, const std::string& color /*= "host.button"*/,const std::string& tooltip /*= ""*/ )
	{
		Button* button = m_factory->createWhiteButton();
		std::string c = color;
		if(c == "")
		{
			c = "host.button";
		}
		if(text != "")
		{
			button->setText(m_language->getElement(text));
		}

		if(tooltip != "")
		{
			button->setToolTipText(m_language->getElement(tooltip));
		}

		if(c != "")
		{
			button->setFontColor(m_color->getColor(c));
		}

		button->setContentResizeScale(1.1f,1.2f);
        if(!Platf::isRetina())
		button->setFont(m_font->getFont(0.9f));
        else
        button->setFont(m_font->getFont(0.9f * 2.0f));
		button->setToggleButton(true);
		m_buttons.push_back(button);
		button->addMouseListener(this);

		button->resizeToContents();

		return button;
	}

	FlowLayout* GeneralDialogBox::addButtonGroup( const std::vector<Button*> &buttons,
		agui::ButtonGroup& group, int numOnRow )
	{
		FlowLayout* flow = addFlow();
		flow->setHorizontallyCentered(false);
		flow->setMaxOnRow(numOnRow);
		for(size_t i = 0; i < buttons.size(); ++i)
		{
			flow->add(buttons[i]);
			group.add(buttons[i]);
		}

		group.addActionListener(this);
		resizeFlowsToContents();

		return flow;
	}

	void GeneralDialogBox::sizeChanged( 
		agui::Widget* source, const agui:: Dimension &size )
	{

	}

	void GeneralDialogBox::actionPerformed( const agui::ActionEvent &evt )
	{
		resizeFlowsToContents();
		updateScrollBars();

		if(evt.getSource() == m_genFrame)
		{
			frameActionReceived(m_genFrame,evt.getId());
		}
	}

	void GeneralDialogBox::onParentSizeChanged( agui::Widget* source, const agui::Dimension &parentInnerSize )
	{
		if(source == m_container)
		{
			m_container->setSize(parentInnerSize);

			int extra = isVScrollNeeded() ?  m_vscroll->getWidth() : 0;
			m_flow->setSize(m_container->getInnerWidth() - extra,
				getContentHeight()
				);

			extra = isVScrollNeeded() ?  m_vscroll->getWidth() : 0;

			int topMar = parentInnerSize.getHeight() * 0.0;
			m_flow->setMargins(topMar,5,topMar / 3,1);
			m_flow->setSize(m_container->getInnerWidth() - extra,
				getContentHeight());

			resizeFlowsToContents();
			updateScrollBars();
		}
	}

	int GeneralDialogBox::getContentHeight() const
	{
		return m_flow->getContentsHeight();
	}

	int GeneralDialogBox::getMaxHeight() const
	{
		int h = m_container->getInnerHeight() ;


		return h > 0 ? h : 0;
	}

	int GeneralDialogBox::getMaxWidth() const
	{
		return m_container->getInnerWidth();
	}

	void GeneralDialogBox::valueChanged( agui::VScrollBar* source, int val )
	{
		m_flow->setLocation(0,-val);
	}

	void GeneralDialogBox::mouseWheelDownCB( agui::MouseEvent &mouseEvent )
	{
		m_vscroll->wheelScrollDown(mouseEvent.getMouseWheelChange());
	}

	void GeneralDialogBox::mouseWheelUpCB( agui::MouseEvent &mouseEvent )
	{
		m_vscroll->wheelScrollUp(mouseEvent.getMouseWheelChange());
	}

	void GeneralDialogBox::updateScrollBars()
	{
		VScrollController::updateScrollBars();
		int center = 3;
		int subH = 5;
		m_vscroll->setLocation(m_vscroll->getLocation().getX(),center);
		m_vscroll->setSize(m_vscroll->getWidth(),m_vscroll->getHeight() - subH);
	}

	TextField* GeneralDialogBox::createTextField( const std::string& labelName, int length, const std::string& tooltip /*= ""*/ )
	{
        if(Platf::isRetina())
            length *= 2.0f;
        
		FlowLayout* f = addFlow();
		f->setMargins(1,1,1,1);
		TextField* field = m_factory->createTextField();
        if(Platf::isRetina())
            field->setFont(m_font->getFont(2.0f));
		field->setSize(length,0);
		field->resizeHeightToContents();
		Label* label = m_factory->createLabel();
        if(Platf::isRetina())
            label->setFont(m_font->getFont(2.0f));
		m_alignLabels.push_back(label);
		label->setMargins(4,1,5,1);
		if(labelName.length() > 0)
		label->setText(m_language->getElement(labelName));
		if(tooltip.length() > 0)
			field->setToolTipText(m_language->getElement(tooltip));
		label->addMouseListener(this);
		field->addMouseListener(this);
		field->setHideSelection(true);
        field->setFieldName(label->getText());
		f->add(label);
		f->add(field);
		resizeAlignLabels();
		return field;
	}

	void GeneralDialogBox::resizeAlignLabels()
	{
		int biggest = 0;
		for(int i = 0; i < m_alignLabels.size(); i++)
		{
			int sz = m_alignLabels[i]->getFont()->getTextWidth(m_alignLabels[i]->getText());
			if(sz > biggest)
				biggest = sz;
		}

		for(int i = 0; i < m_alignLabels.size(); i++)
		{
			m_alignLabels[i]->setAutosizing(false);
			m_alignLabels[i]->setAlignment(agui::ALIGN_MIDDLE_RIGHT);
			m_alignLabels[i]->resizeToWidth(biggest + m_alignLabels[i]->getMargin(agui::SIDE_LEFT) + m_alignLabels[i]->getMargin(agui::SIDE_RIGHT));
		}
	}

	CheckBox* GeneralDialogBox::createCheckBox( const std::string& text, const std::string& tooltip )
	{
		FlowLayout* f = addFlow();
		f->setMargins(1,1,1,1);
		CheckBox* field = m_factory->createCheckBox();
        field->setAutosizing(true);
        if(Platf::isRetina())
            field->setFont(m_font->getFont(2.0f));
		field->setText(m_language->getElement(text));
		//field->resizeToContents();
		field->addMouseListener(this);
		f->add(field);
		return field;
	}

	std::vector<DropDown*> GeneralDialogBox::createDate( const std::string& title )
	{
		std::vector<DropDown*> dds;
		std::stringstream ss;

		FlowLayout* f = addFlow();
		f->setMargins(1,1,1,1);
		DropDown* ddMonth = m_factory->createDropDown();
		DropDown* ddDay = m_factory->createDropDown();
		DropDown* ddYear = m_factory->createDropDown();

		Label* label = m_factory->createLabel();
        if(Platf::isRetina())
            label->setFont(m_font->getFont(2.0f));

		int startYear = TimeOfDay::getYear();
		for(int i = startYear; i >= startYear - 100; i--)
		{
			ss.clear();
			ss.str("");
			ss << i;

			ddYear->addItem(ss.str());
		}
        
		ddMonth->addItem(m_language->getElement("month.january"));
		ddMonth->addItem(m_language->getElement("month.february"));
		ddMonth->addItem(m_language->getElement("month.march"));
		ddMonth->addItem(m_language->getElement("month.april"));
		ddMonth->addItem(m_language->getElement("month.may"));
		ddMonth->addItem(m_language->getElement("month.june"));
		ddMonth->addItem(m_language->getElement("month.july"));
		ddMonth->addItem(m_language->getElement("month.august"));
		ddMonth->addItem(m_language->getElement("month.september"));
		ddMonth->addItem(m_language->getElement("month.october"));
		ddMonth->addItem(m_language->getElement("month.november"));
		ddMonth->addItem(m_language->getElement("month.december"));

		for(int i = 1; i <= 31; i++)
		{
			ss.clear();
			ss.str("");
			ss << i;

			ddDay->addItem(ss.str());
		}

		ddYear->setNoSelectionText(m_language->getElement("date.year"));
		ddYear->resizeToContents();
		ddMonth->setNoSelectionText(m_language->getElement("date.month"));
		ddMonth->resizeToContents();
		ddDay->setNoSelectionText(m_language->getElement("date.day"));
		ddDay->resizeToContents();
		m_alignLabels.push_back(label);
		label->setMargins(4,1,5,1);
		label->setText(m_language->getElement(title));
		label->addMouseListener(this);
		ddMonth->addMouseListener(this);
		ddDay->addMouseListener(this);
		ddYear->addMouseListener(this);

		
        if(Platf::isRetina())
        {
            ddYear->agui::Widget::setSize(ddYear->getWidth() * 1.25f, ddYear->getHeight());
        }

        
		f->add(label);
		f->add(ddMonth);
		f->add(ddDay);
		f->add(ddYear);
		resizeAlignLabels();
		dds.push_back(ddMonth);
		dds.push_back(ddDay);
		dds.push_back(ddYear);
		return dds;
	}

	Label* GeneralDialogBox::createLabel( const std::string& labelName, const std::string& text,bool appendColon, const std::string& tooltip /*= ""*/ )
	{
		FlowLayout* f = addFlow();
		f->setMargins(1,1,1,1);
		Label* field = m_factory->createLabel();
		Label* label = m_factory->createLabel();
		m_alignLabels.push_back(label);
        
        if(Platf::isRetina())
        {
            field->setFont(m_font->getFont(2.0f));
            label->setFont(m_font->getFont(2.0f));
        }
		label->setText(m_language->getElement(labelName));
		if(appendColon)
			label->setText(label->getText() + ":");

		if(tooltip.length() > 0)
			field->setToolTipText(m_language->getElement(tooltip));
		field->setText(text);
		label->addMouseListener(this);
		field->addMouseListener(this);

		f->add(label);
		f->add(field);
		resizeAlignLabels();
		return field;
	}

	Slider* GeneralDialogBox::createSlider( const std::string& labelName, const std::string& tooltip /*= ""*/ )
	{
		FlowLayout* f = addFlow();
		f->setMargins(1,1,1,1);
		Slider* field = m_factory->createSlider();
		field->setSize(150,field->getHeight());
		Label* label = m_factory->createLabel();
		m_alignLabels.push_back(label);
        if(Platf::isRetina())
            label->setFont(m_font->getFont(2.0f));
		label->setText(m_language->getElement(labelName));
		if(tooltip.length() > 0)
			field->setToolTipText(m_language->getElement(tooltip));
		label->addMouseListener(this);
		field->addMouseListener(this);

        if(Platf::isRetina())
            label->setMargins(20, 1, 1, 1);
        
		f->add(label);
		f->add(field);
		resizeAlignLabels();
		return field;
	}

	DropDown* GeneralDialogBox::createDropdown( const std::string& labelName, const std::string& title, const std::vector<std::string>& items, const std::string& tooltip /*= ""*/ )
	{
		FlowLayout* f = addFlow();
		f->setMargins(1,1,1,1);
		
		DropDown* field = m_factory->createDropDown();
		field->setNoSelectionText(m_language->getElement(title));
		Label* label = m_factory->createLabel();
        if(Platf::isRetina())
            label->setFont(m_font->getFont(2.0f));
		label->setMargins(2,1,1,1);
		m_alignLabels.push_back(label);
		label->setText(m_language->getElement(labelName));
		if(tooltip.length() > 0)
			field->setToolTipText(m_language->getElement(tooltip));
		label->addMouseListener(this);
		field->addMouseListener(this);
		for(int i = 0; i < items.size(); i++)
		{
			field->addItem(m_language->getElement(items[i]));
		}
		field->resizeToContents();
		f->add(label);
		f->add(field);
		resizeAlignLabels();
		return field;
	}

	void GeneralDialogBox::setTitle( const std::string& text )
	{
		m_title = m_language->getElement(text);
	}

	void GeneralDialogBox::setTitle( const std::string& text, const std::string& arg )
	{
		m_title = m_language->getElementWithArg(text,arg);
	}

	const std::string& GeneralDialogBox::getTitle() const
	{
		return m_title;
	}

	void GeneralDialogBox::dialogWillAppear()
	{

	}

	void GeneralDialogBox::setOkText( const std::string& text )
	{
		m_okText = text;
	}

	const std::string& GeneralDialogBox::getOkText() const
	{
		return m_okText;
	}

	void GeneralDialogBox::setGeneralFrame( GeneralFrame* frame )
	{
		m_genFrame = frame;
	}

	cge::GeneralFrame* GeneralDialogBox::getGeneralFrame()
	{
		return m_genFrame;
	}

	void GeneralDialogBox::frameActionReceived( GeneralFrame* frame,const std::string& action )
	{

	}

	void GeneralDialogBox::hideGeneralFrame()
	{
		if(m_genFrame)
		{
			m_genFrame->hideDialog();
			m_genFrame->removeWidgetsFromBottom(parentBottomWidgets());
		}
	}

	GuiFactory* GeneralDialogBox::getFactory()
	{
		return m_factory;
	}

	void GeneralDialogBox::setUnlocalizedTitle( const std::string& text )
	{
		m_title = text;
	}

	bool GeneralDialogBox::wantCloseButton() const
	{
		return true;
	}

	void GeneralDialogBox::scrollToTop()
	{
		m_vscroll->setValue(0);
	}

	TextBox* GeneralDialogBox::createFullTextBox()
	{
		GridLayout* grid = m_factory->createGridLayout();
		grid->setNumberOfRows(1);
		TextBox* text = m_factory->createTextBox();
		grid->add(text);
		m_container->add(grid);
		return text;
	}

	agui::FlowLayout* GeneralDialogBox::getFlow()
	{
		return m_flow;
	}

	void GeneralDialogBox::setCancelText( const std::string& text )
	{
		m_cancelText = text;
	}

	const std::string& GeneralDialogBox::getCancelText() const
	{
		return m_cancelText;
	}

	Button* GeneralDialogBox::createActionButton( const std::string& text )
	{
		FlowLayout* f = addFlow();
		f->setMargins(1,1,1,1);
		Button* field = m_factory->createWhiteButton();
        if(Platf::isRetina())
        field->setFont(m_font->getFont(2.0f));
		field->setText(m_language->getElement(text));
		field->resizeToContents();
		field->addMouseListener(this);
		f->add(field);
		return field;
	}

	void GeneralDialogBox::addWhitespace( int height )
	{
		FlowLayout* f = addFlow();
		f->setMargins(1,1,1,1);
        if(Platf::isRetina())
            height *= 2.0f;
		agui::Widget* field = m_factory->createEmptyWidget();
		field->setSize(1,height);
		field->addMouseListener(this);
		f->add(field);
	}

	float GeneralDialogBox::getWidthScalar() const
	{
		return 0.7f;
	}

	float GeneralDialogBox::getHeightScalar() const
	{
		return 0.7f;
	}

	ProfileAvatar* GeneralDialogBox::createProfileAvatar()
	{
		FlowLayout* f = addFlow();
		f->setMargins(1,1,1,1);
		ProfileAvatar* field = m_factory->createProfileAvatar();
		field->setSize(150,150);
		field->addMouseListener(this);
		f->add(field);
		return field;
	}

	bool GeneralDialogBox::wantOkButton() const
	{
		return true;
	}

	void GeneralDialogBox::forceResize()
	{
		onParentSizeChanged(m_container,m_container->getInnerSize());
		onParentSizeChanged(m_container,m_container->getInnerSize());
	}

	std::string GeneralDialogBox::getSideLabelText() const
	{
		return "";
	}

	const std::vector<agui::Widget*>& GeneralDialogBox::parentBottomWidgets()
	{
		return m_bottomWidgets;
	}

	agui::Widget* GeneralDialogBox::focusWidget()
	{
		return NULL;
	}

	TextBox* GeneralDialogBox::createTextBox( bool autosizeWidth )
	{
		FlowLayout* f = addFlow();
		f->setMargins(1,1,1,1);
		TextBox* field = m_factory->createTextBox();
		field->setSize(250,250);
		field->addMouseListener(this);

        if(Platf::isRetina())
        {
            field->setFont(getFactory()->getFontManager()->getFont(2.0f));
        }
        
		f->add(field);
		if(autosizeWidth)
		{
			f->setSingleRow(true);
			f->setResizeRowToWidth(true);
		}
		resizeAlignLabels();
		return field;
	}


}
