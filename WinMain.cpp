#include <time.h>
#include <vector>
#include <shlobj.h>
#include <shtypes.h>
#include <windows.h>
#include <commctrl.h>

// Enabling Visual Styles
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "comctl32")

#define IDB_TOOLBAR_START 301
#define IDB_TOOLBAR_STOP 302
#define IDB_TOOLBAR_TDSET 303
#define IDB_TOOLBAR_XTSET 304
#define IDB_TOOLBAR_HELP 306

HWND m_hToolBar = nullptr;
LRESULT CreateToolBar(HWND hWnd)
{
    LPITEMIDLIST pidl;
    SHGetSpecialFolderLocation(hWnd, CSIDL_DESKTOP, &pidl);
    SHFILEINFO sfi = { 0 };

    HIMAGELIST hImageList = (HIMAGELIST)::SHGetFileInfo((LPCTSTR)pidl, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

    m_hToolBar = CreateWindow(TOOLBARCLASSNAME, NULL,
        WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, 0, 0, hWnd, (HMENU)1,
        NULL, NULL);

    ::SendMessage(m_hToolBar, TB_SETIMAGELIST, 0, (LPARAM)hImageList);

    srand((unsigned int)time(NULL));
    const UINT uButtonNums = 5;
    UINT uImgCount = ImageList_GetImageCount(hImageList);
    TBBUTTON tbButtons[uButtonNums] = {
        {MAKELONG(rand() % uImgCount, 0), IDB_TOOLBAR_START, TBSTATE_ENABLED, TBSTYLE_BUTTON | BTNS_AUTOSIZE, {0}, 0, (INT_PTR)TEXT("aaa")},
        {MAKELONG(rand() % uImgCount, 0), IDB_TOOLBAR_STOP, TBSTATE_ENABLED, TBSTYLE_BUTTON | BTNS_AUTOSIZE, {0}, 0, (INT_PTR)TEXT("bbb")},
        {MAKELONG(rand() % uImgCount, 0), IDB_TOOLBAR_TDSET, TBSTATE_ENABLED, TBSTYLE_BUTTON | BTNS_AUTOSIZE, {0}, 0, (INT_PTR)TEXT("ccc")},
        {MAKELONG(rand() % uImgCount, 0), IDB_TOOLBAR_XTSET, TBSTATE_ENABLED, TBSTYLE_BUTTON | BTNS_AUTOSIZE, {0}, 0, (INT_PTR)TEXT("ddd")},
        {MAKELONG(rand() % uImgCount, 0), IDB_TOOLBAR_HELP, TBSTATE_ENABLED, TBSTYLE_BUTTON | BTNS_AUTOSIZE, {0}, 0, (INT_PTR)TEXT("eee")} };

    ::SendMessage(m_hToolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    ::SendMessage(m_hToolBar, TB_ADDBUTTONS, (WPARAM)uButtonNums, (LPARAM)&tbButtons);

    ::SendMessage(m_hToolBar, TB_AUTOSIZE, 0, 0);
    ::SendMessage(m_hToolBar, TB_SETSTYLE, 0, TBSTYLE_FLAT | CCS_TOP);
    //::GetWindowRect(m_hToolBar, &m_rcToolBar);
    ::ShowWindow(m_hToolBar, true);
    return 0;
}
std::vector<HWND> hWndBtn;
LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
    {
        CreateToolBar(hWnd);
        for (int i = 0; i < 100; i++)
        {
            hWndBtn.push_back(CreateWindow(TEXT("BUTTON"), TEXT("ButtonN"), WS_VISIBLE | WS_CHILD, 100, 100, 300, 200, hWnd, NULL, NULL, NULL));
        }
        break;
    }
    case WM_PAINT:
    {
        break;
    }
    case WM_SIZE:
    {
        SendMessage(m_hToolBar, uMsg, wParam, lParam);
        INT width = max(0, LOWORD(lParam));
        INT height = max(0, HIWORD(lParam));

        int y = 100;
        int spacing = 30;
        HDWP hDWP = BeginDeferWindowPos((int)hWndBtn.size());
        for (int i = 0; i < hWndBtn.size(); ++i)
        {
            height = 50;
            DeferWindowPos(
                hDWP,
                hWndBtn[i],
                NULL,
                100, y,
                width - 200, height,
                SWP_NOZORDER
            );
            y = y + height + spacing;
        }
        EndDeferWindowPos(hDWP);
        break;
    }
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, BOOL) {
    WNDCLASS wc = { 0 };
    wc.lpszClassName = TEXT("wndclass");
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpfnWndProc = WinProc;
    RegisterClass(&wc);

    HWND hWnd = CreateWindow(wc.lpszClassName, TEXT("Window"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, 0, 0, hInstance, 0);
    if (hWnd == NULL) return 1;
    ShowWindow(hWnd, SW_SHOWNORMAL);
    MSG msg = { 0 };
    while (GetMessage(&msg, 0, 0, 0)) {
        DispatchMessage(&msg);
    }
    return 0;
}
