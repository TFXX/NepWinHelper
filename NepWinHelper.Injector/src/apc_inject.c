#include "..\inc\pch.h"
#include "..\inc\injector.h"

bool apc_inject(DWORD dwProcessID, const TCHAR *szFileName)
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

	// ΪDll·����������ע����̵��ڴ�ռ���
	SIZE_T dwFileNameSizeOfByte = (_tcslen(szFileName) + 1) * sizeof(TCHAR);
	void *szFileNameRemote = _copy_memory_to_process(hProcess, szFileName, dwFileNameSizeOfByte);

	if (NULL == szFileNameRemote)
	{
		return false;
	}

	// ö�ٽ����е������߳�
	THREAD_ID_ARRAY *pThreadIDs = _enum_thread_from_process(dwProcessID);
	if (NULL == pThreadIDs)
	{
		return false;
	}
	
	// �����̣߳���LoadLibrary�����߳�APC����

	// �ͷ��ڴ�
	_delete_thread_id_array(pThreadIDs);
	CloseHandle(hProcess);
	return true;
}