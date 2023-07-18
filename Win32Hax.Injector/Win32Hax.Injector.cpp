#include "pch.h"
#include "Win32Hax.Injector.hpp"
/*
Win32Hax::Injector::CInjectorV2&Win32Hax::Injector::CInjectorV2::operator<<(const HANDLE hFile)
{
	m_list_hFiles.emplace_front(hFile);
	return *this;
}


Win32Hax::Injector::CInjectorV2& Win32Hax::Injector::CInjectorV2::operator>>(const DWORD dwProcessId)
{
	this -> inject(dwProcessId);
	return *this;
}

Win32Hax::Injector::CInjectorV2& Win32Hax::Injector::CInjectorV2::inject(const DWORD dwProcessId)
{
	// 打开进程
	m_hTargetProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (nullptr == m_hTargetProcess)
	{
		throw std::runtime_error(std::to_string(GetLastError()));;
	}

	// 读取Dll路径
	get_file_final_path(m_list_hFiles.cbegin(), m_list_hFiles.cend(), std::front_inserter(m_list_finalPath));

	this -> on_inject(dwProcessId);

	// 释放进程
	::CloseHandle(m_hTargetProcess);

	return *this;
}
*/

Win32Hax::Injector::CInjector::CInjector(const HANDLE hFile)
	: m_hFile(hFile)
{
	if(nullptr == hFile)
	{
		throw std::runtime_error("hFile is NULL");
	}
}

Win32Hax::Injector::CInjector &Win32Hax::Injector::CInjector::load(const HANDLE hFile)
{
	if (nullptr == hFile)
	{
		throw std::runtime_error("hFile is NULL");
	}

	m_hFile = hFile;
}

Win32Hax::Injector::CInjector& Win32Hax::Injector::CInjector::inject(const DWORD dwProcessId)
{
	if (nullptr == m_hFile)
	{
		throw std::runtime_error("hFile is NULL");
	}

	// 打开进程
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (nullptr == hProcess)
	{
		throw std::runtime_error(std::to_string(::GetLastError()));;
	}

	return this -> inject(hProcess);
}