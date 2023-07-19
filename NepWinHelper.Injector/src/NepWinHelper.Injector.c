#include "..\inc\pch.h"
#include "..\inc\NepWinHelper.Injector.h"

inline void *copy_memory_to_process(const HANDLE hProcess, const TCHAR *pSrc, const SIZE_T SizeOfByte)
{
	// ����ռ�
	void *pDst = ::VirtualAllocEx(hProcess, NULL, SizeOfByte, MEM_COMMIT, PAGE_READWRITE);

	if (NULL == pDst)
	{
		return NULL;
	}

	// �����ڴ�
	::WriteProcessMemory(hProcess, pDst, pSrc, SizeOfByte, NULL);
	
	return pDst; 
}

bool create_remote_thread_inject(DWORD dwProcessID, const TCHAR *szFileName)
{
	// �򿪽���
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, NULL, dwProcessID);

	if (NULL == hProcess || INVALID_HANDLE_VALUE == hProcess)
	{
		return false;
	}

	/*
	// ���ļ�
	HANDLE hFile = ::CreateFile2(szFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, NULL);
	
	if (NULL == hProcess || INVALID_HANDLE_VALUE == hProcess)
	{
		return false;
	}
	*/


	// ΪDll·����������ע����̵��ڴ�ռ���
	SIZE_T dwFileNameSizeOfByte = (_tcslen(szFileName) + 1) * sizeof(TCHAR);
	void *szFileNameRemote = copy_memory_to_process(hProcess, szFileName, dwFileNameSizeOfByte);

	if (NULL == szFileNameRemote)
	{
		::VirtualFreeEx(hProcess, szFileNameRemote, 0, MEM_RELEASE);
		return false;
	}


	// ��ȡKernel32ģ��
	HMODULE hModule = NULL;
	;

	if(FALSE == ::GetModuleHandleEx(0, TEXT("Kernel32"), &hModule) || NULL == hModule)
	{
		return false;
	}

	// ��ȡLoadLibrary������ģ���еĵ�ַ
	FARPROC fnLoadLibrary = ::GetProcAddress(hModule, LOAD_LIBRARY_NAME);

	if (NULL == fnLoadLibrary)
	{
		return false;
	}

	// �ڴ�ע������������̣߳�����ִ��LoadLibrary����
	DWORD dwThreadID;
	HANDLE hThread = ::CreateRemoteThreadEx(hProcess, NULL, 0, (PTHREAD_START_ROUTINE)fnLoadLibrary, szFileNameRemote, 0, NULL, &dwThreadID);
	
	if (NULL == hThread || INVALID_HANDLE_VALUE == hThread)
	{
		return false;
	}

	::CloseHandle(hThread);
	::CloseHandle(hModule);
	::CloseHandle(hProcess);

	return true;
}