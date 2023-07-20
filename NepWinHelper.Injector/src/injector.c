#include "..\inc\pch.h"
#include "..\inc\injector.h"

void *_copy_memory_to_process(const HANDLE hProcess, const TCHAR *pSrc, const SIZE_T SizeOfByte)
{
	// 申请空间
	void *pDst = VirtualAllocEx(hProcess, NULL, SizeOfByte, MEM_COMMIT, PAGE_READWRITE);

	if (NULL == pDst)
	{
		return NULL;
	}

	// 拷贝内存
	WriteProcessMemory(hProcess, pDst, pSrc, SizeOfByte, NULL);
	
	return pDst; 
}

FARPROC _get_func_from_module(const TCHAR *szModuleName, const char *szFuncName)
{
	// 获取模块
	HMODULE hModule = NULL;

	if(FALSE == GetModuleHandleEx(0, szModuleName, &hModule) || NULL == hModule)
	{
		return NULL;
	}

	// 获取函数在模块中的地址
	FARPROC pFunc = GetProcAddress(hModule, szFuncName);

	CloseHandle(hModule);

	return pFunc;
}

_THREAD_ID_ARRAY *_enum_thread_from_process(const DWORD dwProcessID)
{
	
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessID);

	DWORD* aThreadIDs = calloc(2048, sizeof(DWORD));
	if (NULL == aThreadIDs)
	{
		return NULL;
	}

	_THREAD_ID_ARRAY *pThreadIDs = (_THREAD_ID_ARRAY *)malloc(sizeof(_THREAD_ID_ARRAY));
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