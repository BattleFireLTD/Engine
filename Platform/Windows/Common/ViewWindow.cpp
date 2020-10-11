#include "ViewWindow.h"
#include "Runtime/Debugger/Logger.h"
namespace Editor {
	ViewWindow::ViewWindow() {
		mbDraging = false;
		mbFixedPos = false;
		mbFixedWidth = false;
		mbFixedHeight = false;
		mFixedRect = {};
		mUIRoot = nullptr;
		mLastTouchObject = nullptr;
		mLastHoverObject = nullptr;
		SetWindowName("ViewWindow");
	}
	void ViewWindow::DrawContent(Gdiplus::Graphics&painter) {
		if (mUIRoot != nullptr) {
			mUIRoot->DrawRecursively(painter);
		}
	}
	void ViewWindow::OnCommand(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		UINT nNotify = HIWORD(wParam);
		UINT commandID = LOWORD(wParam);
		mUIRoot->ProcessEvent(commandID);
	}
	void ViewWindow::OnSize(WPARAM wParam, LPARAM lParam, void*reserved) {
		RECT rect;
		GetWindowRect(mhWnd, &rect);
		mRect.X = rect.left;
		mRect.Y = rect.top;
		mRect.Width = rect.right - rect.left;
		mRect.Height = rect.bottom - rect.top;
	}
	void ViewWindow::OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		UINode*node = nullptr;
		if (mUIRoot != nullptr) {
			node = mUIRoot->Intersect(x, y);
		}
		if (node != nullptr) {
			node->OnTouchBegin(x, y);
			mLastTouchObject = node;
			InvalidateRect(mhWnd, nullptr, true);
			SetCapture(mhWnd);
		}
		else {
			DoubleBufferedWindow::OnLButtonDown(wParam, lParam, reserved);
		}
	}
	void ViewWindow::OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		UINode*node = nullptr;
		if (mUIRoot != nullptr) {
			node = mUIRoot->Intersect(x, y);
		}
		if (node != nullptr&&node == mLastTouchObject) {
			node->OnTouchEnd(x, y);
			InvalidateRect(mhWnd, nullptr, true);
			ReleaseCapture();
		}
		else {
			if (mLastTouchObject != nullptr) {
				mLastTouchObject->OnTouchCanceled(x, y);
				mLastTouchObject = nullptr;
				InvalidateRect(mhWnd, nullptr, true);
				ReleaseCapture();
			}
			else {
				DoubleBufferedWindow::OnLButtonUp(wParam, lParam, reserved);
			}
		}
	}
	void ViewWindow::OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		UINode*node = nullptr;
		if (mUIRoot!=nullptr){
			node=mUIRoot->Intersect(x, y);
		}
		if (node != nullptr) {
			if (mLastHoverObject != node) {
				if (mLastHoverObject != nullptr) {
					mLastHoverObject->OnTouchLeave(x, y);
				}
				mLastHoverObject = node;
				mLastHoverObject->OnTouchEnter(x, y);
				InvalidateRect(mhWnd, nullptr, true);
			}
		}
		else {
			if (mLastHoverObject != nullptr) {
				mLastHoverObject->OnTouchLeave(x, y);
				mLastHoverObject = nullptr;
				InvalidateRect(mhWnd, nullptr, true);
			}
		}
		DoubleBufferedWindow::OnMouseMove(wParam, lParam, reserved);
	}
	void ViewWindow::OnMouseLeave(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		if (mLastHoverObject != nullptr) {
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mLastHoverObject->OnTouchLeave(x, y);
			mLastHoverObject = nullptr;
			InvalidateRect(mhWnd, nullptr, true);
		}
		DoubleBufferedWindow::OnMouseLeave(wParam, lParam, reserved);
	}

	void ViewWindow::Init(BaseWindow*parent){
		DWORD windowStyle = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		mhWnd = CreateWindowEx(NULL, L"ViewWindow", NULL, windowStyle, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, parent->GetHwnd(), NULL, GetModuleHandle(NULL), nullptr);
		SetWindowLongPtr(mhWnd, GWL_USERDATA, (LONG_PTR)this);
		mHDC = GetWindowDC(mhWnd);
		mParent = parent;
	}
	void ViewWindow::AppendUI(UINode*node) {
		if (mUIRoot==nullptr){
			mUIRoot = node;
		}
		else {
			mUIRoot->AppendChild(node);
		}
	}
	void ViewWindow::FixedWindow(int x, int y, int width, int height) {
		mFixedRect = {x,y,width,height};
		mbFixedPos = (x == -1 || y == -1)?false:true;
		mbFixedWidth = width == -1 ? false : true;
		mbFixedHeight = height == -1 ? false : true;
	}
	void ViewWindow::OnParentResized(int width, int height) {
		int x = 0, y = 0, w = width, h = height;
		if (mbFixedPos){
			x = mFixedRect.X;
			y = mFixedRect.Y;
		}
		if (mbFixedWidth){
			w = mFixedRect.Width;
		}
		if (mbFixedHeight){
			h = mFixedRect.Height;
		}
		if (mUIRoot != nullptr) {
			mUIRoot->OnContainerSizeChangedRecursively(w, h);
		}
		MoveWindow(x, y, w, h);
	}
	void ViewWindow::InitWindowClasses() {
		RegisterWindowClass(CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS , L"ViewWindow", WindowEventProc);
	}
}