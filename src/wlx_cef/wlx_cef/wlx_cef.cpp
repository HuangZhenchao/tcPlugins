// Win32CEFTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "wlx_cef.h"
#include "MyCef/MyClient.h"
#include "MyCef/MyApp.h"

#define MAX_LOADSTRING 100
HINSTANCE hInst;
HMODULE FLibHandle = 0;


ATOM MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
// 此代码模块中包含的函数的前向声明: 
string sDetectstring;
string ModuleFilePath;
map<string, string> mLoadUrl;

//字符串分割函数
std::vector<std::string> split(std::string str, std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;//扩展字符串以方便操作
    int size = str.size();
    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

char* strlcpy(char* p, const char* p2, int maxlen)
{
    if ((int)strlen(p2) >= maxlen) {
        strncpy(p, p2, maxlen);
        p[maxlen] = 0;
    }
    else
        strcpy(p, p2);
    return p;
}

char* strlcat(char* p, const char* p2, int maxlen)
{
    return strncat(p, p2, maxlen - strlen(p) - 1);
}


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
        char ModuleFileName[MAX_PATH];
        GetModuleFileNameA((HMODULE)hModule, ModuleFileName, MAX_PATH);
        *(strrchr(ModuleFileName, '\\') + 1) = 0;
        ModuleFilePath = ModuleFileName;
        string iniPath = ModuleFilePath + "wlx_cef.ini";
        char strExtensions[1024];
        GetPrivateProfileStringA("Extensions", "ext", "HTML,TXT", strExtensions,1024, iniPath.c_str());
        
        std::vector<string> vExt = split(strExtensions, ",");
        for (auto itExt = vExt.begin(); itExt != vExt.end(); itExt++) {
            if (*itExt == "") {
                continue;
            }
            if (itExt == vExt.begin()) {
                sDetectstring = "ext=\"" + *itExt + "\" ";
            }
            else {
                sDetectstring = sDetectstring + "| ext=\"" + *itExt + "\" ";
            }
            string s = *itExt;
            transform(s.begin(), s.end(), s.begin(), ::toupper);
            char cLoadUrl[1024];
            GetPrivateProfileStringA("Extensions", s.c_str(), "", cLoadUrl, 1024, iniPath.c_str());

            
            mLoadUrl[s] = cLoadUrl;
            
        }
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

void __stdcall ListGetDetectString(char* DetectString, int maxlen)
{
    char _detectstring[1024] = {0};
    sDetectstring.copy(_detectstring, sDetectstring.length(), 0);
    strlcpy(DetectString, _detectstring, maxlen);
}

CefString GetLoadUrl(char* FileToLoad) {
    //在DLL_PROCESS_ATTACH里读取ini里的ext和LoadURL到map
    //取FileToLoad的ext，从map查找LoadURL，若为空，直接加载本地文件，否则以自建服务器来实现功能。
    char cFileExt[1024];
    strlcpy(cFileExt, FileToLoad, 1024);
    strlcpy(cFileExt, strupr(strrchr(cFileExt, '.') + 1), 1024);
    string sFileExt = cFileExt;
    string sLoadUrl;
    map<string, string>::iterator itLoadUrl = mLoadUrl.find(sFileExt);
    if (itLoadUrl != mLoadUrl.end()) {
        sLoadUrl = itLoadUrl->second;
    }
    CefString target_url;
    if (sLoadUrl == "") {
        target_url = CefString(FileToLoad);
    }
    else {
        target_url = CefString(sLoadUrl + "?" + FileToLoad);
    }
    return target_url;
}

HWND __stdcall ListLoad(HWND ParentWin, char* FileToLoad, int ShowFlags)
{
    CefString csLoadUrl = GetLoadUrl(FileToLoad); //L"http://www.baidu.com";
    CefMainArgs main_args(hInst);
    CefRefPtr<MyApp> myApp = CefRefPtr<MyApp>(new MyApp());
    CefRefPtr<MyClient> pMyClient=CefRefPtr<MyClient>(new MyClient());

    CefSettings settings;
    settings.no_sandbox = true;
    // Specify the path for the sub-process executable.
    // 指定子进程的执行路径
    CefString(&settings.browser_subprocess_path).FromString(ModuleFilePath+"subprocess.exe");
    settings.multi_threaded_message_loop = true;
    bool flag = CefInitialize(main_args, settings, myApp.get(), nullptr);
    
    HWND hWnd = pMyClient->CreateWnd(hInst, ParentWin); //hWndDll
    pMyClient->CreateBrowser(hWnd, csLoadUrl);//wlx_cef
    SetProp(hWnd,L"MyCefApp", (HANDLE)myApp.get());
    WCHAR c[20] = { 0 };
    _itow((DWORD)hWnd, c, 10);
    //MessageBox(NULL, FileToLoadW, TEXT("ListLoad"), MB_OK);
    //WritePrivateProfileStringW(L"test", L"ListLoad", csLoadUrl.c_str(), L"D:\\wlx_cef.ini");
    return hWnd;
}
int __stdcall ListLoadNext(HWND ParentWin, HWND ListWin, char* FileToLoad, int ShowFlags) {
    if (!FileToLoad) {
        return LISTPLUGIN_ERROR;
    }
    CefString csLoadUrl = GetLoadUrl(FileToLoad);
    CefRefPtr<MyClient> pMyClient=(MyClient *)GetProp(ListWin,L"MyCefClient");
    pMyClient->NewUrl(csLoadUrl);
    return LISTPLUGIN_OK;
}
void __stdcall ListCloseWindow(HWND ListWin)
{      
    DestroyWindow(ListWin);
    return;
}
