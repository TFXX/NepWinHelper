#pragma once

#include "pch.h"

#ifdef UNICODE
#define LOAD_LIBRARY_NAME "LoadLibraryW"
#else
#define LOAD_LIBRARY_NAME "LoadLibraryA"
#endif

/**
	@brief 通过CreateRemoteThread方法注入Dll
	@param dwProcessID 待注入的进程ID
	@param szFileName 待注入的文件路径
**/
bool create_remote_thread_inject(DWORD dwProcessID, const TCHAR *szFileName);