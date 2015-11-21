#include "Game/Handler/MessageBoxResultReceiver.hpp"
namespace cge
{
	MessageBoxResultReceiver::MessageBoxResultReceiver(void)
		: m_msgTag(-1)
	{
	}

	MessageBoxResultReceiver::~MessageBoxResultReceiver(void)
	{
	}

	void MessageBoxResultReceiver::setMessageResultTag( int tag )
	{
		m_msgTag = tag;
	}

	int MessageBoxResultReceiver::getMessageResultTag() const
	{
		return m_msgTag;
	}

}
