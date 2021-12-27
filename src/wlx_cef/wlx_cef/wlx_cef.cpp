// Win32CEFTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "wlx_cef.h"
#include "MyCef/MyClient.h"
#include "MyCef/MyApp.h"
#define MAX_LOADSTRING 100
HINSTANCE hInst;
HMODULE FLibHandle = 0;

#define parsefunction "ext=\"HTML\" | ext=\"MP4\""
#define countof(str) (sizeof(str)/sizeof(str[0]))

ATOM MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
// 此代码模块中包含的函数的前向声明: 

BOOL APIENTRY DllMain(HANDLE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
                       
        hInst = (HINSTANCE)hModule;

    }
        break;
    case DLL_PROCESS_DETACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}
char* strlcpy(char* p, char* p2, int maxlen)
{
    if ((int)strlen(p2) >= maxlen) {
        strncpy(p, p2, maxlen);
        p[maxlen] = 0;
    }
    else
        strcpy(p, p2);
    return p;
}
WCHAR* awlcopy(WCHAR* outname, char* inname, int maxlen)
{
    if (inname) {
        int result=MultiByteToWideChar(CP_ACP, 0, inname, -1, outname, maxlen);
        int err = GetLastError();
        //outname[maxlen] = 0;
        return outname;
    }
    else
        return NULL;
}
void __stdcall ListGetDetectString(char* DetectString, int maxlen)
{
    strlcpy(DetectString, parsefunction, maxlen);
}

HWND __stdcall ListLoad(HWND ParentWin, char* FileToLoad, int ShowFlags)
{       
    CefMainArgs main_args(hInst);
    CefRefPtr<MyApp> myApp;
    CefRefPtr<MyClient> pMyClient;
    myApp = CefRefPtr<MyApp>(new MyApp());

    CefSettings settings;
    settings.no_sandbox = true;
    // Specify the path for the sub-process executable.
    // 指定子进程的执行路径
    CefString(&settings.browser_subprocess_path).FromASCII("D:\\APPP\\TotalCMD_x86\\Plugins\\src\\wlx_cef\\Debug\\subprocess.exe");
    settings.multi_threaded_message_loop = true;
    //MessageBox(NULL, settings.browser_subprocess_path.str, TEXT("ListLoad"), MB_OK);
    bool flag = CefInitialize(main_args, settings, myApp.get(), nullptr);
    


    WCHAR FileToLoadW[1024] = {'\0'};
    awlcopy(FileToLoadW, FileToLoad, 1024);

    pMyClient = CefRefPtr<MyClient>(new MyClient());
    HWND hWnd = pMyClient->CreateWnd(hInst, ParentWin); //hWndDll
    pMyClient->CreateBrowser(hWnd, CefString(FileToLoadW));
    SetProp(hWnd,L"MyCefApp", (HANDLE)myApp.get());
    WCHAR c[20] = { 0 };
    _itow((DWORD)hWnd, c, 10);
    //MessageBox(NULL, FileToLoadW, TEXT("ListLoad"), MB_OK);
    return hWnd;
}
int __stdcall ListLoadNext(HWND ParentWin, HWND ListWin, char* FileToLoad, int ShowFlags) {
    CefRefPtr<MyClient> pMyClient=(MyClient *)GetProp(ListWin,L"MyCefClient");
    //MessageBox(NULL, TEXT("ListLoadNext"), TEXT("ListLoadNext"), MB_OK);
    pMyClient->NewUrl(CefString(L"https://blog.csdn.net"));
    return LISTPLUGIN_OK;
}
void __stdcall ListCloseWindow(HWND ListWin)
{
    WritePrivateProfileStringW(L"test", L"ListCloseWindow", L"经过", L"D:\\wlx_cef.ini");
    //MessageBox(NULL, L"ListCloseWindow", TEXT("ListCloseWindow"), MB_OK);
        
    DestroyWindow(ListWin);
    //CefRefPtr<CefBrowser> pBrowser = pMyClient->GetBrowser();
    WCHAR c[20] = { 0 };
    //_itow((HANDLE)pBrowser, c, 10);
    //Sleep(5000);
    //if (pBrowser) {
        //WritePrivateProfileStringW(L"test", L"pBrowser存在", L"经过", L"D:\\wlx_cef.ini"); 
    //}
    //CefQuitMessageLoop();
    //CefShutdown();
    return;
}
