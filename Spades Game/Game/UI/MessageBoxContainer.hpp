#ifndef CGE_MESSAGE_BOX_CONTAINER_HPP
#define CGE_MESSAGE_BOX_CONTAINER_HPP
#include "Game/UI/MessageBox.hpp"
#include "Game/Handler/MessageBoxResultReceiver.hpp"
namespace cge
{
	class MessageBoxContainer : public agui::Widget
	{
		Frame* m_frame;
		MessageBox* m_msg;
	public:
		virtual void parentSizeChanged();
		void recenter();
		MessageBoxContainer(Frame* frame,
		MessageBox* msg);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		void showMessage(const std::string& title, const std::string& message, MessageBox::MessageType type, MessageBoxResultReceiver* rec, int tag);
		bool isBusy() const;
		int getTag() const;
		virtual void logic(double timeElapsed);
		virtual ~MessageBoxContainer(void);
	};
}

#endif
