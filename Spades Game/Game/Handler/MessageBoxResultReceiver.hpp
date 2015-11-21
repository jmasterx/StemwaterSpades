#ifndef CGE_MESSAGE_BOX_RESULT_RECEIVER_HPP
#define CGE_MESSAGE_BOX_RESULT_RECEIVER_HPP
#include "Game/UI/MessageBox.hpp"
namespace cge
{
	class MessageBoxResultReceiver
	{
		int m_msgTag;
	public:
		MessageBoxResultReceiver(void);
		virtual void messageBoxResult(MessageBox* msgbox,MessageBox::MessageResult result) = 0;
		void setMessageResultTag(int tag);
		int getMessageResultTag() const;
		virtual ~MessageBoxResultReceiver(void);
	};
}

#endif
