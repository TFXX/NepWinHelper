#pragma once
#include "pch.h"

namespace Win32Hax::Memory
{

template<typename T>
class CPointerChain
{
public:
	CPointerChain &operator+(const size_t offset);

	template<typename T>
	friend T *operator*(const CPointerChain<T> &pointerChain);
private:
	std::forward_list<size_t> m_pointerList;
};

template<typename T>
T *operator*(const CPointerChain<T> &pointerChain)
{
	size_t p = 0;
	for(auto iterator : m_pointerList)
	{
		p += iterator;
	}
}
}