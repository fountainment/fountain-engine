#include <stdio.h>
#include <GL/gl.h>
#include <windows.h>

#define KS(r,d) keymap[(r)&FT_KEYBOARDSTATE_SIZE]=(d)

POINT mousePos;
static int keymap[FT_KEYBOARDSTATE_SIZE] = { 0 };

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC *, HGLRC *);
void DisableOpenGL(HWND, HDC, HGLRC);

void keyMapSetting()
{
	for (int i = 'A'; i <= 'Z'; i++)
		KS(i, i - 'A' + FT_A);
	for (int i = VK_F1; i <= VK_F12; i++)
		KS(i, i - VK_F1 + FT_F1);
	KS(VK_ESCAPE, FT_Esc);
	for (int i = 0; i <= 9; i++)
		KS(i, i - 0 + FT_0);
	KS(VK_RETURN, FT_Enter);
	KS(VK_SPACE, FT_Space);
	KS(VK_CONTROL, FT_Ctrl);
	//KS(VK_, FT_Alt);
	//KS(XK_, FT_Alt);
	KS(VK_LSHIFT, FT_Shift);
	KS(VK_RSHIFT, FT_Shift);
	KS(VK_UP, FT_Up);
	KS(VK_DOWN, FT_Down);
	KS(VK_LEFT, FT_Left);
	KS(VK_RIGHT, FT_Right);
}

int WINAPI
WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	fountain::basicSetting();

	keyMapSetting();

	DWORD winStyle =
	    WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX) & (~WS_SIZEBOX);

	WNDCLASSEX wcex;
	HWND hwnd;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	BOOL bQuit = FALSE;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon =
	    (HICON) LoadImage(NULL, fountain::mainWin.icon.c_str(), IMAGE_ICON,
			      0, 0, LR_LOADFROMFILE);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "fountain";
	wcex.hIconSm =
	    (HICON) LoadImage(NULL, fountain::mainWin.icon.c_str(), IMAGE_ICON,
			      0, 0, LR_LOADFROMFILE);

	if (!RegisterClassEx(&wcex))
		return 0;

	if (fountain::mainWin.isFullScreen) {
		winStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_VISIBLE;
		fountain::mainWin.w = GetSystemMetrics(SM_CXSCREEN);
		fountain::mainWin.h = GetSystemMetrics(SM_CYSCREEN);
	}
	hwnd = CreateWindowEx(0,
			      "fountain",
			      fountain::mainWin.title.c_str(),
			      winStyle,
			      CW_USEDEFAULT,
			      CW_USEDEFAULT,
			      fountain::mainWin.w,
			      fountain::mainWin.h, NULL, NULL, hInstance, NULL);

	if (fountain::mainWin.hideCursor)
		ShowCursor(false);
	ShowWindow(hwnd, nCmdShow);

	EnableOpenGL(hwnd, &hDC, &hRC);
	fountain::initAllSystem();
	fountain::gameInit();

	while (!bQuit) {

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				bQuit = TRUE;
			} else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		GetCursorPos(&mousePos);
		ScreenToClient(hwnd, &mousePos);
		fountain::sysMouse.update(mousePos.x, mousePos.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3f(1.0, 1.0, 1.0);
		glPushMatrix();
		fountain::singleFrame();
		glPopMatrix();
		SwapBuffers(hDC);
	}

	DisableOpenGL(hwnd, hDC, hRC);

	DestroyWindow(hwnd);

	return msg.wParam;
}

void EnableOpenGL(HWND hwnd, HDC * hDC, HGLRC * hRC)
{
	PIXELFORMATDESCRIPTOR pfd;

	int iFormat;

	*hDC = GetDC(hwnd);

	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags =
	    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	iFormat = ChoosePixelFormat(*hDC, &pfd);

	SetPixelFormat(*hDC, iFormat, &pfd);

	*hRC = wglCreateContext(*hDC);

	wglMakeCurrent(*hDC, *hRC);
	//wglGetProcAddress("wglSwapIntervalEXT")(0);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hwnd, hDC);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_DESTROY:
		return 0;

	case WM_KEYDOWN:
		{
			switch (wParam) {
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			default:
				fountain::sysKeyboard.
				    setState(keymap
					     [wParam & FT_KEYBOARDSTATE_SIZE],
					     1);
			}
		}
		break;

	case WM_KEYUP:
		fountain::sysKeyboard.
		    setState(keymap[wParam & FT_KEYBOARDSTATE_SIZE], 0);
		break;

	case WM_KILLFOCUS:
		fountain::sysKeyboard.clearState();
		fountain::sysMouse.clearState();
		break;

	case WM_LBUTTONDOWN:
		fountain::sysMouse.setState(1, 1);
		break;

	case WM_LBUTTONUP:
		fountain::sysMouse.setState(1, 0);
		break;

	case WM_RBUTTONDOWN:
		fountain::sysMouse.setState(3, 1);
		break;

	case WM_RBUTTONUP:
		fountain::sysMouse.setState(3, 0);
		break;

	case WM_SYSCOMMAND:
		switch (wParam) {
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}