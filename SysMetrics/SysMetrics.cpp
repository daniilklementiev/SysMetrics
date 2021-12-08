// SysMetrics.cpp : 

#include "framework.h"
#include "SysMetrics.h"

#define MAX_LOADSTRING  100
#define CMD_NEWWINDOW   1001

struct Metrics {
    int nIndex;
    WCHAR indexConst[32];
    WCHAR description[1024];
};

Metrics metrics[] = { 
    {56,    L"SM_ARRAGE",       L"Способ упорядочивания свернутых окон"}, 
    {67,    L"SM_CLEANBOOT",    L"Режим загрузки системы"},
    {5,     L"SM_CXBORDER",     L"Размер горизонтальной рамки окна в пикселях"},
    {6,     L"SM_CYBORDER",     L"Размер вертикальной рамки окна в пикселях"},
    {2,     L"SM_CXVSCROLL",    L"Ширина вертикальной полосы прокрутки в пикселях"},
    {20,    L"SM_CYVSCROLL",    L"Высота рисунка 'стрелки' полосы прокрутки в пикселях"},
    {1,     L"SM_CYSCREEN",     L"Высота экрана основного монитора в пикселях"},
    {30,    L"SM_CXSIZE",       L"Ширина кнопки в заголовке окна или строке заголовка в пикселях"},
    {31,    L"SM_CYSIZE",       L"Высота кнопки в заголовке окна или строке заголовка в пикселях"},
    {28 ,   L"SM_CXMIN",        L"Минимальная ширина окна в пикселях."},
    {29,    L"SM_CYMIN",        L"Минимальная высота окна в пикселях"},
    {16,    L"SM_CXFULLSCREEN", L"Ширина клиентской области для полноэкранного окна на основном мониторе в пикселях"},
    {17,    L"SM_CYFULLSCREEN", L"Высота клиентской области для полноэкранного окна на основном мониторе в пикселях"}


};

HINSTANCE hInst;                               
WCHAR szTitle[MAX_LOADSTRING];                 
WCHAR szWindowClass[MAX_LOADSTRING];           
HWND mList, mDescr, mValue;
ATOM centeredWindow;
HWND window;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
DWORD   CALLBACK    CreateControls(LPVOID);
DWORD   CALLBACK    ChangeMetricsInfo(LPVOID);
DWORD   CALLBACK    NewWindow(LPVOID);
LRESULT CALLBACK    CenteredWndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    

    
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SYSMETRICS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SYSMETRICS));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SYSMETRICS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(10,20,30));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SYSMETRICS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 340, 320, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: CreateControls(&hWnd); break;
    
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            int notificationCode = HIWORD(wParam);
            if (notificationCode == CBN_SELCHANGE)
            {
                // Parse notifications:
                CreateThread(NULL, 0, ChangeMetricsInfo, NULL, 0, NULL);
                break;
            }

            // Parse the menu selections:
            switch (wmId)
            {
            case CMD_NEWWINDOW: {
                NewWindow(&hWnd);
                break;
            }
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
            
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

DWORD CALLBACK CreateControls(LPVOID params) {
    centeredWindow = 0;
    // 
    HWND hWnd = *((HWND*)params);
    mList = CreateWindowExW(0, L"Combobox", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 10, 10, 150, 300, hWnd, 0, hInst, NULL);
    
    int mSize = sizeof(metrics) / sizeof(Metrics);
    for (size_t i = 0; i < mSize; i++) {
        SendMessageW(mList, CB_ADDSTRING, 0, (LPARAM)metrics[i].indexConst);
    }

    mDescr = CreateWindowExW(0, L"Edit", L" ", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_READONLY, 10, 40, 150, 200, hWnd, 0, hInst, NULL);
    CreateWindowExW(0, L"Static", L"Metric:", WS_VISIBLE | WS_CHILD | SS_CENTER, 175, 10, 50, 20, hWnd, 0, hInst, NULL);
    mValue = CreateWindowExW(0, L"Static", L"--", WS_VISIBLE | WS_CHILD | SS_CENTER, 220, 10, 50, 20, hWnd, 0, hInst, NULL);

    CreateWindowExW(0, L"Button", L"New centered win", WS_VISIBLE | WS_CHILD, 170, 150, 150, 25, hWnd, (HMENU)CMD_NEWWINDOW, hInst, NULL);
    return 0;
}

DWORD CALLBACK ChangeMetricsInfo(LPVOID params) {
    // Triggering SEL_CHANGE notification
    
    // Get Description and place on mDescr
    int selectedIndex = SendMessageW(mList, CB_GETCURSEL, 0, 0);
    SendMessageW(mDescr, WM_SETTEXT, 0, (LPARAM)metrics[selectedIndex].description);
    // Get System Metrics
    WCHAR buff[12];
    _itow_s(GetSystemMetrics(metrics[selectedIndex].nIndex), buff, 10);
    // Set string to mVal control
    SendMessageW(mValue, WM_SETTEXT, 0, (LPARAM)buff);
    return 0;
}

DWORD CALLBACK NewWindow(LPVOID params) {
    HWND hWnd = *((HWND*)params);

    if (centeredWindow == 0)
    {
        WNDCLASSEXW wcl;
        ZeroMemory(&wcl, sizeof(wcl));
        wcl.cbSize = sizeof(WNDCLASSEXW);
        wcl.lpfnWndProc = CenteredWndProc;   
        wcl.hInstance = hInst;
        wcl.lpszClassName = L"CenteredWindow";
        wcl.hbrBackground = CreateSolidBrush(RGB(150, 200, 150));
        wcl.hCursor = LoadCursor(NULL, IDC_APPSTARTING);

        centeredWindow = RegisterClassExW(&wcl);
    }
    int x = (GetSystemMetrics(SM_CXSCREEN)) / 4;
    int y = (GetSystemMetrics(SM_CYSCREEN)) / 4;
    int xStart = ((GetSystemMetrics(SM_CXSCREEN)) - x) / 2;
    int yStart = ((GetSystemMetrics(SM_CYSCREEN)) - y) / 2;
    HWND centeredWindow = CreateWindowW(L"CenteredWindow", L"CenteredWindow", WS_OVERLAPPEDWINDOW, xStart, yStart,
        x, y,
        hWnd, 0, hInst, NULL);
    ShowWindow(centeredWindow, SW_NORMAL);
    UpdateWindow(centeredWindow);
    return 0;
}

LRESULT CALLBACK CenteredWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    default: return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}