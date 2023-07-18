#pragma once
#include "pch.h"
#include "Win32Hax.Injector.hpp"

namespace Win32Hax::Injector
{

class CApcInjector : public CInjector
{
private:
	CInjector &inject(const HANDLE hProcess);
};

}