#include "pch.h"
#include "Win32Hax.Injector.ApcInjector.hpp"
#include "Win32Hax.Memory.RemoteMemory.hpp"

Win32Hax::Injector::CInjector &Win32Hax::Injector::CApcInjector::inject(const HANDLE hProcess)
{
	Memory::CRemoteMemory remoteMem(hProcess);

	//  ���������ռ䲢д��Dll�ĵ�ַ
	std::wstring strFinalPath = CInjector::get_file_final_path(hProcess);
	remoteMem.write_string(strFinalPath);
	return *this;
}