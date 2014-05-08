
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

HINSTANCE hinst; 
#define EASYGX_WINDOW_CLASS "easygx_window"

static LRESULT CALLBACK guiwnd_wm_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}	
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, int nCmdShow) 
{ 
	WNDCLASSEX wcex;
	wcex.cbSize 			= sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)guiwnd_wm_proc;
	wcex.cbClsExtra	= 0;
	wcex.cbWndExtra	= 0;
	wcex.hInstance		= NULL;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= EASYGX_WINDOW_CLASS;
	wcex.hIconSm		= NULL;
	if(0 == RegisterClassEx(&wcex)){
		fprintf(stderr,"test_winapi | RegisterClassEx failed\n");
		return -1;
	}

	HWND hWnd;	
	hWnd = CreateWindowEx(0,EASYGX_WINDOW_CLASS,"test winapi", WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,CW_USEDEFAULT,320,240,
				NULL, NULL, NULL, NULL);

	ShowWindow(hWnd, SW_SHOW);
	
	MSG msg;
	BOOL fGotMessage;
    while ((fGotMessage = GetMessage(&msg, (HWND) NULL, 0, 0)) != 0 && fGotMessage != -1) 
    { 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }
    fprintf(stderr,"test_winapi | Quit\n");
    return msg.wParam; 
}

#ifdef __cplusplus
}
#endif
