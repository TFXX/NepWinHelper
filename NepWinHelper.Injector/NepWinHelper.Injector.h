#pragma once

#include "pch.h"

#ifdef UNICODE
#define LOAD_LIBRARY_NAME "LoadLibraryW"
#else
#define LOAD_LIBRARY_NAME "LoadLibraryA"
#endif

/**
	@brief ͨ��CreateRemoteThread����ע��Dll
	@param dwProcessID ��ע��Ľ���ID
	@param szFileName ��ע����ļ�·��
**/
bool create_remote_thread_inject(DWORD dwProcessID, const TCHAR *szFileName);