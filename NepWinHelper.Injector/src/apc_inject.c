#include "..\inc\pch.h"
#include "..\inc\injector.h"

bool apc_inject(DWORD dwProcessID, const TCHAR *szFileName)
{
	if (NULL == szFileName)
	{
		return false;
	}

	// 打开进程
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, dwProcessID);

	if (NULL == hProcess || INVALID_HANDLE_VALUE == hProcess)
	{
		return false;
	}

	// 为Dll路径拷贝到待注入进程的内存空间内
	SIZE_T dwFileNameSizeOfByte = (_tcslen(szFileName) + 1) * sizeof(TCHAR);
	void *szFileNameRemote = _copy_memory_to_process(hProcess, szFileName, dwFileNameSizeOfByte);

	if (NULL == szFileNameRemote)
	{
		return false;
	}

	// 获取LoadLibrary函数在Kernel32.dll中的地址
	PAPCFUNC fnLoadLibrary = (PAPCFUNC)_get_func_from_module(TEXT("Kernel32"), LOAD_LIBRARY_NAME);

	if (NULL == fnLoadLibrary)
	{
		return false;
	}

	// 枚举进程中的所有线程
	_THREAD_ID_ARRAY *pThreadIDs = _enum_thread_from_process(dwProcessID);
	if (NULL == pThreadIDs)
	{
		return false;
	}
	
	// 遍历线程，将LoadLibrary插入线程APC队列
	for (size_t i = 0; i < pThreadIDs -> length; i++)
	{
		HANDLE hThread = OpenThread(THREAD_SET_CONTEXT, FALSE, pThreadIDs -> pArray[i]);
		if (NULL == hThread && INVALID_HANDLE_VALUE == hThread)
		{
			;
		}
		
		if (FALSE == QueueUserAPC2(fnLoadLibrary, hThread, (ULONG_PTR)szFileNameRemote, QUEUE_USER_APC_FLAGS_NONE))
		{
			;
		}

		CloseHandle(hThread);
	}


	// 释放内存
	_delete_thread_id_array(pThreadIDs);
	CloseHandle(hProcess);
	return true;
}