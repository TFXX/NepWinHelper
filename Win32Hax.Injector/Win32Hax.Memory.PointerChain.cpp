#include "Win32Hax.Memory.PointerChain.hpp"

template<typename T>
Win32Hax::Memory::CPointerChain<T>& Win32Hax::Memory::CPointerChain<T>::operator+(const size_t offset)
{
	m_pointerList.emplace_front(offset);
}