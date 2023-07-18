#pragma once

#include <windows.h>
#include <Shlwapi.h>
#include <tchar.h>

#include <list>
#include <forward_list>
#include <string>
#include <memory>
#include <iterator>
#include <stdexcept>


#define WIN32_LEAN_AND_MEAN

#ifdef _UNICODE
constexpr bool unicodeFlag = true;

using tstring = std::wstring;
using tstring_view = std::wstring_view;

#else

constexpr bool unicodeFlag = false;
using tstring = std::string
using tstring_view = std::wtring_view;

#endif