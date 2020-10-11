#include "MainWindow.h"
#include "Runtime/Debugger/Logger.h"
#include "ViewWindow.h"
namespace Editor {
	MainWindow::MainWindow() {
		mbDraging = false;
		mMarginLeft = 3;
		mMarginRight = 3;
		mMarginTop = 26;
		mMarginBottom = 4;
		mFrameEdgeWidth = 3;
		mUIRoot = nullptr;
		mLastTouchObject = nullptr;
		mTitleBKGColor = Gdiplus::Color(200, 200, 200);
	}
	void MainWindow::OnPaint(){
		if (mBufferWidth != mRect.Width || mBufferHeight != mRect.Height) {
			InitDoubleBuffer();
		}
		Gdiplus::Graphics painter(mBKGDC);
		Gdiplus::SolidBrush brush(mBKGColor);
		painter.FillRectangle(&brush, 0, mMarginTop, mRect.Width, mRect.Height);
		DrawFrame(painter);
		DrawNCUI(painter);
		DrawContent(painter);
		OnEndPaint();
	}
	void MainWindow::OnSize(WPARAM wParam, LPARAM lParam, void*reserved) {
		RECT rect;
		GetWindowRect(mhWnd, &rect);
		mRect.X = rect.left;
		mRect.Y = rect.top;
		mRect.Width = rect.right - rect.left;
		mRect.Height = rect.bottom - rect.top;
		HWND cWnd = GetWindow(mhWnd, GW_CHILD);
		while (cWnd != nullptr) {
			BaseWindow*vw = WindowInstance<BaseWindow>(cWnd);
			vw->OnParentResized(mRect.Width-mMarginLeft-mMarginRight, mRect.Height-mMarginTop-mMarginBottom);
			cWnd = GetNextWindow(cWnd, GW_HWNDNEXT);
		}
	}
	void MainWindow::OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		SetCapture(mhWnd);
		mbDraging = true;
		POINT pos;
		GetCursorPos(&pos);
		RECT windowRect;
		GetWindowRect(mhWnd, &windowRect);
		mDeltaWhenDrag.X = pos.x - windowRect.left;
		mDeltaWhenDrag.Y = pos.y - windowRect.top;
	}
	void MainWindow::OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		ReleaseCapture();
		mbDraging = false;
	}
	void MainWindow::OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		if (mbDraging)
		{
			POINT pos;
			GetCursorPos(&pos);
			MoveWindow(pos.x - mDeltaWhenDrag.X, pos.y - mDeltaWhenDrag.Y, mRect.Width, mRect.Height);
		}
		DoubleBufferedWindow::OnMouseMove(wParam, lParam, reserved);
	}

	void MainWindow::Init(){
		DWORD windowStyle = WS_OVERLAPPED   | WS_CLIPCHILDREN;
		mhWnd = CreateWindowEx(NULL, L"MainWindow", NULL,
			windowStyle, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, GetModuleHandle(NULL), nullptr);
		SetWindowLongPtr(mhWnd, GWL_USERDATA, (LONG_PTR)this);
		mHDC = GetWindowDC(mhWnd);
	}

	LRESULT MainWindow::OnNCACTIVATE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		return TRUE;
	}

	LRESULT MainWindow::OnNCCALCSIZE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		NCCALCSIZE_PARAMS* pParams = NULL;
		RECT* pRect = NULL;
		 
		BOOL bValue = static_cast<BOOL>(wParam);

		if (bValue) {
			pParams = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
			pRect = &pParams->rgrc[0];
			pRect->left = pRect->left + mMarginLeft;
			pRect->top = pRect->top + mMarginTop;
			pRect->right = pRect->right - mMarginRight;
			pRect->bottom = pRect->bottom - mMarginBottom;
			pParams->rgrc[1] = pParams->rgrc[0];
		}
		else {
			DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	LRESULT MainWindow::OnNCPAINT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		HDC WindowDC = GetWindowDC(hWnd);
		RECT ClientRect;
		GetClientRect(hWnd, &ClientRect);
		OffsetRect(&ClientRect, mMarginLeft, mMarginTop);
		ExcludeClipRect(WindowDC, ClientRect.left, ClientRect.top,ClientRect.right, ClientRect.bottom);
		Gdiplus::Graphics painter(WindowDC);
		DrawFrame(painter);
		DrawNCUI(painter);
		ReleaseDC(hWnd, WindowDC);
		return 0;
	}
	LRESULT MainWindow::OnNCHITTEST(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		RECT WindowRect;
		GetWindowRect(hWnd, &WindowRect);
		POINT point;
		point.x = LOWORD(lParam) - WindowRect.left;
		point.y = HIWORD(lParam) - WindowRect.top;
		if (point.x <= mFrameEdgeWidth) {
			if (point.y <= mFrameEdgeWidth) {
				return HTTOPLEFT;
			}
			else if (point.y >= (mRect.Height - mFrameEdgeWidth)) {
				return HTBOTTOMLEFT;
			}
			return HTLEFT;
		}
		else if (point.x > mFrameEdgeWidth && point.x < (mRect.Width - mFrameEdgeWidth)) {
			if (point.y <= mFrameEdgeWidth) {
				return HTTOP;
			}
			else if (point.y >= (mRect.Height - mFrameEdgeWidth)) {
				return HTBOTTOM;
			}
			return HTCLIENT;
		}
		else {
			if (point.y <= mFrameEdgeWidth) {
				return HTTOPRIGHT;
			}
			else if (point.y >= (mRect.Height - mFrameEdgeWidth)) {
				return HTBOTTOMRIGHT;
			}
			return HTRIGHT;
		}
		return HTCLIENT;
	}

	void MainWindow::DrawFrame(Gdiplus::Graphics&painter) {
		Gdiplus::SolidBrush brush(mBKGColor);
		painter.FillRectangle(&brush, 0, 0, mRect.Width, mMarginTop);
	}
	void MainWindow::DrawNCUI(Gdiplus::Graphics&painter) {
		if (mUIRoot!=nullptr){
			mUIRoot->DrawRecursively(painter);
		}
	}
	void MainWindow::InitWindowClasses() {
		RegisterWindowClass(CS_VREDRAW | CS_HREDRAW | CS_DROPSHADOW | CS_DBLCLKS , L"MainWindow", WindowEventProc);
	}
}