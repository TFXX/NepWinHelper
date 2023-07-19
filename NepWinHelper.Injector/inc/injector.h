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
	@brief ͨ��CreateRemoteThread����ע��Dll
	@param dwProcessID ��ע��Ľ���ID
	@param szFileName ��ע����ļ�·��
	@return �ɹ�����true; ʧ�ܷ���false, ����GetLastError()�鿴ʧ��ԭ��
**/
bool create_remote_thread_inject(const DWORD dwProcessID, const TCHAR *szFileName);

/**
	@brief ͨ���߳�APC����עDll
	@param dwProcessID ��ע��Ľ���ID
	@param szFileName ��ע����ļ�·��
	@return �ɹ�����true; ʧ�ܷ���false, ����GetLastError()�鿴ʧ��ԭ��
**/
bool apc_inject(const DWORD dwProcessID, const TCHAR *szFileName);


/*
	@brief �����ڴ浽ָ������
	@param hProcess ָ�����̵ľ��
	@param pSrc �ڴ����ʼ��ַ
	@param SizeOfByte �ڴ���ֽڳ���
	@return �ڴ渱�������ָ�����̵ĵ�ַ
*/
void *_copy_memory_to_process(const HANDLE hProcess, const TCHAR *pSrc, const SIZE_T SizeOfByte);

FARPROC _get_func_from_module(const TCHAR *szModuleName, const char *szFuncName);

THREAD_ID_ARRAY *_enum_thread_from_process(const DWORD dwProcessID);
void _delete_thread_id_array(THREAD_ID_ARRAY *pThreadIDs);