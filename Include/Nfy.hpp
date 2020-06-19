/*
The MIT License (MIT)

Copyright (c) 2020 James Boer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#pragma once

#include <mutex>
#include <memory>
#include <vector>
#include <algorithm>

namespace Nfy
{

	class SingleThreaded
	{
	public:
		void lock() {}
		void unlock() {}
	};

	using MultiThreaded = std::mutex;

	template <typename T, class Mutex = MultiThreaded, class Alloc = std::allocator<std::weak_ptr<T>>>
	class Notifier
	{
	public:
		Notifier() {}

		void Register(std::weak_ptr<T> observer)
		{
			std::lock_guard<Mutex> lock(m_mutex);
			m_notifiers.push_back(observer);
		}

		void Unregister(std::weak_ptr<T> observer)
		{
			auto obsPtr = observer.lock();
			std::lock_guard<Mutex> lock(m_mutex);
			m_notifiers.erase(
				std::remove_if(m_notifiers.begin(), m_notifiers.end(), [obsPtr] (const auto & p)
				{
					return (p.expired() || p.lock() == obsPtr) ? true : false;
				}),
				m_notifiers.end()
			);
		}

		template <typename ... Args, typename ... Fn>
		void Notify(void(T:: * M)(Fn...), Args &&... args)
		{
			std::lock_guard<Mutex> lock(m_mutex);
			m_notifiers.erase(
				std::remove_if(m_notifiers.begin(), m_notifiers.end(), [&] (const auto & p) 
				{
					auto ptr = p.lock();
					if (ptr)
					{
						(ptr.get()->*M)(std::forward<Args>(args)...);
						return false;
					}
					return true;
				}),
				m_notifiers.end()
			);
		}

		template <typename ... Args, typename ... Fn>
		bool Notify(bool(T:: * M)(Fn...), Args &&... args)
		{
			std::lock_guard<Mutex> lock(m_mutex);
			bool retVal = false;
			m_notifiers.erase(
				std::remove_if(m_notifiers.begin(), m_notifiers.end(), [&] (const auto & p) 
				{
					auto ptr = p.lock();
					if (ptr)
					{
						if (!retVal)
							retVal = (ptr.get()->*M)(std::forward<Args>(args)...);
						return false;
					}
					return true;
				}),
				m_notifiers.end()
			);
			return retVal;
		}

	private:
		Mutex m_mutex;
		std::vector<std::weak_ptr<T>, Alloc> m_notifiers;
	};

}


