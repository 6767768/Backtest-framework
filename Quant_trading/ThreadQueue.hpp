/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright © 2020 Xiwei Wang 
 * Created by Xiwei Wang on 07/26/2020
 * All rights reserved.                                               
 *                                               
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef THREADQUEUE
#define THREADQUEUE

#include<mutex>
#include<thread>
#include<queue>
#include<memory>
#include<exception>

class QueueException :std::exception {
	const char*what() const throw() {
		return "EmptyException";
	}
};

template<typename T>
class ThreadQueue {
private:
	mutable std::mutex m_mutex;
	/*被mutable修饰的变量，将永远处于可变的状态，即使在一个const函数中。
	但问题是，我只想修改某个成员变量，其余成员变量仍然希望被const保护。*/
	std::queue<T> Queue;
public:
	ThreadQueue() {};
	ThreadQueue(const ThreadQueue& Q) {
		std::lock_guard<std::mutex>(Q.m_mutex);
		Queue = Q.Queue;
	}
	ThreadQueue& operator=(const ThreadQueue&) = delete;
	bool empty() const;
	size_t size() const;
	void push(const T &ele);
	std::shared_ptr<T> pop();
	void pop(T&value);
	void pop(T *value);
};

template<typename T>
bool ThreadQueue<T>::empty() const {
	std::lock_guard<std::mutex> guard(m_mutex);
	return Queue.empty();
}

template<typename T>
size_t ThreadQueue<T>::size() const {
	std::lock_guard<std::mutex> guard(m_mutex);
	return Queue.size();
}

template<typename T>
void ThreadQueue<T>::push(const T &ele) {
	std::lock_guard<std::mutex> guard(m_mutex);
	Queue.push(ele);
}

template<typename T>
std::shared_ptr<T> ThreadQueue<T>::pop() {
	std::lock_guard<std::mutex> lock(m_mutex);
	if (Queue.empty())throw QueueException();
	std::shared_ptr<T>const res(std::make_shared<T>(Queue.pop()));
	Queue.pop();
	return res;
}

template<typename T>
void ThreadQueue<T>::pop(T&value) {
	std::lock_guard<std::mutex> guard(m_mutex);
	if (Queue.empty())throw QueueException();
	value = Queue.front();
	Queue.pop();
}

template<typename T>
void ThreadQueue<T>::pop(T*value) {
	std::lock_guard<std::mutex> guard(m_mutex);
	if (Queue.empty())throw QueueException();
	*value = Queue.front();
	Queue.pop();
}

#endif // !THREADQUEUE



