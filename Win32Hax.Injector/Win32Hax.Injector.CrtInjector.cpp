#include "pch.h"
#include "Win32Hax.Injector.CrtInjector.hpp"
#include "Win32Hax.Memory.RemoteMemory.hpp"

namespace Win32Hax::Injector
{
/*
std::unique_ptr<CInjector> CCrtFactory::create()
{
	return std::make_unique<CCrtInjector>();
}


CInjector &CCrtInjector::on_inject(const DWORD dwProcessId)
{
	// ���������ռ䲢д��ÿ��Dll�ĵ�ַ
	std::forward_list<void *> list_pPathsRemote;
	write_string_remote(get_paths_cbegin(), get_paths_cend(), std::front_inserter(list_pPathsRemote), get_target_process_handle());

	// ��ȡLoadLibrary������ַ
	HMODULE hKernel32Module = nullptr;
	PTHREAD_START_ROUTINE pfnLoadLibrary = nullptr;
	if(TRUE == ::GetModuleHandleEx(0, _T("Kernel32"), &hKernel32Module))
	{
		if(nullptr == hKernel32Module)
		{
			throw std::runtime_error("hKernel32Module is NULL");
		}
		else
		{
			pfnLoadLibrary = reinterpret_cast<PTHREAD_START_ROUTINE>(::GetProcAddress(hKernel32Module, "LoadLibraryW"));

			if(nullptr == pfnLoadLibrary)
			{
				throw std::runtime_error(std::to_string(GetLastError()));
			}
		}
	}
	else
	{
		throw std::runtime_error(std::to_string(GetLastError()));
	}

	// Crt����ÿ��Dll
	std::forward_list<HANDLE> list_hThreadsRemote;
	for (void *pPathRemote : list_pPathsRemote)
	{
		DWORD dwThreadId = 0;
		HANDLE hThreadRemote = ::CreateRemoteThreadEx(get_target_process_handle(), nullptr, 0, pfnLoadLibrary, pPathRemote, 0, nullptr, &dwThreadId);
		
		if(nullptr == hThreadRemote)
		{
			throw std::runtime_error(std::to_string(GetLastError()));
		}
	}

	// �ͷ���Դ
	for (HANDLE hThreadRemote : list_hThreadsRemote)
	{
		::CloseHandle(hThreadRemote);
	}
	::FreeLibrary(hKernel32Module);

	free_memory_remote(list_pPathsRemote.begin(), list_pPathsRemote.end(), get_target_process_handle());

	return *this;
}
*/

CInjector& CCrtInjector::inject(const HANDLE hProcess)
{
	Memory::CRemoteMemory remoteMem(hProcess);

	//  ���������ռ䲢д��Dll�ĵ�ַ
	std::wstring strFinalPath = CInjector::get_file_final_path(hProcess);
	remoteMem.write_string(strFinalPath);

	// ��ȡLoadLibrary������ַ
	HMODULE hModKernel32 = nullptr;
	PTHREAD_START_ROUTINE pfnLoadLibrary = nullptr;
	if (TRUE == ::GetModuleHandleEx(0, TEXT("Kernel32"), &hModKernel32))
	{
		if (nullptr == hModKernel32)
		{
			throw std::runtime_error("hModKernel32 is NULL");
		}
		else
		{
			pfnLoadLibrary = reinterpret_cast<PTHREAD_START_ROUTINE>(::GetProcAddress(hModKernel32, "LoadLibraryW"));

			if (nullptr == pfnLoadLibrary)
			{
				throw std::runtime_error(std::to_string(::GetLastError()));
			}
		}
	}
	else
	{
		throw std::runtime_error(std::to_string(::GetLastError()));
	}

	// Crt����Dll
	DWORD dwThreadId = 0;
	HANDLE hThreadRemote = ::CreateRemoteThreadEx(hProcess, nullptr, 0, pfnLoadLibrary, remoteMem.get(), 0, nullptr, &dwThreadId);

	// �ͷ���Դ
	::CloseHandle(hThreadRemote);
	return *this;
}

}