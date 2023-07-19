#include "..\inc\pch.h"
#include "..\inc\injector.h"

void *_copy_memory_to_process(const HANDLE hProcess, const TCHAR *pSrc, const SIZE_T SizeOfByte)
{
	// ����ռ�
	void *pDst = VirtualAllocEx(hProcess, NULL, SizeOfByte, MEM_COMMIT, PAGE_READWRITE);

	if (NULL == pDst)
	{
		return NULL;
	}

	// �����ڴ�
	WriteProcessMemory(hProcess, pDst, pSrc, SizeOfByte, NULL);
	
	return pDst; 
}

FARPROC _get_func_from_module(const TCHAR *szModuleName, const char *szFuncName)
{
	// ��ȡģ��
	HMODULE hModule = NULL;

	if(FALSE == GetModuleHandleEx(0, szModuleName, &hModule) || NULL == hModule)
	{
		return NULL;
	}

	// ��ȡ������ģ���еĵ�ַ
	FARPROC pFunc = GetProcAddress(hModule, szFuncName);

	CloseHandle(hModule);

	return pFunc;
}

THREAD_ID_ARRAY* _enum_thread_from_process(const DWORD dwProcessID)
{
	
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessID);

	DWORD* aThreadIDs = calloc(2048, sizeof(DWORD));
	if (NULL == aThreadIDs)
	{
		return NULL;
	}

	THREAD_ID_ARRAY *pThreadIDs = (THREAD_ID_ARRAY *)malloc(sizeof(THREAD_ID_ARRAY));
	if (NULL == pThreadIDs)
	{
		free(aThreadIDs);
		return NULL;
	}

	pThreadIDs -> pArray = aThreadIDs;
	pThreadIDs -> length = 0;

	if (NULL == hSnapShot || INVALID_HANDLE_VALUE == hSnapShot)
	{
		_delete_thread_id_array(pThreadIDs);
		return NULL;
	}

	THREADENTRY32 threadEntry32 = {
		.dwSize = sizeof(THREADENTRY32)
	};

	if (TRUE == Thread32First(hSnapShot, &threadEntry32))
	{
		do{
			if (dwProcessID == threadEntry32.th32OwnerProcessID)
			{
				pThreadIDs -> pArray[pThreadIDs -> length++] = threadEntry32.th32ThreadID;
			}
		}while(TRUE == Thread32Next(hSnapShot, &threadEntry32));
	}
	else
	{
		;
	}

	return pThreadIDs;
}

bool create_remote_thread_inject(DWORD dwProcessID, const TCHAR *szFileName)
{
	if (NULL == szFileName)
	{
		return false;
	}

	// �򿪽���
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, dwProcessID);

	if (NULL == hProcess || INVALID_HANDLE_VALUE == hProcess)
	{
		return false;
	}

	/*
	// ���ļ�
	HANDLE hFile = CreateFile2(szFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, NULL);
	
	if (NULL == hProcess || INVALID_HANDLE_VALUE == hProcess)
	{
		return false;
	}
	*/


	// ΪDll·����������ע����̵��ڴ�ռ���
	SIZE_T dwFileNameSizeOfByte = (_tcslen(szFileName) + 1) * sizeof(TCHAR);
	void *szFileNameRemote = _copy_memory_to_process(hProcess, szFileName, dwFileNameSizeOfByte);

	if (NULL == szFileNameRemote)
	{
		// VirtualFreeEx(hProcess, szFileNameRemote, 0, MEM_RELEASE);
		return false;
	}

	// ��ȡLoadLibrary������Kernel32.dll�еĵ�ַ
	PTHREAD_START_ROUTINE fnLoadLibrary = (PTHREAD_START_ROUTINE)_get_func_from_module(TEXT("Kernel32"), LOAD_LIBRARY_NAME);

	if (NULL == fnLoadLibrary)
	{
		return false;
	}

	// �ڴ�ע������������̣߳�����ִ��LoadLibrary����
	DWORD dwThreadID;
	HANDLE hThread = CreateRemoteThreadEx(hProcess, NULL, 0, fnLoadLibrary, szFileNameRemote, 0, NULL, &dwThreadID);
	
	if (NULL == hThread || INVALID_HANDLE_VALUE == hThread)
	{
		return false;
	}

	// �ͷ��ڴ�
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return true;
}