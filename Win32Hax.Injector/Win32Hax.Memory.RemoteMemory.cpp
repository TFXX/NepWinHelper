#include "Win32Hax.Memory.RemoteMemory.hpp"

Win32Hax::Memory::CRemoteMemory::CRemoteMemory(const HANDLE hProcess)
	: m_hProcess(hProcess)
{

	if (nullptr == hProcess)
	{
		throw std::runtime_error("hProcess is NULL");
	}
}

Win32Hax::Memory::CRemoteMemory::~CRemoteMemory()
{
	if (nullptr != m_hProcess)
	{
		::VirtualFreeEx(m_hProcess, m_pointer, 0, MEM_RELEASE);
	}
}

LPVOID Win32Hax::Memory::CRemoteMemory::get() const
{
	return m_pointer;
}