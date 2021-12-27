#pragma once
#include "stdafx.h"
#include "resource.h"
#define propString L"MyCefClient"
//#define REQUIRE_UI_THREAD()   assert(CefCurrentlyOn(TID_UI));
class MyClient : public CefClient, public CefLifeSpanHandler
{
public:
    HWND hWnd;
    //HINSTANCE hInstance;
    bool loadFlag;
    //DWORD dw;

    ATOM MyRegisterClass(HINSTANCE hInstance)
    {
        WNDCLASSEXW wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32CEFTEST));
        wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_WIN32CEFTEST);
        wcex.lpszClassName = L"main";
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        return RegisterClassExW(&wcex);
    }
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_CREATE:
        {
        }
        break;
        case WM_SIZE:
        {
            
            MyClient* pMyCient = (MyClient*)GetProp(hWnd, propString);

            if (!(pMyCient && pMyCient->m_browser)) { break; }

            //CefRefPtr<MyClient> client = pMyCient->myClient;
            //CefRefPtr<CefBrowser> browser = client->GetBrowser();
            CefWindowHandle hWndBrowser = pMyCient->m_browser->GetHost()->GetWindowHandle();
            RECT rect;

            GetClientRect(hWnd, &rect);
            MoveWindow(hWndBrowser, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);

            pMyCient = nullptr;
        }
        break;
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
            switch (wmId)
            {
            case IDM_ABOUT:

                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: �ڴ˴�����ʹ�� hdc ���κλ�ͼ����...
            EndPaint(hWnd, &ps);
        }
        break;
        /*
        case WM_CLOSE:
        {
            //browser = client->GetBrowser();
            //browser->GetHost()->CloseBrowser(true);
            //CefShutdown();
            if (client.get() && !client->IsClosing()) {
                //CefRefPtr<CefBrowser> pBrowser = client->GetBrowser();
                //if (pBrowser.get())
                    //pBrowser->GetHost()->CloseBrowser(true);
                //MessageBox(NULL, L"WM_CLOSE", TEXT("tidhi"), MB_OK);
                //client.release();
                DestroyWindow(hWnd);
            }
        }
        break;*/
        case WM_DESTROY:
        {
            //MessageBox(NULL, L"WM_DESTROY", TEXT("MyClient"), MB_OK);   
            //client = nullptr;
            //PostQuitMessage(0);
        }
        break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }
    HWND CreateWnd(HINSTANCE hInstance) {
        MyRegisterClass(hInstance);
        hWnd = CreateWindowW(L"main", L"test", WS_OVERLAPPEDWINDOW,
            0, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);
        WCHAR c[20] = { 0 };
        _itow((DWORD)this, c, 10);
        //MessageBox(NULL, c, TEXT("tidhi"), MB_OK);
        SetProp(hWnd, propString, this);
        ShowWindow(hWnd, SW_SHOWNORMAL);
        return hWnd;
    }
    HWND CreateWnd(HINSTANCE hInstance, HWND hWndParent) {
        MyRegisterClass(hInstance);
        RECT rect;
        GetClientRect(hWndParent, &rect);
        CefRect cefRect(0, 0, rect.right - rect.left, rect.bottom - rect.top);
        hWnd = CreateWindowW(L"main", L"test", WS_CHILD,
            rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, hWndParent, nullptr, hInstance, nullptr);
        SetProp(hWnd, propString, this);
        ShowWindow(hWnd, SW_SHOWNORMAL);
        return hWnd;
    }
    void CreateBrowser(HWND hWndParent, CefString target_url) {
        //CEF_REQUIRE_UI_THREAD();
        CefWindowInfo window_info;
        RECT rect;
        GetClientRect(hWndParent, &rect);
        CefRect cefRect(0, 0, rect.right - rect.left, rect.bottom - rect.top);
        window_info.SetAsChild(hWndParent, cefRect);
        //window_info.SetAsPopup(NULL, "mycef");

        CefBrowserSettings browser_settings;

        //CefString url = target_url;
        CefBrowserHost::CreateBrowser(window_info, this, target_url, browser_settings, nullptr, nullptr);
        
    }
    void NewUrl(CefString target_url) {
        m_browser->GetMainFrame()->LoadURL(target_url);
        
    }
public:
    CefRefPtr<CefBrowser> m_browser;
    //bool m_bIsClose = false;
    CefRefPtr<CefBrowser> GetBrowser() {
        return m_browser;
    }
    //bool IsClosing() const { return m_bIsClose; }
public:

    virtual ~MyClient() { }
    /*
public:
    virtual bool DoClose(CefRefPtr<CefBrowser> browser) override {
        REQUIRE_UI_THREAD();
        // Closing the main window requires special handling. See the DoClose()
        // documentation in the CEF header for a detailed destription of this
        // process.

        m_bIsClose = true;
        // Allow the close. For windowed browsers this will result in the OS close
        // event being sent.
        return false;
    }*/
    virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                CefRefPtr<CefFrame> frame,
                const CefString& target_url,
                const CefString& target_frame_name,
                WindowOpenDisposition target_disposition,
                bool user_gesture,
                const CefPopupFeatures& popupFeatures,
                CefWindowInfo& windowInfo,
                CefRefPtr<CefClient>& client,
                CefBrowserSettings& settings,
                CefRefPtr<CefDictionaryValue>& extra_info,
                bool* no_javascript_access) override
    {
        if (!target_url.empty())

        {

            switch (target_disposition)
            {
            case WOD_NEW_FOREGROUND_TAB:
            case WOD_NEW_BACKGROUND_TAB:
            case WOD_NEW_POPUP:
            case WOD_NEW_WINDOW:
                browser->GetMainFrame()->LoadURL(target_url);
                return true; //ֹͣ����
            }

        }
        return true;
    }
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override {
        m_browser = browser;
        //pMyClient->loadFlag = true;
        //PostMessage(hWnd, WM_SIZE, NULL, NULL);
        //MessageBox(NULL, L"CreateBrowser", TEXT("tidhi"), MB_OK);
    }

public:
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override
    {
        return this;
    }
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override
    {
        CEF_REQUIRE_UI_THREAD();
        
        //m_browser.release();
        m_browser = nullptr;
        WritePrivateProfileStringW(L"test", L"OnBeforeClose", L"����", L"D:\\wlx_cef.ini");

    }
private:

    IMPLEMENT_REFCOUNTING(MyClient);
};