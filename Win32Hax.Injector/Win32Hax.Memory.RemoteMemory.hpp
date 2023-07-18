#pragma once
#include "pch.h"

namespace Win32Hax::Memory
{
class CRemoteMemory
{
private:
	LPVOID m_pointer = nullptr;
	const HANDLE m_hProcess = nullptr;
public:
	CRemoteMemory(const HANDLE hProcess);
	~CRemoteMemory();
	template<typename string_type>
	CRemoteMemory& write_string(const string_type str);
	LPVOID get() const;
};

template<typename string_type>
CRemoteMemory& CRemoteMemory::write_string(const string_type str)
{
	if (NULL == m_hProcess)
	{
		throw std::runtime_error("m_hProcess is NULL");
	}

	// 如果有资源先释放
	if(m_pointer)
	{
		::VirtualFreeEx(m_hProcess, m_pointer, 0, MEM_RELEASE);
	}
	

	LPVOID pPathRemote = ::VirtualAllocEx(m_hProcess, nullptr, (str.length() + 1) * sizeof(string_type::value_type), MEM_COMMIT, PAGE_READWRITE);

	if (nullptr == pPathRemote)
	{
		throw std::runtime_error(std::to_string(::GetLastError()));
	}

	if (::WriteProcessMemory(m_hProcess, pPathRemote, str.c_str(), (str.length() + 1) * sizeof(string_type::value_type), nullptr))
	{
		m_pointer = pPathRemote;
		return *this;
	}
	else
	{
		::VirtualFreeEx(m_hProcess, pPathRemote, 0, MEM_RELEASE);
		throw std::runtime_error(std::to_string(::GetLastError()));
	}
}
}
