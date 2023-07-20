#include <thread>
#include <windows.h>
#include <winuser.h>

class CDll
{
public:
    using SEnumArgs = struct
    {
        HWND hWnd;
        DWORD dwProcessID;
    };

    BOOL static CALLBACK enum_windows_proc(HWND hWnd, LPARAM lParam)
    {
        if (NULL == hWnd)
        {
            return FALSE;
        }

        SEnumArgs &enumArgs = *reinterpret_cast<SEnumArgs *>(lParam);
        DWORD dwWindowProcessID = 0;
        DWORD dwWindowThreadID = ::GetWindowThreadProcessId(hWnd, &dwWindowProcessID);
        if (0 == dwWindowProcessID || 0 == dwWindowThreadID)
        {
            return FALSE;
        }

        if (enumArgs.dwProcessID == dwWindowProcessID)
        {
            enumArgs.hWnd = hWnd;
            ::SetLastError(ERROR_SUCCESS);
            return FALSE;
        }
        return TRUE;
    }

    static HWND get_window_handle_by_process(DWORD dwPID)
    {
        SEnumArgs enumArgs = {
            .hWnd = NULL,
            .dwProcessID = dwPID
        };

        if (FALSE == ::EnumWindows(CDll::enum_windows_proc, reinterpret_cast<LPARAM>(&enumArgs)) && ERROR_SUCCESS != ::GetLastError())
        {
            return NULL;
        }

        return enumArgs.hWnd;
    }

    static void main_thread()
    {
        HWND hWnd = get_window_handle_by_process(::GetCurrentProcessId());
        if(FALSE == ::SetWindowText(hWnd, TEXT("Œ“±ªHook¡À :(")))
        {
            return ;        
        }
    }
};

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD dwReason,     // reason for calling function
    LPVOID lpvReserved )  // reserved
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:{
        std::thread dllThread(CDll::main_thread);
        dllThread.detach();
        break;
        }
    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        break;
    }
    
    return TRUE;
}