#ifndef CGE_CONCURRENT_QUEUE_HPP
#define CGE_CONCURRENT_QUEUE_HPP
#include <boost/thread.hpp>  
#include <queue>
template<typename Data>
class ConcurrentQueue
{
private:
	std::queue<Data> m_queue;
	mutable boost::mutex m_mutex;
	boost::condition_variable m_cond;
public:
	void push(Data const& data)
	{
		boost::mutex::scoped_lock lock(m_mutex);
		m_queue.push(data);
		lock.unlock();
		m_cond.notify_one();
	}

	bool empty() const
	{
		boost::mutex::scoped_lock lock(m_mutex);
		return m_queue.empty();
	}

	bool tryPop(Data& popped_value)
	{
		boost::mutex::scoped_lock lock(m_mutex);
		if(m_queue.empty())
		{
			return false;
		}

		popped_value=m_queue.front();
		m_queue.pop();
		return true;
	}

	void waitAndPop(Data& popped_value)
	{
		boost::mutex::scoped_lock lock(m_mutex);
		while(m_queue.empty())
		{
			m_cond.wait(lock);
		}

		popped_value=m_queue.front();
		m_queue.pop();
	}
};

#endif