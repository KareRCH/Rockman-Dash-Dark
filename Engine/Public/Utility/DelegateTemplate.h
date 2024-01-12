#pragma once

#include <type_traits>
#include <vector>
#include <algorithm>
#include "FastDelegate.h"

using namespace std;
using namespace fastdelegate;

//template<typename Return, typename... Args>
//class BroadcastDelegate
//{
//public:
//	typedef FastDelegate<Return(Args...)>	Delegate;
//	typedef Delegate::BaseType				DelegateBaseType;
//
//	void Add(const DelegateBaseType& Listener)
//	{
//		m_Delegates.push_back(Listener);
//		++m_iNumDelegates;
//	}
//	void Remove(const DelegateBaseType& Listener)
//	{
//		auto iter = find_if(m_Delegates.begin(), m_Delegates.end(),
//			[&Listener](DelegateBaseType& inListener) {
//				return (Listener == inListener);
//			});
//		if (iter != m_Delegates.end())
//		{
//			m_Delegates.erase(iter);
//			--m_iNumDelegates;
//		}
//	}
//	void Broadcast(Args... args)
//	{
//		for (size_t i = 0; i < m_iNumDelegates; i++)
//		{
//			if (!m_Delegates[i].empty())
//				m_Delegates[i](args);
//		}
//	}
//
//private:
//	size_t						m_iNumDelegates;
//	vector<DelegateBaseType>	m_Delegates;
//};

template<typename Delegate, typename... Args>
class MulticastDelegate
{
public:
	void Add(const Delegate& Listener)
	{
		m_Delegates.push_back(Listener);
		++m_iNumDelegates;
	}
	void Remove(const Delegate& Listener)
	{
		auto iter = find_if(m_Delegates.begin(), m_Delegates.end(),
			[&Listener](Delegate& inListener) {
				return (Listener == inListener);
			});
		if (iter != m_Delegates.end())
		{
			m_Delegates.erase(iter);
			--m_iNumDelegates;
		}
	}
	void Broadcast(Args... args)
	{
		for (size_t i = 0; i < m_iNumDelegates; i++)
		{
			if (!m_Delegates[i].empty())
				m_Delegates[i](args...);
		}
	}

	void Clear()
	{
		m_iNumDelegates = 0;
		m_Delegates.clear();
	}

private:
	size_t				m_iNumDelegates = { 0 };
	vector<Delegate>	m_Delegates;
};