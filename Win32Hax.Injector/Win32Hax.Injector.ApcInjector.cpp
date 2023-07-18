#include "pch.h"
#include "Win32Hax.Injector.ApcInjector.hpp"
#include "Win32Hax.Memory.RemoteMemory.hpp"

Win32Hax::Injector::CInjector &Win32Hax::Injector::CApcInjector::inject(const HANDLE hProcess)
{
	Memory::CRemoteMemory remoteMem(hProcess);

	//  向进程申请空间并写入Dll的地址
	std::wstring strFinalPath = CInjector::get_file_final_path(hProcess);
	remoteMem.write_string(strFinalPath);
	return *this;
}