/******************************************************************************
    �T�[�r�X�A�C�R��
        �T�[�r�X�A�C�R�� �\�[�X�t�@�C��
            Copyright(C) 2007 Wraith.   All rights reserved.
                                            Coded by Wraith in Jul 17, 2007.
******************************************************************************/

///////////////////////////////////////////////////////////////////////////////
//
//  �� svcicon.cpp
//      http://tricklib.com/cxx/ex/svcicon/svcicon.cpp
//
//  �� �֘A�t�@�C��
//      ���\�[�X�w�b�_�t�@�C��
//      http://tricklib.com/cxx/ex/svcicon/svcicon.rh
//      ���\�[�X�t�@�C��
//      http://tricklib.com/cxx/ex/svcicon/svcicon.rc
//      �A�v���P�[�V�����A�C�R��
//      http://tricklib.com/cxx/ex/svcicon/svcicon.ico
//      �f�t�H���g�T�[�r�X�A�C�R��
//      http://tricklib.com/cxx/ex/svcicon/svc.ico
//      UAC�I�[�o�[���C�A�C�R��
//      http://tricklib.com/cxx/ex/svcicon/miniuac.ico
//      "start"�A�C�R��
//      http://tricklib.com/cxx/ex/svcicon/start.ico
//      "pause"�A�C�R��
//      http://tricklib.com/cxx/ex/svcicon/pause.ico
//      "stop"�A�C�R��
//      http://tricklib.com/cxx/ex/svcicon/stop.ico
//      "resume"�A�C�R��
//      http://tricklib.com/cxx/ex/svcicon/resume.ico
//      "restart"�A�C�R��
//      http://tricklib.com/cxx/ex/svcicon/restart.ico
//      "����"�A�C�R��
//      http://tricklib.com/cxx/ex/svcicon/close.ico
//      "start pending"�I�[�o�[���C�A�C�R��
//      http://tricklib.com/cxx/ex/svcicon/start_pending.ico
//      "running"�I�[�o�[���C�A�C�R��
//      http://tricklib.com/cxx/ex/svcicon/running.ico
//      "pause pending"�I�[�o�[���C�A�C�R��
//      http://tricklib.com/cxx/ex/svcicon/pause_pending.ico
//      "paused"�I�[�o�[���C�A�C�R��
//      http://tricklib.com/cxx/ex/svcicon/paused.ico
//      "continue pending"�I�[�o�[���C�A�C�R��
//      http://tricklib.com/cxx/ex/svcicon/continue_pending.ico
//      "stop pending"�I�[�o�[���C�A�C�R��
//      http://tricklib.com/cxx/ex/svcicon/stop_pending.ico
//      "stopped"�I�[�o�[���C�A�C�R��
//      http://tricklib.com/cxx/ex/svcicon/stopped.ico
//      �S�t�@�C���p�b�N
//      http://tricklib.com/cxx/ex/svcicon/svcicon.zip
//
//  �� ���t�@�����X�E�T�|�[�g�y�[�W
//      http://tricklib.com/cxx/ex/svcicon/
//
//  �� ���C�Z���X���
//      http://tricklib.com/license.htm
//
//  �� �R���p�C�����@
//
//      case Borland C++
//          bcc32 -tWM -O1 -c svcicon.cpp
//          brcc32 svcicon.rc
//          ilink32 -aa svcicon.obj, svcicon.exe, , c0w32.obj import32.lib cw32mt.lib, , svcicon.res
//
//      case Visual C++
//          rc svcicon.rc
//          cl /EHsc /O1 svcicon.cpp /link /machine:IX86 /SUBSYSTEM:WINDOWS svcicon.res
//

#include <assert.h>
#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <string>
#include <map>

#if 0x0600 <= WINVER // 0x0600 <= _WIN32_WINNT
#if defined(__BORLANDC__)
#pragma warn -8017
#endif
#include <initguid.h>
#include <wincodec.h>
#if defined(__BORLANDC__)
#pragma warn .8017
#endif
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(X) (sizeof(X) /sizeof(X[0]))
#endif

#include "svcicon.rh"

#if defined(_MSC_VER)
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "KERNEL32.lib")
#pragma comment(lib, "GDI32.lib")
#pragma comment(lib, "SHELL32.lib")
#pragma comment(lib, "USER32.lib")
#pragma comment(lib, "ADVAPI32.lib")
#if 0x0600 <= WINVER
#pragma comment(lib, "OLE32.lib")
#pragma comment(lib, "WindowsCodecs")
#endif
#endif

#if defined(_MSC_VER)
#pragma warning(disable: 4996)
#endif

LPCWSTR application_name = L"svcicon";

//////////////////////////////////////////////////////////////////////////////
//
//  for DEBUG and Error Information
//

void debug_print(const char *format, ...)
{
    char buffer[1024];
    va_list arg_list;
    va_start(arg_list, format);
    wvsprintfA(buffer, format, arg_list);
    va_end(arg_list);
    OutputDebugStringA(buffer);
}
void debug_print(const wchar_t *format, ...)
{
    wchar_t buffer[1024];
    va_list arg_list;
    va_start(arg_list, format);
    wvsprintfW(buffer, format, arg_list);
    va_end(arg_list);
    OutputDebugStringW(buffer);
}

const std::string make_error_messageA(DWORD error_code = GetLastError())
{
    LPVOID  message_buffer;
    FormatMessageA
    (
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&message_buffer, 0, NULL
    );
    const std::string result = (const char *)message_buffer;
    LocalFree(message_buffer);
    return result;
}

const std::wstring make_error_messageW(DWORD error_code = GetLastError())
{
    LPVOID  message_buffer;
    FormatMessageW
    (
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&message_buffer, 0, NULL
    );
    const std::wstring result = (const wchar_t *)message_buffer;
    LocalFree(message_buffer);
    return result;
}
#if defined(_UNICODE) || defined(UNICODE)
#define make_error_message make_error_messageW
#else
#define make_error_message make_error_messageA
#endif

//////////////////////////////////////////////////////////////////////////////
//
//  Declare auto closers
//

template<class T> class demi
{
    public:
        T value;

        demi() {}
        demi(const T &X) :value(X) {}

        operator T& () { return value; }
        operator const T& () const { return value; }

        T * operator & () { return &value; }
        const T * operator & () const { return &value; }

        T & operator () () { return value; }
        const T & operator () () const { return value; }
};

#define DECLARE_AUTO_CLOSE(name, type, param, init, close_command) \
class name :public demi<type> \
{ \
  public: \
    name(param) :demi<type>(init) { } \
    ~name() \
    { \
        close(); \
    } \
    void close() \
    { \
        if (value) \
        { \
            close_command; \
            value = NULL; \
        } \
    } \
};

DECLARE_AUTO_CLOSE(AUTO_COMPATIBLE_DC, HDC, HDC dc, CreateCompatibleDC(dc), DeleteDC(value))

class AUTO_WINDOW_DC :public demi<HDC>
{
  public:
    HWND window;
    AUTO_WINDOW_DC(HWND a_window = 0) :demi<HDC>(GetWindowDC(a_window)), window(a_window) { }
    ~AUTO_WINDOW_DC()
    {
        close();
    }
    void close()
    {
        if (value)
        {
            ReleaseDC(window, value);
            value = NULL;
        }
    }
};

///////////////////////////////////////////////////////////////////////////////
//
//  �T�[�r�X�R���g���[��
//

#if defined(__BORLANDC__)
#pragma warn -8027
#endif

class service_controller
{
public:
    typedef service_controller this_type;
    volatile HANDLE thread_handle;
    SC_HANDLE handle;
    service_controller(SC_HANDLE a_handle = NULL) :handle(a_handle), thread_handle(NULL) { }
    ~service_controller()
    {
        close();
    }
    void purge_thread()
    {
        if (thread_handle)
        {
            TerminateThread(thread_handle, 0);
            thread_handle = NULL;
        }
    }
    DWORD get_status(DWORD SERVICE_STATUS::*ss_member) const
    {
        SERVICE_STATUS service_status = { 0, };
        QueryServiceStatus(handle, &service_status);
        return service_status.*ss_member;
    }
    DWORD get_status() const
    {
        return get_status(&SERVICE_STATUS::dwCurrentState);
    }
    DWORD get_controls_accepted() const
    {
        return get_status(&SERVICE_STATUS::dwControlsAccepted);
    }
    LPQUERY_SERVICE_CONFIGW get_config()
    {
        DWORD dwBytesNeeded = 4096;
        LPQUERY_SERVICE_CONFIGW config = (LPQUERY_SERVICE_CONFIGW)LocalAlloc(LPTR, dwBytesNeeded);
        if (config) 
        {
            if (!QueryServiceConfigW(handle, config, dwBytesNeeded, &dwBytesNeeded))
            {
                LocalFree(config);
                config = (LPQUERY_SERVICE_CONFIGW)LocalAlloc(LPTR, dwBytesNeeded);
                if (!QueryServiceConfigW(handle, config, dwBytesNeeded, &dwBytesNeeded))
                {
                    LocalFree(config);
                    config = NULL;
                }
            }
        }
        return config;
    }
    
    DWORD start()
    {
        LPQUERY_SERVICE_CONFIGW config = get_config();
        if
        (
            !config ||
            SERVICE_DISABLED != config->dwStartType ||
            (
                IDOK == MessageBoxW
                (
                    NULL,
                    L"���̃T�[�r�X�̓X�^�[�g�A�b�v�̎�ނ�\"����\"�ɐݒ肳��Ă��܂��B\n���̃T�[�r�X�̃X�^�[�g�A�b�v�̎�ނ�\"�蓮\"�ɕύX���Ă������ł����H",
                    application_name,
                    MB_ICONEXCLAMATION |MB_OKCANCEL
                ) &&
                ChangeServiceConfig // �{���͂��̊֐������s������G���[���b�Z�[�W��\������ׂ��B
                (
                    handle,
                    SERVICE_NO_CHANGE,
                    SERVICE_DEMAND_START,
                    SERVICE_NO_CHANGE,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL
                )
            )
        )
        {
            StartService(handle, 0, NULL);
        }
        if (config)
        {
            LocalFree(config);
        }
        return get_status();
    }
    DWORD control(DWORD dwControl)
    {
        SERVICE_STATUS service_status = { 0, };
        if (!ControlService(handle, dwControl, &service_status))
        {
            OutputDebugString(_T("ControlService: faild"));
            OutputDebugString(make_error_message().c_str());
        }
        return service_status.dwCurrentState;
    }
    DWORD control(LPCWSTR control)
    {
        if (0 == wcscmp(L"/start", control))
        {
            return start();
        }
        else
        if (0 == wcscmp(L"/pause", control))
        {
            return pause();
        }
        else
        if (0 == wcscmp(L"/stop", control))
        {
            return stop();
        }
        else
        if (0 == wcscmp(L"/resume", control))
        {
            return resume();
        }
        else
        if (0 == wcscmp(L"/restart", control))
        {
            return restart();
        }
        else
        {
            WCHAR buffer[1024];
            wsprintfW(buffer, L"�s���ȃI�v�V���� %s ���w�肳��܂����B", control);
            MessageBoxW(NULL, buffer, application_name, MB_ICONEXCLAMATION |MB_OK);
            return get_status();
        }
    }
    DWORD stop()
    {
        return control(SERVICE_CONTROL_STOP);
    }
    DWORD pause()
    {
        return control(SERVICE_CONTROL_PAUSE);
    }
    DWORD resume()
    {
        return control(SERVICE_CONTROL_CONTINUE);
    }
    DWORD restart()
    {
        purge_thread();
        thread_handle = CreateThread(NULL, 0, start_restart_proc, this, 0, NULL);
        Sleep(150);
        return get_status();
    }
    void close()
    {
        if (handle)
        {
            purge_thread();
            CloseServiceHandle(handle);
            handle = NULL;
        }
    }
    
    static DWORD WINAPI start_restart_proc(LPVOID lpParam)
    {
        return  ((this_type *)lpParam)->restart_proc();
    }
    DWORD restart_proc()
    {
        stop();
        while(SERVICE_STOPPED != get_status())
        {
            Sleep(100);
        }
        start();
        thread_handle = NULL;
        return 0;
    }
};

#if defined(__BORLANDC__)
#pragma warn .8027
#endif

///////////////////////////////////////////////////////////////////////////////
//
//  GDI
//

UINT get_ILC_color_flag(HDC dc)
{
    int color_resolution = GetDeviceCaps(dc, COLORRES);
    
    if (32 <= color_resolution)
    {
        return ILC_COLOR32;
    }
    else
    if (24 <= color_resolution)
    {
        return ILC_COLOR24;
    }
    else
    if (16 <= color_resolution)
    {
        return ILC_COLOR16;
    }
    else
    if (8 <= color_resolution)
    {
        return ILC_COLOR8;
    }
    else
    if (4 <= color_resolution)
    {
        return ILC_COLOR4;
    }
    else
    {
        return ILC_COLOR;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
//  �A�C�R��
//

HICON create_merge_icon(SIZE icon_size, HICON icon1, HICON icon2, COLORREF backgroud_color)
{
#if 0
// �A���t�@�������ރo�[�W����
    //  �C���[�W���X�g���g���ăA�C�R�����}�[�W
    HIMAGELIST source_image = ImageList_Create(icon_size.cx, icon_size.cy, get_ILC_color_flag(AUTO_WINDOW_DC()) |ILC_MASK, 2, 1);
#else
// �A���t�@�������ޖ��̉��΍�(��������)�o�[�W����
    // Windows XP �� ILC_COLOR32 ���w�肷��ƃI�[�o�[���C�C���[�W����镔�����������肷��E�E�E�Ȃ��H
    HIMAGELIST source_image = ImageList_Create(icon_size.cx, icon_size.cy, ILC_COLOR24 |ILC_MASK, 2, 1);
    ImageList_SetBkColor(source_image, backgroud_color);
    #endif
    ImageList_AddIcon(source_image, icon1);
    ImageList_AddIcon(source_image, icon2);
    HIMAGELIST merge_image = ImageList_Merge(source_image, 0, source_image, 1, 0, 0);
    HICON merge_icon = ImageList_GetIcon(merge_image, 0, ILD_TRANSPARENT);
    ImageList_Destroy(source_image);
    ImageList_Destroy(merge_image);
    return merge_icon;
}

#if 0x0600 <= WINVER

//
//  �ȉ��� InitBitmapInfo(), Create32BitHBITMAP(), IconToBitmap() �̃p�N������...
//  http://shellrevealed.com/blogs/shellblog/archive/2007/02/06/Vista-Style-Menus_2C00_-Part-1-_2D00_-Adding-icons-to-standard-menus.aspx
//

void InitBitmapInfo(__out_bcount(cbInfo) BITMAPINFO *pbmi, ULONG cbInfo, LONG cx, LONG cy, WORD bpp)
{
    ZeroMemory(pbmi, cbInfo);
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biPlanes = 1;
    pbmi->bmiHeader.biCompression = BI_RGB;

    pbmi->bmiHeader.biWidth = cx;
    pbmi->bmiHeader.biHeight = cy;
    pbmi->bmiHeader.biBitCount = bpp;
}

HRESULT Create32BitHBITMAP(HDC hdc, const SIZE *psize, __deref_opt_out void **ppvBits, __out HBITMAP* phBmp)
{
    *phBmp = NULL;

    BITMAPINFO bmi;
    InitBitmapInfo(&bmi, sizeof(bmi), psize->cx, psize->cy, 32);

    HDC hdcUsed = hdc ? hdc : GetDC(NULL);
    if (hdcUsed)
    {
        *phBmp = CreateDIBSection(hdcUsed, &bmi, DIB_RGB_COLORS, ppvBits, NULL, 0);
        if (hdc != hdcUsed)
        {
            ReleaseDC(NULL, hdcUsed);
        }
    }
    return (NULL == *phBmp) ? E_OUTOFMEMORY : S_OK;
}

HBITMAP IconToBitmap(HICON icon)
{
    HBITMAP hbmp = NULL;
    IWICImagingFactory *pFactory;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory,(LPVOID*)&pFactory);
    
    if (SUCCEEDED(hr))
    {
        IWICBitmap *pBitmap;
        hr = pFactory->CreateBitmapFromHICON(icon, &pBitmap);
        if (SUCCEEDED(hr))
        {
            UINT cx, cy;
            hr = pBitmap->GetSize(&cx, &cy);
            if (SUCCEEDED(hr))
            {
                const SIZE sizIcon = { (int)cx, -(int)cy };
                BYTE *pbBuffer;
                hr = Create32BitHBITMAP(NULL, &sizIcon, reinterpret_cast<void **>(&pbBuffer), &hbmp);
                if (SUCCEEDED(hr))
                {
                    const UINT cbStride = cx * 4;// == sizeof(ARGB);
                    const UINT cbBuffer = cy * cbStride;
                    hr = pBitmap->CopyPixels(NULL, cbStride, cbBuffer, pbBuffer);
                    if (SUCCEEDED(hr))
                    {
                    }
                    else
                    {
                        DeleteObject(hbmp);
                        hbmp = NULL;
                    }
                }
            }
            pBitmap->Release();
        }
        pFactory->Release();
    }
    return hbmp;
}
#endif


///////////////////////////////////////////////////////////////////////////////
//
//  �ʒm�A�C�R��
//

const UINT WM_NOTIFYICON_BASE = WM_APP;

BOOL control_notify_icon(DWORD message, HWND owner, UINT id, LPCWSTR caption = NULL, HICON icon = NULL)
{
    NOTIFYICONDATAW notify_icon_data =
    {
        sizeof(NOTIFYICONDATA),
        owner,
        id,
        (UINT)(((NIM_DELETE != message) ? NIF_MESSAGE: 0) | ((caption) ? NIF_TIP: 0) | ((icon) ? NIF_ICON: 0)),
        WM_NOTIFYICON_BASE +id,
        icon,
    };
    if (caption)
    {
        assert((int)wcslen(caption) < (int)ARRAY_SIZE(notify_icon_data.szTip));
        lstrcpyW(notify_icon_data.szTip, caption);
    }
    
    BOOL result;
    int try_count = 0;
    const int max_try_count = 150;
    do
    {
        result = Shell_NotifyIconW(message, &notify_icon_data);
    }
    while
    (
        FALSE == result &&
//      ERROR_TIMEOUT == GetLastError() && ��Shell_NotifyIcon()�̖߂�l�� FALSE �Ȃ̂� GetLastError() �� ERROR_SUCCESS ��Ԃ����Ƃ�����̂Ń`�F�b�N�͂��Ȃ��B
        NIM_ADD == message &&
        FALSE == (result = Shell_NotifyIconW(NIM_MODIFY, &notify_icon_data)) &&
        ++try_count < max_try_count &&
        (Sleep(1000), true)
    );
    //  cf. KB418138
    
    return result;
}


///////////////////////////////////////////////////////////////////////////////
//
//  ��DPI�Ή����}���`DPI�Ή�
//

class dpi_cache
{
private:
    UINT dpi_x;
    UINT dpi_y; 

public:
    //typedef HRESULT (WINAPI* GetDpiForMonitor_api_type)(HMONITOR hmonitor, MONITOR_DPI_TYPE dpiType, UINT * dpiX, UINT * dpiY);
    typedef HRESULT (WINAPI* GetDpiForMonitor_api_type)(HMONITOR hmonitor, int dpiType, UINT * dpiX, UINT * dpiY);
    GetDpiForMonitor_api_type pGetDpiForMonitor;

    dpi_cache()
        :dpi_x(96)
        ,dpi_y(96)
    {
        HMODULE user32_dll = GetModuleHandleW(L"user32.dll");
        typedef HRESULT (WINAPI* SetProcessDpiAwarenessInternal_api_type)(DWORD);
        auto pSetProcessDpiAwarenessInternal = (SetProcessDpiAwarenessInternal_api_type)GetProcAddress(user32_dll, "SetProcessDpiAwarenessInternal");
        if
        (
            NULL == pSetProcessDpiAwarenessInternal ||
            (
                S_OK != pSetProcessDpiAwarenessInternal(2) &&   // 2 means Process_Per_Monitor_DPI_Aware
                S_OK != pSetProcessDpiAwarenessInternal(1)      // 1 means Process_Per_Monitor_DPI_Aware
            )
        )
        {
            typedef BOOL (WINAPI* SetProcessDPIAware_api_type)(void);
            auto pSetProcessDPIAware = (SetProcessDPIAware_api_type)GetProcAddress(user32_dll, "SetProcessDPIAware");
            if (NULL != pSetProcessDPIAware)
            {
                pSetProcessDPIAware();
            }
        }
    
        HMODULE shcore_dll = LoadLibraryW(L"shcore.dll");
        if (NULL != (HMODULE)shcore_dll)
        {
            pGetDpiForMonitor = (GetDpiForMonitor_api_type)GetProcAddress(shcore_dll, "GetDpiForMonitor");
            FreeLibrary(shcore_dll);
        }
        else
        {
            pGetDpiForMonitor = nullptr;
        }
    }

    bool update(HWND hwnd) // call from WM_CREATE, WM_SETTINGCHANGE, WM_DPICHANGED
    {
        UINT old_dpi_x = dpi_x;
        UINT old_dpi_y = dpi_y;
        
        if (nullptr != pGetDpiForMonitor)
        {
            pGetDpiForMonitor
            (
                MonitorFromWindow
                (
                    hwnd,
                    MONITOR_DEFAULTTONEAREST
                ),
                0, // means MDT_EFFECTIVE_DPI
                &dpi_x,
                &dpi_y
            );
        }
        else
        {
            HDC dc = GetWindowDC(hwnd);
            dpi_x = GetDeviceCaps(dc, LOGPIXELSX);
            dpi_y = GetDeviceCaps(dc, LOGPIXELSY);
            ReleaseDC(hwnd, dc);
        }
        
        return
            old_dpi_x != dpi_x ||
            old_dpi_y != dpi_y;
    }
    
    UINT GetDpiX() const
    {
        return dpi_x;
    }
    UINT GetDpiY() const
    {
        return dpi_y;
    }
    double GetRateX() const
    {
        return dpi_x /96.0;
    }
    double GetRateY() const
    {
        return dpi_y /96.0;
    }
};


///////////////////////////////////////////////////////////////////////////////
//
//  service icon
//

namespace service_icon
{
    //
    //  system
    //
    HINSTANCE hInstance = NULL;
    LPCTSTR lpszClassName = _T("notify_icon_owner");
#if 0x0600 <= WINVER
    HMODULE uxtheme_dll = NULL;
    BOOL (*IsThemeActive)() = NULL;
    bool is_enabled_theme = false;
#endif
    const UINT TIMER_ID = 100;
    HMODULE user32_dll = NULL;
    HMODULE imageres_dll = NULL;
    
    int argc;
    LPWSTR *args;

    //
    //  notify icon
    //
    UINT WM_taskbarcreated = WM_APP;
    const UINT NOTIFYICON_ID = 10;

    //
    //  commandline
    //
    LPCWSTR this_file_name = NULL;
    LPCWSTR icon_file_name = NULL;
    LPCWSTR icon_index = NULL;
    LPCWSTR display_name = NULL;
    std::wstring service_display_name;
    std::wstring display_message;
    LPCWSTR machine_name = NULL;
    WCHAR service_name[1024];

    //
    //  icon
    //
    SIZE smallicon_size;
    HICON base_icon = NULL;
    std::map<DWORD, HICON> icon_table;
    
    //
    //  service
    //
    SC_HANDLE service_manager = NULL;
    service_controller *target_service = NULL;
    std::map<DWORD, LPCWSTR> text_table;
    DWORD prev_status = 0;
    bool is_admin_mode = false;
    
    //
    //  dpi
    //
    dpi_cache dpi;
    
    #if 0x0600 <= WINVER
    inline bool is_vista_or_later()
    {
        OSVERSIONINFO osvi;
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        GetVersionEx (&osvi);
        return 6 <= osvi.dwMajorVersion;
    }
    inline bool is_theme_active()
    {
        return IsThemeActive && IsThemeActive();
    }
#endif
    inline HICON load_icon(HMODULE module, UINT id)
    {
        return (HICON)LoadImage(module, MAKEINTRESOURCE(id), IMAGE_ICON, smallicon_size.cx, smallicon_size.cy, LR_DEFAULTCOLOR);
    }
    inline HICON remove_icon_alpha(HICON icon)
    {
        //  �A�C�R���}�[�W������ alpha ���K�؂ɏ�������Ȃ����𗘗p���āA���j���[�̏�����Ԃō�����ԂɂȂ�������
        return create_merge_icon(smallicon_size, icon, icon, 0x00DDDDDD);
    }
    inline HICON load_icon(UINT id)
    {
        switch(id)
        {
        case UAC_ICON:
            return load_icon(user32_dll, 106);
        case DO_VERSION_INFO:
            return load_icon(user32_dll, 104);
        case DO_CLOSE_ICON:
            return load_icon(imageres_dll, 5102);
        default:
            return load_icon(hInstance, id);
        }
    }

    inline HICON make_status_icon(UINT icon_id)
    {
        return create_merge_icon(smallicon_size, base_icon, load_icon(icon_id), 0x00444444);
    }
    HWND create()
    {
        //
        //  �T�[�r�X�̎擾
        //
        DWORD request_rights = SC_MANAGER_ALL_ACCESS;
        service_manager = OpenSCManagerW(machine_name, NULL, request_rights);
        is_admin_mode = service_manager;
        if (!is_admin_mode)
        {
            request_rights = STANDARD_RIGHTS_READ |SERVICE_QUERY_STATUS | SERVICE_QUERY_CONFIG;
            service_manager = OpenSCManagerW(machine_name, NULL, request_rights);
        }
        else
        {
            request_rights |= SERVICE_PAUSE_CONTINUE;
        }
        if (service_manager)
        {
            SC_HANDLE service_handle = OpenServiceW(service_manager, service_name, request_rights);
            
            if (service_handle)
            {
                target_service = new service_controller(service_handle);
                
                LPQUERY_SERVICE_CONFIGW config = target_service->get_config();
                if (config) 
                {
                    if (0 != _wcsicmp(service_name, config->lpDisplayName))
                    {
                        service_display_name = config->lpDisplayName;
                    }
                    LocalFree(config);
                }

                
            }
        }
        if (!target_service)
        {
            WCHAR buffer[1024];
            wsprintfW(buffer, L"�w�肳�ꂽ�T�[�r�X %s ���J���܂���ł����B\n%s", display_name, make_error_messageW().c_str());
            MessageBoxW(NULL, buffer, application_name, MB_ICONSTOP |MB_OK);
            return NULL;
        }
        
        //
        //  �X�e�[�^�X�\���p�e�L�X�g�̏���
        //
        text_table[SERVICE_START_PENDING]     = L"�J�n��";
        text_table[SERVICE_RUNNING]           = L"�J�n";
        text_table[SERVICE_PAUSE_PENDING]     = L"�ꎞ��~��";
        text_table[SERVICE_PAUSED]            = L"�ꎞ��~";
        text_table[SERVICE_CONTINUE_PENDING]  = L"�ĊJ��";
        text_table[SERVICE_STOP_PENDING]      = L"��~��";
        text_table[SERVICE_STOPPED]           = L"��~";
        
        //
        //  ���j���[�p�A�C�R���̐ݒ�
        //
        HMENU window_menu = LoadMenu(hInstance, MAKEINTRESOURCE(NOTIFYICON_MENU));
        HMENU notify_icon_menu = GetSubMenu(window_menu, 0);
        MENUITEMINFO mii = { sizeof(MENUITEMINFO), };
        int i = 0;
#if 0x0600 <= WINVER
        is_enabled_theme = is_vista_or_later() && is_theme_active();
#endif
        while(true)
        {
            mii.fMask = MIIM_ID |MIIM_FTYPE;
            if (!GetMenuItemInfo(notify_icon_menu, i, TRUE, &mii))
            {
                break;
            }
            
            if
            (
                MFT_SEPARATOR != (mii.fType &MFT_SEPARATOR)
            )
            {
                HICON icon = load_icon(mii.wID);
                if (!icon)
                {
                    OutputDebugString(_T("LoadImage: faild"));
                    OutputDebugString(make_error_message().c_str());
                    ++i;
                    continue;
                }
                if (!is_admin_mode)
                {
                    static HICON uac_icon = load_icon(MINI_UAC_ICON);
                    switch(mii.wID)
                    {
                    case DO_START_ICON:
                    case DO_PAUSE_ICON:
                    case DO_STOP_ICON:
                    case DO_RESUME_ICON:
                    case DO_RESTART_ICON:
                        icon = create_merge_icon(smallicon_size, icon, uac_icon, 0x00DDDDDD);
                        break;
                    default:
                        icon = remove_icon_alpha(icon);
                        break;
                    }
                }
                else
                {
                    icon = remove_icon_alpha(icon);
                }
                mii.fMask = MIIM_DATA |MIIM_BITMAP;
#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable:4302)
#   pragma warning(disable:4311)
#endif
                mii.dwItemData = (ULONG_PTR)icon; // ��mii.hbmpItem �Ńr�b�g�}�b�v���g���ꍇ�ɂ͂���Ȃ����AWM_THEMECHANGED �ɔ����ĕۑ����Ă����B
#if defined(_MSC_VER)
#   pragma warning(pop)
#endif
#if 0x0600 <= WINVER
                if (is_enabled_theme)
                {
                    mii.hbmpItem = IconToBitmap(icon);
                }
                else
#endif
                {
                    mii.hbmpItem = HBMMENU_CALLBACK;
                }
                if (!SetMenuItemInfo(notify_icon_menu, i, TRUE, &mii))
                {
                    OutputDebugString(_T("SetMenuItemInfo: faild"));
                    OutputDebugString(make_error_message().c_str());
                }
            }
            ++i;
        }
        
        //
        //  �E�B���h�E�̍쐬
        //
        return CreateWindow
        (
            lpszClassName,
            _T("�T�[�r�X�A�C�R��"),
            WS_POPUPWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            NULL,
            window_menu,
            hInstance,
            NULL
        );
    }
	void close()
	{
		if (service_manager)
		{
			CloseServiceHandle(service_manager);
			service_manager = NULL;
		}
	}

    //
    //  �����̏��i
    //
    void elevate(HWND hwnd, LPCWSTR action)
    {
        std::wstring params;
        params += action;
        params += L" \"";
        params += display_name;
        params += L"\"";
        if (icon_file_name)
        {
            params += L" \"";
            params += icon_file_name;
            params += L"\"";
            if (icon_index)
            {
                params += L" ";
                params += icon_index;
            }
        }
#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable:4302)
#   pragma warning(disable:4311)
#endif
        if (32 < (int)ShellExecuteW(hwnd, L"runas", this_file_name, params.c_str(), NULL, SW_SHOW))
        {
            //  �V�����C���X�^���X�̍쐬�ɐ�������(��)�̂ł��̃C���X�^���X�͏I������B
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            //  ���������V�����C���X�^���X�����i�ł��Ă���Ƃ͌���Ȃ��B
        }
        else
        {
            MessageBoxW
            (
                hwnd,
                L"���������i�ł��Ȃ������ׁA��������s�ł��܂���ł����B",
                application_name,
                MB_ICONEXCLAMATION |MB_OK
            );
        }
#if defined(_MSC_VER)
#   pragma warning(pop)
#endif
                //  Windows XP �ŏ��i�Ƀg���C�������Ɍ��̃��[�U�̂܂ܐV�����C���X�^���X
        //  ���N������ƂȂ������̃C���X�^���X�ł� ShellExecute ���܂Ƃ��ɓ���
        //  ���Ȃ��Ȃ�E�E�E�Ȃ��H
    }
    
    //
    //  �ʒm�A�C�R��TIP�p�e�L�X�g�̍쐬
    //
    LPCWSTR make_icon_caption(DWORD current_status)
    {
        const int max_caption_length = 64; // ARRAYA_SIZE(((NOTIFYICONDATA*)0)->szTip);
        //  �V�F���̃o�[�W�����ɂ���Ă�128�����܂ň����邱�ƂɂȂ��Ă���̂���
        //  ���ۂɂ�64�����܂ł����\������Ȃ��E�E�E�Ȃɂ��ǉ��Ńt���O���̎w��
        //  ���K�v�H
        
        std::wstring host_and_status;
        if (machine_name)
        {
            host_and_status += L"@";
            host_and_status += machine_name;
        }
        std::map<DWORD, LPCWSTR>::const_iterator text_i = text_table.find(current_status);
        if (text_i != text_table.end())
        {
            host_and_status += L" : ";
            host_and_status += text_i->second;
        }
        
        if (L"" != service_display_name)
        {
            display_message = service_display_name;
            display_message += L"(";
            display_message += service_name;
            display_message += L")";
            display_message += host_and_status;
            if (display_message.length() < max_caption_length)
            {
                return display_message.c_str();
            }
        }
        display_message = service_name +host_and_status;
        return display_message.c_str();
    }
    
    //
    //  ���j���[������Ԃ̍X�V
    //
    void update_menu_status(HWND hwnd, DWORD current_status)
    {
        HMENU window_menu = GetMenu(hwnd);
        HMENU notify_icon_menu = GetSubMenu(window_menu, 0);
        DWORD controls_accepted = target_service->get_controls_accepted();
        EnableMenuItem(notify_icon_menu, DO_START_ICON, MF_BYCOMMAND |((SERVICE_STOPPED == current_status) ?MF_ENABLED :MF_GRAYED));
        EnableMenuItem(notify_icon_menu, DO_PAUSE_ICON, MF_BYCOMMAND |((SERVICE_RUNNING == current_status && (SERVICE_ACCEPT_PAUSE_CONTINUE &controls_accepted)) ?MF_ENABLED :MF_GRAYED));
        EnableMenuItem(notify_icon_menu, DO_STOP_ICON, MF_BYCOMMAND |((SERVICE_STOPPED != current_status && (SERVICE_ACCEPT_STOP &controls_accepted)) ?MF_ENABLED :MF_GRAYED));
        EnableMenuItem(notify_icon_menu, DO_RESUME_ICON, MF_BYCOMMAND |((SERVICE_PAUSED == current_status) ?MF_ENABLED :MF_GRAYED));
        EnableMenuItem(notify_icon_menu, DO_RESTART_ICON, MF_BYCOMMAND |((SERVICE_RUNNING == current_status && (SERVICE_ACCEPT_STOP &controls_accepted)) ?MF_ENABLED :MF_GRAYED));
    }
    
    //
    //  �ʒm�A�C�R���p�A�C�R���̎擾
    //
    HICON get_status_icon(DWORD status)
    {
        std::map<DWORD, HICON>::const_iterator icon_i = icon_table.find(status);
        if (icon_i != icon_table.end())
        {
            return icon_i->second;
        }
        else
        {
            return base_icon;
        }
    }
    
    //
    //  �ʒm�A�C�R���̍X�V
    //
    void update_status(HWND hwnd, DWORD current_status)
    {
        if (prev_status != current_status)
        {
            //
            //  ���j���[������Ԃ̍X�V
            //
            update_menu_status(hwnd, current_status);
            
            //
            //  �ʒm�A�C�R���̍X�V
            //
            if (control_notify_icon(NIM_MODIFY, hwnd, NOTIFYICON_ID, make_icon_caption(current_status), get_status_icon(current_status)))
            {
                //
                //  �X�e�[�^�X�̕ۑ�
                //
                prev_status = current_status;
            }
        }
    }
    inline void control_service(HWND hwnd, LPCWSTR action)
    {
        if (is_admin_mode)
        {
            update_status(hwnd, target_service->control(action));
        }
        else
        {
            elevate(hwnd, action);
        }
    }
    
    void load_base_icon()
    {
        if (3 <= argc)
        {
            icon_file_name = args[2];
            if (4 <= argc)
            {
                icon_index = args[3];
                ExtractIconExW(icon_file_name, _wtoi(icon_index), NULL, &base_icon, 1);
            }
            else
            {
                ExtractIconExW(icon_file_name, 0, NULL, &base_icon, 1);
            }
        }
        else
        {
            base_icon = load_icon(DEFAULT_SERVICE_ICON);
        }
    }
    
    void update_icon()
    {
        smallicon_size.cx = (UINT)(GetSystemMetrics(SM_CXSMICON) *dpi.GetRateX());
        smallicon_size.cy = (UINT)(GetSystemMetrics(SM_CYSMICON) *dpi.GetRateY());

        load_base_icon();

        //
        //  �X�e�[�^�X�\���p�A�C�R���̍쐬
        //
        icon_table[SERVICE_START_PENDING]     = make_status_icon(START_PENDING_ICON);
        icon_table[SERVICE_RUNNING]           = make_status_icon(RUNNING_ICON);
        icon_table[SERVICE_PAUSE_PENDING]     = make_status_icon(PAUSE_PENDING_ICON);
        icon_table[SERVICE_PAUSED]            = make_status_icon(PAUSED_ICON);
        icon_table[SERVICE_CONTINUE_PENDING]  = make_status_icon(CONTINUE_PENDING_ICON);
        icon_table[SERVICE_STOP_PENDING]      = make_status_icon(STOP_PENDING_ICON);
        icon_table[SERVICE_STOPPED]           = make_status_icon(STOPPED_ICON);
    }

    void update_dpi(HWND hwnd)
    {
        if (dpi.update(hwnd))
        {
            update_icon();
        }
    }

#if !defined(WM_DPICHANGED)
#define WM_DPICHANGED                   0x02E0
#endif
    
    LRESULT CALLBACK window_procedure
    (
        HWND hwnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    )
    {
        switch(message)
        {
        
        case WM_CREATE:
            {
                dpi.update(hwnd);
                update_icon();
                DWORD current_status = target_service->get_status();
                if (control_notify_icon(NIM_ADD, hwnd, NOTIFYICON_ID, make_icon_caption(current_status), get_status_icon(current_status)))
                {
                    update_menu_status(hwnd, current_status);
                    prev_status = current_status;
                    SetTimer(hwnd, TIMER_ID, 300, NULL);
                }
                else
                {
                    MessageBoxW
                    (
                        hwnd,
                        L"�ʒm�A�C�R����o�^�ł��܂���ł����B",
                        application_name,
                        MB_ICONSTOP |MB_OK
                    );
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                }
            }
            break;

        case WM_SETTINGCHANGE:
            update_dpi(hwnd);
            break;
        
        case WM_DPICHANGED:
            update_dpi(hwnd);
            break;
            
        case WM_MEASUREITEM:
            {
                MEASUREITEMSTRUCT *mi = (MEASUREITEMSTRUCT*)lParam;
                if (mi != NULL)
                {
                    int diff_cx = smallicon_size.cx -GetSystemMetrics(SM_CXMENUCHECK);
                    if (0 < diff_cx)
                    {
                        mi->itemWidth += diff_cx;
                    }
                    mi->itemHeight = smallicon_size.cy;
                    return TRUE;
                    //  ���j���[�̃I�[�i�[�h���[�܂��Ɋւ��Ă͐̂��� Windows �̎d�l��������������
                    //  �Ȃ̂ŏ����̊��ł��܂Ƃ��ɓ�������҂��邱�Ƃ͖����B���̓s�x�������K�v
                    //  �E�E�E�Ƃ��������܂̂��̍��W�v�Z�������̊����ׂĂŖ��Ȃ��\������邩
                    //  �Ƃ����Ƃ��Ԃ�A�m���BMENUITEMINFO::hbmpItem �Ńr�b�g�}�b�v���g���`������
                    //  �ς܂���̂��ȒP�����A�g�B
                }
            }
            break;

        case WM_DRAWITEM:
            {
                DRAWITEMSTRUCT *di = (DRAWITEMSTRUCT*)lParam;
                if (di != NULL)
                {
                    switch(di->CtlType)
                    {
                    case ODT_MENU:
                        {
                            if (di->itemData)
                            {
                                //UINT is_checked = ODS_CHECKED == (di->itemState &ODS_CHECKED);
                                UINT is_disabled = ODS_DISABLED == (di->itemState &ODS_DISABLED);
                                DrawState
                                (
                                    di->hDC,
                                    GetSysColorBrush(COLOR_MENU),
                                    NULL,
                                    (LPARAM)(di->itemData),
                                    (WPARAM)0,
                                    di->rcItem.left +(di->rcItem.right -di->rcItem.left) -smallicon_size.cx -4,
                                    di->rcItem.top +(di->rcItem.bottom -di->rcItem.top -smallicon_size.cy) /2,
                                    smallicon_size.cx,
                                    smallicon_size.cy,
                                    DST_ICON |DSS_NORMAL |((is_disabled) ? DSS_DISABLED: 0)
                                );
                                return TRUE;
                                //  ���j���[�̃I�[�i�[�h���[�܂��Ɋւ��Ă͐̂��� Windows �̎d�l��������������
                                //  �Ȃ̂ŏ����̊��ł��܂Ƃ��ɓ�������҂��邱�Ƃ͖����B���̓s�x�������K�v
                                //  �E�E�E�Ƃ��������܂̂��̍��W�v�Z�������̊����ׂĂŖ��Ȃ��\������邩
                                //  �Ƃ����Ƃ��Ԃ�A�m���BMENUITEMINFO::hbmpItem �Ńr�b�g�}�b�v���g���`������
                                //  �ς܂���̂��ȒP�����A�g�B
                            }
                        }
                    }
                }
            }
            break;
            
        case WM_TIMER:
            update_status(hwnd, target_service->get_status());
            break;
        
        case WM_COMMAND:
        {
            assert(NULL != target_service);
            UINT wID = LOWORD(wParam);
            //HWND hwndCtl = (HWND)lParam;
            //UINT wNotifyCode = HIWORD(wParam);
            switch(wID)
            {
            case DO_START_ICON:
                control_service(hwnd, L"/start");
                break;

            case DO_PAUSE_ICON:
                control_service(hwnd, L"/pause");
                break;
                
            case DO_STOP_ICON:
                control_service(hwnd, L"/stop");
                break;
                
            case DO_RESUME_ICON:
                control_service(hwnd, L"/resume");
                break;
                
            case DO_RESTART_ICON:
                control_service(hwnd, L"/restart");
                break;
            
            //case DISPLAY_PROPERTY_ICON:
                //break;
            
            case DO_VERSION_INFO:
            {
                WCHAR filename[1024];
                GetModuleFileNameW(hInstance, filename, ARRAY_SIZE(filename) -1);
                MessageBoxW
                (
                    hwnd,
                    (
                        std::wstring(application_name) +L" " +VERSION_WSTRING +L"\r\n"
                        +L"\r\n"
                        +filename
                    ).c_str(),
                    L"�o�[�W�������",
                    MB_ICONINFORMATION |MB_OK
                );
            }
            break;
            
            case DO_CLOSE_ICON:
                PostMessage(hwnd, WM_CLOSE, 0, 0);
                break;
            }
            break;
        }
        
        case WM_NOTIFYICON_BASE +NOTIFYICON_ID:
            switch(lParam)
            {
                case WM_LBUTTONDOWN:
                case WM_RBUTTONDOWN:
                {
                    SetForegroundWindow(hwnd);
                    POINT pt;
                    GetCursorPos(&pt);
                    TrackPopupMenu(GetSubMenu(GetMenu(hwnd), 0), TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
                    PostMessage(hwnd, WM_NULL, 0, 0);
                    //  cf. KB135788
                    break;
                }
            }
            break;
            
        case WM_DESTROY:
            control_notify_icon(NIM_DELETE, hwnd, NOTIFYICON_ID);
            if (target_service)
            {
                delete target_service;
                target_service = NULL;
            }
            PostQuitMessage(0);
            break;
            
#if 0x0600 <= WINVER
        case WM_THEMECHANGED:
            {
                bool is_enabled_theme_now = is_vista_or_later() && is_theme_active();
                if (is_enabled_theme_now != is_enabled_theme)
                {
                    //
                    //  ���j���[�`����̍X�V
                    //
                    HMENU window_menu = GetMenu(hwnd);
                    HMENU notify_icon_menu = GetSubMenu(window_menu, 0);
                    MENUITEMINFO mii = { sizeof(MENUITEMINFO), };
                    int i = 0;
                    while(true)
                    {
                        mii.fMask = MIIM_FTYPE |MIIM_DATA |MIIM_BITMAP;
                        if (!GetMenuItemInfo(notify_icon_menu, i, TRUE, &mii))
                        {
                            break;
                        }
                        
                        if
                        (
                            MFT_SEPARATOR != (mii.fType &MFT_SEPARATOR) &&
                            NULL != mii.dwItemData
                        )
                        {
                            mii.fMask = MIIM_BITMAP;
                            if (is_enabled_theme_now)
                            {
                                if (NULL == mii.hbmpItem || HBMMENU_CALLBACK == mii.hbmpItem)
                                {
                                    mii.hbmpItem = IconToBitmap((HICON)mii.dwItemData);
                                }
                            }
                            else
                            {
                                if (NULL != mii.hbmpItem && HBMMENU_CALLBACK != mii.hbmpItem)
                                {
                                    DeleteObject((HBITMAP)(mii.hbmpItem));
                                }
                                mii.hbmpItem = HBMMENU_CALLBACK;
                            }
                            if (!SetMenuItemInfo(notify_icon_menu, i, TRUE, &mii))
                            {
                                OutputDebugString(_T("SetMenuItemInfo: faild"));
                                OutputDebugString(make_error_message().c_str());
                            }
                        }
                        ++i;
                    }
                    is_enabled_theme = is_enabled_theme_now;
                }
            }
            break;
#endif

        default:
            if (WM_taskbarcreated == message)
            {
                if (!control_notify_icon(NIM_ADD, hwnd, NOTIFYICON_ID, display_name, get_status_icon(prev_status)))
                {
                    //  ���̏󋵉��Ŏ��s�����ꍇ�̓T�C�����g�ɏI������B
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                }
            }
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
        return 0;
    }
    
    bool regist()
    {
        WNDCLASS this_window_class;
        
        this_window_class.style = 0;
        this_window_class.lpfnWndProc = (WNDPROC)window_procedure;
        this_window_class.cbClsExtra = 0;
        this_window_class.cbWndExtra = 0;
        this_window_class.hInstance = hInstance;
        this_window_class.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(APPLICATION_ICON));
        this_window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
        this_window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW +1);
        this_window_class.lpszMenuName = MAKEINTRESOURCE(NOTIFYICON_MENU);
        this_window_class.lpszClassName = lpszClassName;
        
        if (!RegisterClass(&this_window_class))
        {
            OutputDebugString(_T("svcicon.exe::RegisterClass: faild"));
            return false;
        }
        return true;
    }
    
#if defined(__BORLANDC__)
#pragma warn -8027
#endif
    class message_loop :public MSG
    {
    public:
        HRESULT do_loop()
        {
            while(true)
            {
                switch(GetMessage(this, NULL, 0, 0))
                {
                case 0:
                    return ERROR_SUCCESS;
                case -1:
                    return GetLastError();
                }
                TranslateMessage(this);
                DispatchMessage(this);
            }
        }    
    };
#if defined(__BORLANDC__)
#pragma warn .8027
#endif

    UINT run(HINSTANCE a_hInstance, HINSTANCE a_hPrevInstance, LPSTR a_lpCmdLine, int a_nCmdShow)
    {
        a_hPrevInstance, a_lpCmdLine, a_nCmdShow;
        
        hInstance = a_hInstance;
        
#if 0x0600 <= WINVER
        CoInitialize(NULL);
        uxtheme_dll = LoadLibraryW(L"UxTheme.dll");
        if (uxtheme_dll)
        {
            IsThemeActive = (BOOL (*)())GetProcAddress(uxtheme_dll, "IsThemeActive");
        }
#endif

        user32_dll = GetModuleHandleW(L"user32.dll");
        imageres_dll = LoadLibraryW(L"imageres.dll");

        WM_taskbarcreated = RegisterWindowMessageW(L"TaskbarCreated");
        smallicon_size.cx = GetSystemMetrics(SM_CXSMICON);
        smallicon_size.cy = GetSystemMetrics(SM_CYSMICON);
                    
        args = CommandLineToArgvW(GetCommandLineW(), &argc);
        LPWSTR action = NULL;
        
        this_file_name = args[0];

        if (2 <= argc)
        {
            if (3 <= argc && L'/' == args[1][0])
            {
                action = args[1];
                ++args;
                --argc;
            }
            
            display_name = args[1];
            lstrcpyW(service_name, display_name);
            LPCWSTR separator = wcschr(display_name, L'@');
            if (separator)
            {
                service_name[separator -display_name] = L'\0';
                machine_name = separator +1;
            }
            else
            {
                machine_name = NULL;
            }
            
            if (regist() && create())
            {
                if (action)
                {
                    assert(NULL != target_service);
                    if (is_admin_mode)
                    {
                        target_service->control(action);
                    }
                    else
                    {
                        MessageBoxW
                        (
                            NULL,
                            L"���������i�ł��Ȃ������ׁA��������s�ł��܂���ł����B",
                            application_name,
                            MB_ICONEXCLAMATION |MB_OK
                        );
                    }
                }
                message_loop().do_loop();
            }
            else
            {
                OutputDebugString(_T("CreateWindow: faild"));
                OutputDebugString(make_error_message().c_str());
            }
			close();
		}
        else
        {
            MessageBoxW
            (
                NULL,
                L"�^�X�N�o�[�̒ʒm�̈��ŊĎ�����T�[�r�X���w�肵�Ă��������B\n"
                L"\n"
                L"\n"
                L"usage: svcicon [�T�[�r�X��[@�R���s���[�^��] [�A�C�R���t�@�C���� [�A�C�R���C���f�b�N�X�l]]]\n"
                L"\n"
                L"cf. http://tricklib.com/cxx/ex/svcicon/\n",
                application_name,
                MB_ICONINFORMATION |MB_OK
            );
        }
        
        LocalFree(args);
        FreeLibrary(imageres_dll);
        
#if 0x0600 <= WINVER
        CoUninitialize();
        if (uxtheme_dll)
        {
            FreeLibrary(uxtheme_dll);
            uxtheme_dll = NULL;
        }
#endif
        
        return EXIT_SUCCESS;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  WinMain
//

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return service_icon::run(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

/******************************************************************************
    ��������                  Wraith the Trickster                  ��������
    �������� �`I'll go with heaven's advantage and fool's wisdom.�` ��������
******************************************************************************/
