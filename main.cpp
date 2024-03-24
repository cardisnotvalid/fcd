
#include <Windows.h>
#include <cstdio>
#include <algorithm>

HFONT CreateCustomFont() {
    LOGFONT lf = { 0 };
    lf.lfHeight = 18;
    lf.lfWidth = 8;
    lf.lfWeight = 800;
    lstrcpy(lf.lfFaceName, TEXT("Cascadia Mono"));
    return CreateFontIndirect(&lf);
}

void DrawTextCenter(HWND hwnd, const char* text) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    RECT rc;
    GetClientRect(hwnd, &rc);

    SIZE size;
    HFONT hFont = CreateCustomFont();
    SelectObject(hdc, hFont);

    COLORREF bgColor = RGB(24, 24, 24);
    COLORREF textColor = RGB(255, 255, 255);

    SetBkColor(hdc, bgColor);
    SetTextColor(hdc, textColor);

    GetTextExtentPoint32(hdc, text, strlen(text), &size);
    int x = (rc.right - rc.left - size.cx) / 2;
    int y = (rc.bottom - rc.top - size.cy) / 2;

    TextOut(hdc, x, y, text, strlen(text));
    EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_KEYDOWN:
            if ((GetAsyncKeyState(VK_F4) & 0x8000) && (GetAsyncKeyState(VK_MENU) & 0x8000))
                PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char g_szClassName[] = "myWindowClass";
    
    WNDCLASSEX wc = { };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hIcon = (HICON)LoadImage(hInstance, "fcd.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(24, 24, 24));
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = (HICON)LoadImage(hInstance, "fcd.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "FCD",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    DrawTextCenter(hwnd, "This is desk cleaning app ^_-");
    BlockInput(TRUE);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
