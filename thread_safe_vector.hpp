// thread_safe_vector.hpp: starter code -- please refer to in-class example for ThreadSafeQueue

#ifndef THREAD_SAFE_VECTOR_HPP
#define THREAD_SAFE_VECTOR_HPP

#include <vector>
#include <mutex>
#include <condition_variable>
#include <utility>

template <typename T>
class ThreadSafeVector
{
private:
    std::vector<T> data;
    mutable std::mutex m;
    std::condition_variable condition;

public:
    ThreadSafeVector() = default;

    void push_back(T value)
    {
        {
            std::lock_guard<std::mutex> lock(m);
            data.push_back(std::move(value));
        }
        condition.notify_one();
    }

    void wait_and_pop_back(T& value)
    {
        std::unique_lock<std::mutex> lock(m);

        condition.wait(lock, [this] {
            return !data.empty();
        });

        value = std::move(data.back());
        data.pop_back();
    }

    bool try_pop_back(T& value)
    {
        std::lock_guard<std::mutex> lock(m);

        if (data.empty()) {
            return false;
        }

        value = std::move(data.back());
        data.pop_back();
        return true;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};

#endif

// // // thread_safe_queue.hpp
// #include <mutex>
// #include <condition_variable>
// #include <queue>

// template<typename T>
// class ThreadSafeQueue 
// { 
// public:
// 	void push(const T& value);  
// 	bool empty() const;   
// 	bool try_pop(T& popped_value);   
// 	void wait_and_pop(T& popped_value);

// private:
// 	std::queue<T> the_queue; 
// 	mutable std::mutex the_mutex;   
// 	std::condition_variable the_condition_variable; 
// };

// template<typename T>
// bool ThreadSafeQueue<T>::empty() const
// {
// 	std::lock_guard<std::mutex> lock(the_mutex);

// 	return the_queue.empty();
// }

// template<typename T>
// void ThreadSafeQueue<T>::push(const T& value)
// {
// 	std::unique_lock<std::mutex> lock(the_mutex);

// 	the_queue.push(value);

// 	lock.unlock(); //Why?
// 	the_condition_variable.notify_one();
// }

// template<typename T>
// bool ThreadSafeQueue<T>::try_pop(T& popped_value)
// {
// 	std::lock_guard<std::mutex> lock(the_mutex);

// 	if (the_queue.empty())
// 	{
// 		return false;
// 	}

// 	popped_value = the_queue.front();
// 	the_queue.pop();

// 	return true;
// }

// template<typename T>
// void ThreadSafeQueue<T>::wait_and_pop(T& popped_value)
// {
// 	std::unique_lock<std::mutex> lock(the_mutex);

// 	while (the_queue.empty())
// 	{
// 		the_condition_variable.wait(lock);
// 	}

// 	popped_value = the_queue.front();
// 	the_queue.pop();
// }
