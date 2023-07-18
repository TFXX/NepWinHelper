#pragma once
#include "pch.h"
#include "Win32Hax.Injector.hpp"

namespace Win32Hax::Injector
{
/*
class CCrtInjector : public CInjector
{
private:
	CInjector &on_inject(const DWORD dwProcessId);
};

class CCrtFactory : public CFactory
{
public:
	std::unique_ptr<CInjector> static create();
};
*/

class CCrtInjector : public CInjector
{
public:
	CInjector& inject(const HANDLE hProcess);
};

}
