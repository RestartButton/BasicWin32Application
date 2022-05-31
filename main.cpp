#ifndef UNICODE
#define UNICODE
#endif


//compile com [g++ main.cpp -lgdi32]

#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//ponto de entrada para a api do windows
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //cria a classe da janela
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);
    //instancia uma janela
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Not A Virus",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,

        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,
        NULL,
        hInstance,
        NULL
    );

    if(hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { }; //state-machine da aplicação
    //loop da aplicação
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
//processo principal
LRESULT CALLBACK WndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
    switch( uMsg ){
        case WM_DESTROY: //finaliza aplicação
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: //desenha tela inicial
            {
                //cria nova tela de pintura
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint( hwnd, &ps );
                //limpa tela
                FillRect( hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));
                //desenha na tela
                SetTextColor( hdc, RGB(0, 0, 0) );
                SetBkMode( hdc, TRANSPARENT );
                DrawTextExW( hdc, (LPWSTR)L"Hello World!", -1, &ps.rcPaint, DT_SINGLELINE | DT_CENTER | DT_VCENTER, NULL );
                //finaliza pintura
                EndPaint(hwnd, &ps);
            }
            return 0;
        case WM_LBUTTONUP: //click do mouse
            {
                //anula tela de pintura anterior
                InvalidateRect( hwnd, NULL, TRUE );
                //cria nova tela de pintura
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint( hwnd, &ps );
                //cria e vincula fonte
                LOGFONT logfont; 
                ZeroMemory(&logfont, sizeof(LOGFONT));
                logfont.lfCharSet = DEFAULT_CHARSET;
                logfont.lfHeight = 70; 
                HFONT hNewFont = CreateFontIndirect(&logfont);
                HFONT hOldFont = (HFONT)SelectObject( hdc, hNewFont );
                //limpa tela
                FillRect( hdc, &ps.rcPaint, CreateSolidBrush(RGB(0, 255, 255)) );
                //desenha na tela
                SetTextColor( hdc, RGB(0, 0, 0) );
                SetBkMode( hdc, TRANSPARENT );
                DrawTextExW( hdc, (LPWSTR)L"Click!", -1, &ps.rcPaint, DT_SINGLELINE | DT_CENTER | DT_VCENTER, NULL );
                //desvincula fonte
                SelectObject( hdc, hOldFont );
                DeleteObject( hNewFont );
                //finaliza pintura
                EndPaint(hwnd, &ps);
            }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}