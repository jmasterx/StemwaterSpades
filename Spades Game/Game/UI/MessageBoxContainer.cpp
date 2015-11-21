#include "Game/UI/MessageBoxContainer.hpp"
namespace cge
{

	MessageBoxContainer::MessageBoxContainer( Frame* frame, MessageBox* msg )
		: m_frame(frame),m_msg(msg)
	{
		add(m_frame);
		setMargins(0,0,0,0);
		m_frame->setResizable(false);
		m_frame->setMovable(false);
		setVisibility(false);
		setFocusable(false);
	}

	MessageBoxContainer::~MessageBoxContainer(void)
	{
		remove(m_frame);
	}

	void MessageBoxContainer::paintBackground( const agui::PaintEvent &paintEvent )
	{

	}

	void MessageBoxContainer::paintComponent( const agui::PaintEvent &paintEvent )
	{

	}

	void MessageBoxContainer::showMessage( const std::string& title, const std::string& message, MessageBox::MessageType type, MessageBoxResultReceiver* rec, int tag )
	{
		if(getGui())
		{
			if(m_msg->getModalContainer()->getParent() == NULL)
				getGui()->add(m_msg->getModalContainer());

			m_msg->getModalContainer()->bringToFront();
			m_msg->getModalContainer()->setVisibility(true);
		}

		setGlobalOpacity(0.0f);
		bringToFront();
		m_msg->showMessage(title,message,m_frame,type,rec,tag);
		setSize(m_frame->getSize());
		recenter();
		setVisibility(true);
		bool gotFocus = requestModalFocus();
	}

	void MessageBoxContainer::recenter()
	{
		if(getParent())
		{
			alignToParent(agui::ALIGN_MIDDLE_CENTER);
		}
	}

	void MessageBoxContainer::parentSizeChanged()
	{
		recenter();
	}

	bool MessageBoxContainer::isBusy() const
	{
		return isVisible();
	}

	int MessageBoxContainer::getTag() const
	{
		return m_msg->getTag();
	}

	void MessageBoxContainer::logic( double timeElapsed )
	{
		if(getGlobalOpacity() < 1.0f)
		{
			float o = getGlobalOpacity();
			o += 0.07;
			if(o > 1.0f)
				o = 1.0f;
			setGlobalOpacity(o);
		}
	}

}