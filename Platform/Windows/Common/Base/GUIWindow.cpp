#include "GUIWindow.h"
namespace Editor {
	void GUIWindow::DrawContent(Gdiplus::Graphics&painter)
	{
	}

	void GUIWindow::OnSize(WPARAM wParam, LPARAM lParam, void*reserved) {
		RECT rect;
		GetWindowRect(mhWnd, &rect);
		mRect.X = rect.left;
		mRect.Y = rect.top;
		mRect.Width = rect.right - rect.left;
		mRect.Height = rect.bottom - rect.top;
	}
	void GUIWindow::OnKeyDown(WPARAM wParam, LPARAM lParam, void*reserved) {
	}
	bool drag = false;
	void GUIWindow::OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		SetCapture(mhWnd);
		drag = true;
		POINT pos;
		GetCursorPos(&pos);
		RECT windowRect;
		GetWindowRect(mhWnd, &windowRect);
		mDeltaWhenDrag.X = pos.x - windowRect.left;
		mDeltaWhenDrag.Y = pos.y - windowRect.top;
	}

	void GUIWindow::OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		ReleaseCapture();
		drag = false;
	}

	void GUIWindow::OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		if (drag)
		{
			POINT pos;
			GetCursorPos(&pos);
			if (mIsChild) {
				HWND parent = ::GetParent(mhWnd);
				ScreenToClient(parent, &pos);
				MoveWindow(pos.x - mDeltaWhenDrag.X, pos.y - mDeltaWhenDrag.Y, mRect.Width, mRect.Height);
			}else {
				MoveWindow(pos.x - mDeltaWhenDrag.X, pos.y - mDeltaWhenDrag.Y, mRect.Width, mRect.Height);
			}
		}
	}

	void GUIWindow::OnCommand(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		UINT nNotify = HIWORD(wParam);
		UINT commandID = LOWORD(wParam);
		switch (commandID)
		{
		case 1:
		{
			if (!OpenClipboard(mhWnd))
				return;
			HGLOBAL   hglb;
			hglb = GetClipboardData(CF_UNICODETEXT);
			if (hglb != NULL)
			{
				LPTSTR    lptstr = (LPTSTR)GlobalLock(hglb);
				if (lptstr != NULL)
				{
					// Call the application-defined ReplaceSelection 
					// function to insert the text and repaint the 
					// window. 
					int len = lstrlen(lptstr);
					if (len < 128) {
						char szBuffer[1024] = { 0 };
						//Alice::String::WideCharToUTF8(lptstr, szBuffer);
						//len = strlen(szBuffer);
						//Alice::GetEventDispatcher()->OnPaste(szBuffer, len);
					}
					GlobalUnlock(hglb);
				}
			}
			EmptyClipboard();
			CloseClipboard();
		}
		break;
		}
	}

	void GUIWindow::Init(HWND parent){
		mIsChild = false;
		DWORD windowStyle = WS_OVERLAPPED  | WS_CLIPSIBLINGS;
		if (parent!=nullptr){
			windowStyle|= WS_CHILDWINDOW;
			mIsChild = true;
		}
		mhWnd = CreateWindowEx(NULL, L"GUIWindow", NULL,
			windowStyle, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
			parent, NULL, GetModuleHandle(NULL), nullptr);
		SetWindowLongPtr(mhWnd, GWL_USERDATA, (LONG_PTR)this);
		mHDC = GetWindowDC(mhWnd);
		mBKGColor = Gdiplus::Color(0, 150, 200);
	}

	void GUIWindow::ToChildWindow() {
		mIsChild = true;
	}

	void GUIWindow::ToMainWindow() {
		mIsChild = false;
	}

	void GUIWindow::UpdateTitle() {
		//const char *renderer = (const char*)glGetString(GL_RENDERER);
		//const char *glVersion = (const char*)glGetString(GL_VERSION);
		//char szTitle[128] = { 0 };
		//strcpy(szTitle, "GPU : ");
		//strcat(szTitle, renderer);
		//strcat(szTitle, " ,OpenGL Version : ");
		//strcat(szTitle, glVersion);
		//mTitle->SetText(szTitle);
	}

	LRESULT GUIWindow::OnNCACTIVATE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		OnNCPAINT(hWnd, message, wParam, lParam, reserved);
		return 0;
	}

	LRESULT GUIWindow::OnNCCALCSIZE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		NCCALCSIZE_PARAMS* pParams = NULL;
		RECT* pRect = NULL;

		BOOL bValue = static_cast<BOOL>(wParam);
		if (bValue) pParams = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
		else pRect = reinterpret_cast<RECT*>(lParam);

		if (bValue)
			pRect = &pParams->rgrc[0];

		if (bValue) {
			pRect->left = pRect->left + 3;
			pRect->top = pRect->top + 20;
			pRect->right = pRect->right - 3;
			pRect->bottom = pRect->bottom - 3;
			pParams->rgrc[1] = pParams->rgrc[0];
		}
		else DefWindowProc(hWnd, message, wParam, lParam);
		return 0;
	}

	LRESULT GUIWindow::OnNCPAINT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		HDC WindowDC = GetWindowDC(hWnd);
		RECT WindowRect;
		GetWindowRect(hWnd, &WindowRect);
		OffsetRect(&WindowRect, -WindowRect.left, -WindowRect.top);
		ExcludeClipRect(WindowDC, 0, 0,
			WindowRect.right, WindowRect.bottom);
		SelectClipRgn(WindowDC, NULL);
		ReleaseDC(hWnd, WindowDC);
		return 0;
	}
	LRESULT GUIWindow::OnNCHITTEST(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		RECT WindowRect;
		GetWindowRect(hWnd, &WindowRect);
		POINT point;
		point.x = LOWORD(lParam) - WindowRect.left;
		point.y = HIWORD(lParam) - WindowRect.top;
		Gdiplus::Rect rect = { 0, 3, 3, mRect.Height - 6 };
		if (rect.Contains(point.x,point.y))
			return HTLEFT;

		rect = { mRect.Width - 3, 3, mRect.Width, mRect.Height - 6 };
		if (rect.Contains(point.x, point.y))
			return HTRIGHT;

		rect = { 3, mRect.Height - 3, mRect.Width - 6, 3 };
		if (rect.Contains(point.x, point.y))
			return HTBOTTOM;

		rect = { 3, 0, mRect.Width - 6, 3 };
		if (rect.Contains(point.x, point.y))
			return HTTOP;

		//rect = { 3, 3, mRect.Width - 6, 30 };
		//if (rect.Contains(point.x, point.y))
		//	return HTCAPTION;

		return HTCLIENT;
	}

	void GUIWindow::InitWindowClasses() {
		RegisterWindowClass(CS_VREDRAW | CS_HREDRAW | CS_DROPSHADOW | CS_DBLCLKS , L"GUIWindow", WindowEventProc);
	}
}