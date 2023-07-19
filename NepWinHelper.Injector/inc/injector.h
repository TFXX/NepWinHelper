#pragma once

#include "pch.h"

#ifdef UNICODE
#define LOAD_LIBRARY_NAME "LoadLibraryW"
#else
#define LOAD_LIBRARY_NAME "LoadLibraryA"
#endif

typedef struct 
{
	DWORD *pArray;
	size_t length;
} THREAD_ID_ARRAY;

/**
	@brief 通过CreateRemoteThread方法注入Dll
	@param dwProcessID 待注入的进程ID
	@param szFileName 待注入的文件路径
	@return 成功返回true; 失败返回false, 调用GetLastError()查看失败原因
**/
bool create_remote_thread_inject(const DWORD dwProcessID, const TCHAR *szFileName);

/**
	@brief 通过线程APC队列注Dll
	@param dwProcessID 待注入的进程ID
	@param szFileName 待注入的文件路径
	@return 成功返回true; 失败返回false, 调用GetLastError()查看失败原因
**/
bool apc_inject(const DWORD dwProcessID, const TCHAR *szFileName);


/*
	@brief 拷贝内存到指定进程
	@param hProcess 指定进程的句柄
	@param pSrc 内存的起始地址
	@param SizeOfByte 内存的字节长度
	@return 内存副本相对于指定进程的地址
*/
void *_copy_memory_to_process(const HANDLE hProcess, const TCHAR *pSrc, const SIZE_T SizeOfByte);

FARPROC _get_func_from_module(const TCHAR *szModuleName, const char *szFuncName);

THREAD_ID_ARRAY *_enum_thread_from_process(const DWORD dwProcessID);
void _delete_thread_id_array(THREAD_ID_ARRAY *pThreadIDs);