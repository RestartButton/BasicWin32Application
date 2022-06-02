#ifndef UNICODE
#define UNICODE
#endif


//compile com [g++ main.cpp -lgdi32 -o run -static]

#include <Windows.h>
#include <string>

int count = 0;

wchar_t *toWideString ( int & i ) {

    char * buffer = ( char* )malloc( sizeof( int ) ); 
    itoa( i, buffer, 10 );
    const char * p_char = buffer;
    wchar_t * wtext = ( wchar_t* )malloc( sizeof( wchar_t ) * ( strlen( p_char ) ) );
    
    std::mbstowcs( wtext, p_char, strlen( p_char ) );

    free( buffer );
    LPWSTR ws = wtext;
    return wtext;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//ponto de entrada para a api do windows
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
    //cria a classe da janela
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );

    RegisterClass( &wc );
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

    if( hwnd == NULL ) return 0;

    ShowWindow( hwnd, nCmdShow );

    MSG msg = { }; //state-machine da aplicação
    //loop principal da aplicação
    while ( GetMessage( &msg, NULL, 0, 0 ) > 0 ) {
        
        TranslateMessage( &msg );
        DispatchMessage( &msg );
        
    }

    return 0;
}
//processo principal
LRESULT CALLBACK WndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
    switch( uMsg ){
        case WM_DESTROY: //finaliza aplicação
            PostQuitMessage( 0 );
            return 0;
        case WM_PAINT: //desenha tela inicial
            {
                //anula tela de pintura anterior
                InvalidateRect( hwnd, NULL, TRUE );
                //cria nova tela de pintura
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint( hwnd, &ps );
                //limpa tela
                FillRect( hdc, &ps.rcPaint, ( HBRUSH )( COLOR_WINDOW + 1 ) );
                //desenha na tela
                SetTextColor( hdc, RGB( 0, 0, 0 ) );
                SetBkMode( hdc, TRANSPARENT );
                DrawTextEx( hdc, ( LPWSTR )L"Clique ou Tecle!\n( Esc para sair )", -1, &ps.rcPaint, DT_CENTER, NULL );
                
                LPWSTR secret = ( LPWSTR )( toWideString( count ) );
                if( count >= 5 ) DrawTextEx( hdc, secret, -1, &ps.rcPaint, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE, NULL );
                
                //finaliza pintura
                EndPaint( hwnd, &ps );
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
                ZeroMemory( &logfont, sizeof( LOGFONT ) );
                logfont.lfCharSet = DEFAULT_CHARSET;
                logfont.lfHeight = 35; 
                HFONT hNewFont = CreateFontIndirect( &logfont );
                HFONT hOldFont = ( HFONT )SelectObject( hdc, hNewFont );
                //limpa tela
                FillRect( hdc, &ps.rcPaint, CreateSolidBrush( RGB( 0, 255, 255 ) ) );
                //desenha na tela
                SetTextColor( hdc, RGB( 0, 0, 0 ) );
                SetBkMode( hdc, TRANSPARENT );
                DrawTextEx( hdc, ( LPWSTR )L"Click!", -1, &ps.rcPaint, DT_SINGLELINE | DT_CENTER | DT_VCENTER, NULL );
                //desvincula fonte
                SelectObject( hdc, hOldFont );
                DeleteObject( hNewFont );
                //finaliza pintura
                EndPaint( hwnd, &ps );
                LPWSTR secret = ( LPWSTR )( toWideString( count ) );
                count++;
               
            }
            return 0;
        case WM_KEYUP:
            {
                switch( wParam ){
                    case VK_RETURN:
                        SendMessage( hwnd, WM_PAINT, wParam, lParam );
                        break;
                    case VK_ESCAPE:
                        SendMessage( hwnd, WM_DESTROY, wParam, lParam );
                        break;
                    default:
                        {
                            //anula tela de pintura anterior
                            InvalidateRect( hwnd, NULL, TRUE );
                            //cria nova tela de pintura
                            PAINTSTRUCT ps;
                            HDC hdc = BeginPaint( hwnd, &ps );
                            //cria e vincula fonte
                            LOGFONT logfont; 
                            ZeroMemory( &logfont, sizeof( LOGFONT ) );
                            logfont.lfCharSet = DEFAULT_CHARSET;
                            logfont.lfHeight = 70; 
                            HFONT hNewFont = CreateFontIndirect( &logfont );
                            HFONT hOldFont = ( HFONT )SelectObject( hdc, hNewFont );
                            //limpa tela
                            FillRect( hdc, &ps.rcPaint, CreateSolidBrush( RGB( 100, 255, 100 ) ) );
                            //desenha na tela
                            SetTextColor( hdc, RGB( 0, 0, 0 ) );
                            SetBkMode( hdc, TRANSPARENT );
                            DrawTextEx( hdc, ( LPWSTR )L"Tec!", -1, &ps.rcPaint, DT_SINGLELINE | DT_CENTER | DT_VCENTER, NULL );
                            //desvincula fonte
                            SelectObject( hdc, hOldFont );
                            DeleteObject( hNewFont );
                            //finaliza pintura
                            EndPaint( hwnd, &ps );
                            LPWSTR secret = ( LPWSTR )( toWideString( count ) );
                            count++;
                        }
                        break;
                }
            }
            return 0;
    }
    return DefWindowProc( hwnd, uMsg, wParam, lParam );
}