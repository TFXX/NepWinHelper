#pragma once
#include "pch.h"

namespace Win32Hax::Injector
{
/*
class CInjectorV2
{
public:
	template<typename OutputIterator>
	CInjectorV2& load(OutputIterator begin, OutputIterator end)
	{
		for (auto &iterator : m_list_hFiles)
		{
			m_list_hFiles.emplace_front(*iterator);
		}
		return *this;
	}

	template<typename OutputIterator>
	CInjectorV2& reload(OutputIterator begin, OutputIterator end)
	{
		m_list_hFiles.clear();
		this -> load(begin, end);
		return *this;
	}

	CInjectorV2& operator<<(const HANDLE hFile);
	CInjectorV2& operator>>(const DWORD dwProcessId);

	CInjectorV2& inject(const DWORD dwProcessId);
protected:
	std::forward_list<std::wstring>::iterator get_paths_begin();
	std::forward_list<std::wstring>::iterator get_paths_end();

	std::forward_list<std::wstring>::const_iterator get_paths_cbegin();
	std::forward_list<std::wstring>::const_iterator get_paths_cend();
	const HANDLE get_target_process_handle();

	virtual CInjectorV2& on_inject(const DWORD dwProcessId) = 0;
private:
	HANDLE m_hTargetProcess = nullptr;

	// Dll文件句柄
	std::forward_list<HANDLE> m_list_hFiles;

	// Dll文件路径
	std::forward_list<std::wstring> m_list_finalPath;
};

class CFactory{

};
*/
class CInjector
{
private:
	HANDLE m_hFile = nullptr;
public:
	CInjector() = default;
	CInjector(const	HANDLE hFile);
	CInjector &load(const HANDLE hFile);
	CInjector &inject(const DWORD dwProcessId);
	virtual CInjector &inject(const HANDLE hProcess) = 0;

	static tstring get_file_final_path(const HANDLE hFile)
	{
		if (nullptr == hFile)
		{
			throw std::runtime_error("hFile is nullptr");
		}

		TCHAR szFinalPath[MAX_PATH];
		DWORD dwRes = ::GetFinalPathNameByHandle(hFile, szFinalPath, MAX_PATH, FILE_NAME_NORMALIZED);

		if (dwRes > 0 && dwRes <= MAX_PATH)
		{
			if (::PathFileExists(szFinalPath))
			{
				return szFinalPath;
			}
		}

		throw std::runtime_error(std::to_string(::GetLastError()));
	}
};

/**
*	@brief 获取容器中的文件句柄对应的路径并存储到新的容器中
*	@return 始终返回新容器的迭代器
**/
template<typename InputIterator, typename OutputIterator>
OutputIterator get_file_final_path(InputIterator begin, InputIterator end, OutputIterator detination)
{
	TCHAR szFinalPath[MAX_PATH];
	DWORD dwRes = 0;
	for (InputIterator iterator = begin; iterator != end; iterator++)
	{
		dwRes = ::GetFinalPathNameByHandle(*iterator, szFinalPath, MAX_PATH, FILE_NAME_NORMALIZED);

		if (0 == dwRes)
		{
			throw std::runtime_error(std::to_string(GetLastError()));
		}
		else if (dwRes > MAX_PATH)
		{
			throw std::runtime_error("buffer is to small");
		}
		else
		{
			if (::PathFileExists(szFinalPath))
			{
				detination = szFinalPath;
			}
			else
			{
				throw std::runtime_error(std::to_string(GetLastError()));
			}
		}
	}

	return detination;
}

/**
*	@brief 向进程中写入字符串
**/
template<typename InputIterator, typename OutputIterator>
OutputIterator write_string_remote(InputIterator begin, InputIterator end, OutputIterator detination, const HANDLE hProcess)
{
	if(NULL == hProcess)
	{
		throw std::runtime_error("wreite_string_remote: Process Handle is NULL");
	}
	else
	{
		for (InputIterator iterator = begin; iterator != end; iterator++)
		{
			void *pPathRemote = ::VirtualAllocEx(hProcess, nullptr, (iterator -> length() + 1) * sizeof(wchar_t), MEM_COMMIT, PAGE_READWRITE);
			if (nullptr == pPathRemote)
			{
				throw std::runtime_error(std::to_string(GetLastError()));
			}
			else
			{
				if (::WriteProcessMemory(hProcess, pPathRemote, iterator -> c_str(), (iterator -> length() + 1) * sizeof(TCHAR), nullptr))
				{
					detination = pPathRemote;
				}
				else
				{
					throw std::runtime_error(std::to_string(GetLastError()));
				}
			}
		}
	}
	return detination;
}

/**
*	@brief 释放容器元素指向的内存
**/
template<typename InputIterator>
InputIterator free_memory_remote(InputIterator begin, InputIterator end, const HANDLE hProcess)
{
	if (NULL == hProcess)
	{
		throw std::runtime_error("free_memory_remote: Process Handle Is NULL");
	}
	else
	{
		for (InputIterator iterator = begin; begin != end; begin++)
		{
			if(!::VirtualFreeEx(hProcess, *begin, 0, MEM_RELEASE))
			{
				throw std::runtime_error(std::to_string(::GetLastError()));
			}
		}
	}

	return end;
}
}