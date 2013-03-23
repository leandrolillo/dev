/*
 * win32api.h
 *
 *  Created on: 21/11/2012
 *      Author: Lean
 */

#ifndef WIN32APIRUNNER_H_
#define WIN32APIRUNNER_H_

// Strip junk from windows.h
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// Grab _tWinMain macro
#include <stdio.h>
#include <climits>
#include "core/playground.h"

LRESULT CALLBACK globalWndProc2(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK globalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

class Win32apiRunner: public PlaygroundRunner {
	public:
		static const unsigned char ID = 0;

	private:
		HWND hWnd;
		const char *wndClassName;
		HINSTANCE hInstance;
		unsigned int mouse_x;
		unsigned int mouse_y;boolean fullScreen;
		Logger *logger;

	public:
		Win32apiRunner() {
			this->hWnd = null;
			this->wndClassName = "win32ApiRunnerWndClass";
			this->hInstance = GetModuleHandle(NULL);

			fullScreen = false;

			mouse_x = UINT_MAX;
			mouse_y = UINT_MAX;

			logger = Logger::getLogger("win32/Win32ApiRunner.h");
		}

		virtual unsigned char getId() {
			return Win32apiRunner::ID;
		}

		boolean getFullscreen() {
			return this->fullScreen;
		}

		virtual boolean init() {

			// Window class for our main window
			WNDCLASSEX wndClassEx = { sizeof(WNDCLASSEX), CS_DBLCLKS | CS_OWNDC
					| CS_HREDRAW | CS_VREDRAW, globalWndProc, 0,
					// We will store a pointer to Window object in per-window data
					sizeof(Playground *), hInstance, LoadIcon(NULL,
							IDI_WINLOGO), LoadCursor(NULL, IDC_CROSS),
					(HBRUSH) COLOR_BACKGROUND, null, TEXT(wndClassName),
					LoadIcon(NULL, IDI_WINLOGO) };
			if (!RegisterClassEx(&wndClassEx)) {
				printLastError("Could not register class");
				return false;
			}

			logger->debug("WndClassEx registered");

			hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
					TEXT(wndClassName), TEXT("HelloWorld!"),
					WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
					CW_USEDEFAULT, CW_USEDEFAULT, null, null, hInstance, this);
			if (!hWnd) {
				printLastError("Could not create window");
				return false;
			}

			logger->debug("Window Created");

			return true;
		}

		virtual boolean afterInit() {
			RECT rect;
			GetWindowRect(hWnd, &rect);

			this->resizeWindow(rect.bottom - rect.top, rect.right - rect.left);

//			// Show it
			show(true);

			logger->debug("Window made visible");

			return true;
		}

		LoopResult doLoop() {
			MSG msg;
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (msg.message == WM_QUIT)
					return STOP;

				//return SKIP;
			}

			return CONTINUE;
		}

		virtual LRESULT CALLBACK WndProc(HWND hwnd, UINT messageCode,
				WPARAM wParam, LPARAM lParam) {
			switch (messageCode) {
				case WM_NCCREATE:
					logger->debug("WM_NCCREATE");
					this->hWnd = hwnd;
					return DefWindowProc(hWnd, messageCode, wParam, lParam);
					break;
				case WM_DESTROY:
					logger->debug("WM_DESTROY");
					PostQuitMessage(0);
					break;
				case WM_MOUSEMOVE: {
					if(mouse_x == UINT_MAX) {
						mouse_x = LOWORD(lParam);
						mouse_y = HIWORD(lParam);
					}

					int dx = LOWORD(lParam) - (int) mouse_x;
					int dy = HIWORD(lParam) - (int) mouse_y;

					mouse_x = LOWORD(lParam);
					mouse_y = HIWORD(lParam);

					this->getContainer()->mouseMove(dx, dy);
					logger->debug("WM_MOUSEMOVE: (%d, %d)", dx, dy);

				}
					break;
				case WM_MBUTTONDOWN:
					//TODO: unify with keydown
					break;
				case WM_SIZE:
					logger->debug("WM_SIZE: (%d, %d)", HIWORD(lParam), LOWORD(lParam));

					this->getContainer()->resize(HIWORD(lParam),
							LOWORD(lParam) );
					break;
				case WM_KEYDOWN:
					logger->debug("WM_KEYDOWN: %d, %b", wParam, lParam);
					this->getContainer()->keyDown(wParam);
					break;
				default:
//#ifdef DEBUG
//					printf("WM_%d: DefWindowProc\n", messageCode);
//#endif

					return DefWindowProc(hWnd, messageCode, wParam, lParam);

			}
			return 0;
		}

		virtual ~Win32apiRunner() {
			UnregisterClass(TEXT(wndClassName), hInstance);
			logger->debug("WndClassEx unregistered");
		}

		void setWnd(HWND wnd) {
			hWnd = wnd;
		}

		HINSTANCE getInstance() const {
			return hInstance;
		}

		void setInstance(HINSTANCE instance) {
			hInstance = instance;
		}

		HWND getWnd() const {
			return hWnd;
		}

		boolean setIcon(HICON Icono) {
			return (SetClassLong(this->getWnd(), GCL_HICON, (LONG) Icono) == 0 ?
					false : true);
		}
		boolean setMenu(HMENU Menu) {
			if (GetMenu(this->getWnd()) == NULL) {
				RECT WndRect;
				GetClientRect(this->getWnd(), &WndRect);

				if (!::SetMenu(this->getWnd(), Menu))
					return (false);

				resize(WndRect.right - WndRect.left,
						WndRect.bottom - WndRect.top);

				return (true);
			}

			return (::SetMenu(this->getWnd(), Menu));
		}
		boolean setCursor(HCURSOR Cursor) {
			return (::SetClassLong(this->getWnd(), GCL_HCURSOR,
					(long) Cursor) == null ? false : true);
		}

		boolean resizeWindow(unsigned int height, unsigned int width) {
			DWORD dwExStyle;
			DWORD dwStyle;

			RECT WindowRect = { 0, 0, width, height };
			RECT actualWindowRect;
			GetWindowRect(this->getWnd(), &actualWindowRect);
			if (!this->fullScreen) {
				dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// Window Extended Style
				dwStyle = WS_OVERLAPPEDWINDOW;					// Windows Style

				AdjustWindowRectEx(&WindowRect, dwStyle,
						(GetMenu(this->getWnd()) != NULL), dwExStyle);
				MoveWindow(this->getWnd(), actualWindowRect.top,
						actualWindowRect.left,
						(WindowRect.right - WindowRect.left),
						(WindowRect.bottom - WindowRect.top), true);

				/* Un poquito de inicialización*/
				LPARAM lparam = ((DWORD) (WindowRect.bottom - WindowRect.top)
						<< 16) + (WindowRect.right - WindowRect.left);
				SendMessage(this->getWnd(), WM_SIZE, 0, lparam);
			}

			return (true);
		}

		boolean move(unsigned short x, unsigned short y) {
			if (!this->fullScreen) {
				RECT WndRect;
				GetWindowRect(this->getWnd(), &WndRect);
				MoveWindow(this->getWnd(), x, y, WndRect.right - WndRect.left,
						WndRect.bottom - WndRect.top, true);
			}

			return (true);
		}

		boolean setFullscreen(boolean fullScreen) {
			if (fullScreen) {
				SetWindowLong(this->getWnd(), GWL_STYLE, (long) WS_POPUP);
				SetWindowLong(this->getWnd(), GWL_EXSTYLE,
						(long) WS_EX_APPWINDOW);

				ShowWindow(this->getWnd(), SW_MAXIMIZE);

				POINT point;
				GetCursorPos(&point);
				mouse_x = point.x;
				mouse_y = point.y;

				this->fullScreen = true;
			} else {
				ShowWindow(this->getWnd(), SW_RESTORE);
				SetWindowLong(this->getWnd(), GWL_STYLE,
						(long) WS_OVERLAPPEDWINDOW);
				SetWindowLong(this->getWnd(), GWL_EXSTYLE,
						(long) (WS_EX_APPWINDOW | WS_EX_WINDOWEDGE));
				ShowWindow(this->getWnd(), SW_SHOW);
				DrawMenuBar(this->getWnd());

				this->fullScreen = false;
			}

			return (true);
		}
		void show(boolean show)
		{
			ShowWindow(this->getWnd(), show ? SW_SHOW : SW_HIDE);
		}

	protected:
		void printLastError(const char *message) {
			// Retrieve the system error message for the last-error code

			LPVOID lpMsgBuf;
			DWORD dw = GetLastError();

			FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
							| FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dw,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0,
					NULL);

			logger->error("%s: %lu - %s\n", message, dw, (char *) lpMsgBuf);

			LocalFree(lpMsgBuf);
		}


};

// This is the member window procedure
// It terminates message loop when window is closed, and paints Hello, World! when requested by Windows

// The helper window procedure
// It is called by Windows, and thus it's a non-member function
// This message handler will only be called after successful SetWindowLong call
// We can assume that pointer returned by GetWindowLong is valid
// It will route messages to our member message handler
LRESULT CALLBACK globalWndProc2(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	// Get a window pointer associated with this window
	Win32apiRunner *w = (Win32apiRunner *) GetWindowLong(hwnd, GWL_USERDATA);
	// Redirect messages to the window procedure of the associated window
	return w->WndProc(hwnd, msg, wp, lp);
}

// The helper window procedure
// It is called by Windows, and thus it's a non-member function
// It will provide default handling for messages until WM_NCCREATE comes around,
// in which case it will set per-window this pointer to whatever was passed in CREATESTRUCT
// and reset the window's message handler to WndProc2
LRESULT CALLBACK globalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	// If the message is WM_NCCREATE, associate Window pointer with this window
	// Note: WM_NCCREATE is not the first message that this WndProc will process
	if (msg == WM_NCCREATE) {
		LPCREATESTRUCT cs = (LPCREATESTRUCT) lp;
		// Set associated this pointer
		SetWindowLong(hwnd, GWL_USERDATA, (long) cs->lpCreateParams);
		// Reset the window message handler
		SetWindowLong(hwnd, GWL_WNDPROC, (long) globalWndProc2);
		// Dispatch WM_NCCREATE to the member message handler
		return globalWndProc2(hwnd, msg, wp, lp);
	} else {
		// Use default handling; the window is not associated yet
		return DefWindowProc(hwnd, msg, wp, lp);
	}
}

#endif /* WIN32APIRUNNER_H_ */
