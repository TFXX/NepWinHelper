#pragma once
#include "pch.h"

namespace Win32Hax::Memory
{

/**
*	@brief ÷∏’Î¡¥
**/
template<typename T>
class CPointerChain
{
public:
	CPointerChain &operator+(const size_t offset);
	const T
private:
	std::list<size_t> m_offsetList;
};

}