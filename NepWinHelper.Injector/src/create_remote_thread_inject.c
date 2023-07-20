#include "..\inc\pch.h"
#include "..\inc\injector.h"


bool create_remote_thread_inject(DWORD dwProcessID, const TCHAR *szFileName)
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

	/*
	// 打开文件
	HANDLE hFile = CreateFile2(szFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, NULL);

	if (NULL == hProcess || INVALID_HANDLE_VALUE == hProcess)
	{
	return false;
	}
	*/


	// 为Dll路径拷贝到待注入进程的内存空间内
	SIZE_T dwFileNameSizeOfByte = (_tcslen(szFileName) + 1) * sizeof(TCHAR);
	void *szFileNameRemote = _copy_memory_to_process(hProcess, szFileName, dwFileNameSizeOfByte);

	if (NULL == szFileNameRemote)
	{
		// VirtualFreeEx(hProcess, szFileNameRemote, 0, MEM_RELEASE);
		return false;
	}

	// 获取LoadLibrary函数在Kernel32.dll中的地址
	PTHREAD_START_ROUTINE fnLoadLibrary = (PTHREAD_START_ROUTINE)_get_func_from_module(TEXT("Kernel32"), LOAD_LIBRARY_NAME);

	if (NULL == fnLoadLibrary)
	{
		return false;
	}

	// 在待注入进程中启动线程，用来执行LoadLibrary函数
	DWORD dwThreadID;
	HANDLE hThread = CreateRemoteThreadEx(hProcess, NULL, 0, fnLoadLibrary, szFileNameRemote, 0, NULL, &dwThreadID);

	if (NULL == hThread || INVALID_HANDLE_VALUE == hThread)
	{
		return false;
	}

	// 释放内存
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return true;
}