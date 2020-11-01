#include "MainWindow.h"
#include "Runtime/Debugger/Logger.h"
#include "ViewWindow.h"
namespace Editor {
	MainWindow::MainWindow() {
		mbDraging = false;
		mUIRoot = nullptr;
		mLastTouchObject = nullptr;
		mTitleBKGColor = Gdiplus::Color(200, 200, 200);
		mContainer = nullptr;
		SetWindowName("MainWindow");
	}
	void MainWindow::OnSize(WPARAM wParam, LPARAM lParam, void*reserved) {
		RECT rect;
		GetWindowRect(mhWnd, &rect);
		mRect = {rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top};
		RECT*ptr_rect = &rect;
	}
	LRESULT MainWindow::OnSizing(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		RECT * ptr_new_rect = (RECT*)lParam;
		RECT current_rect;
		GetWindowRect(mhWnd, &current_rect);
		int new_width = ptr_new_rect->right - ptr_new_rect->left;
		int new_height = ptr_new_rect->bottom - ptr_new_rect->top;
		int last_width = current_rect.right - current_rect.left;
		int last_height = current_rect.bottom - current_rect.top;
		switch (wParam){
		case WMSZ_LEFT:{
			int deltaX = new_width - last_width;
			if (deltaX > 0) {
				ExtentWindowFromLeft(deltaX);
			}
			else if (deltaX < 0) {
				ReduceWindowFromLeft(deltaX);
			}
		}
			break;
		case WMSZ_RIGHT: {
			int deltaX = new_width - last_width;
			if (deltaX > 0) {
				ExtentWindowFromRight(deltaX);
			}
			else if (deltaX < 0) {
				ReduceWindowFromRight(deltaX);
			}
		}
			break;
		case WMSZ_TOP: {
			int deltaY = new_height - last_height;
			if (deltaY > 0) {
				ExtentWindowFromTop(deltaY);
			}
			else if (deltaY < 0) {
				ReduceWindowFromTop(deltaY);
			}
		}
			break;
		case WMSZ_TOPLEFT: {
			int deltaX = new_width - last_width;
			if (deltaX > 0) {
				ExtentWindowFromLeft(deltaX);
			}
			else if (deltaX < 0) {
				ReduceWindowFromLeft(deltaX);
			}
			int deltaY = new_height - last_height;
			if (deltaY > 0) {
				ExtentWindowFromTop(deltaY);
			}
			else if (deltaY < 0) {
				ReduceWindowFromTop(deltaY);
			}
		}
			break;
		case WMSZ_TOPRIGHT: {
			int deltaX = new_width - last_width;
			if (deltaX > 0) {
				ExtentWindowFromRight(deltaX);
			}
			else if (deltaX < 0) {
				ReduceWindowFromRight(deltaX);
			}
			int deltaY = new_height - last_height;
			if (deltaY > 0) {
				ExtentWindowFromTop(deltaY);
			}
			else if (deltaY < 0) {
				ReduceWindowFromTop(deltaY);
			}
		}
			break;
		case WMSZ_BOTTOM: {
			int deltaY = new_height - last_height;
			if (deltaY > 0) {
				ExtentWindowFromBottom(deltaY);
			}
			else if (deltaY < 0) {
				ReduceWindowFromBottom(deltaY);
			}
		}
			break;
		case WMSZ_BOTTOMLEFT: {
			int deltaX = new_width - last_width;
			if (deltaX > 0) {
				ExtentWindowFromLeft(deltaX);
			}
			else if (deltaX < 0) {
				ReduceWindowFromLeft(deltaX);
			}
			int deltaY = new_height - last_height;
			if (deltaY > 0) {
				ExtentWindowFromBottom(deltaY);
			}
			else if (deltaY < 0) {
				ReduceWindowFromBottom(deltaY);
			}
		}
			break;
		case WMSZ_BOTTOMRIGHT: {
			int deltaX = new_width - last_width;
			if (deltaX > 0) {
				ExtentWindowFromRight(deltaX);
			}
			else if (deltaX < 0) {
				ReduceWindowFromRight(deltaX);
			}
			int deltaY = new_height - last_height;
			if (deltaY > 0) {
				ExtentWindowFromBottom(deltaY);
			}
			else if (deltaY < 0) {
				ReduceWindowFromBottom(deltaY);
			}
		}
			break;
		}
		return TRUE;
	}
	void MainWindow::ExtentWindowFromLeft(int &deltaX) {
		mContainer->ExtentContainerFromLeft(deltaX);
	}
	void MainWindow::ExtentWindowFromRight(int &deltaX) {
		mContainer->ExtentContainerFromRight(deltaX);
	}
	void MainWindow::ExtentWindowFromBottom(int &deltaY) {
		mContainer->ExtentContainerFromBottom(deltaY);
	}
	void MainWindow::ExtentWindowFromTop(int &deltaY) {
		mContainer->ExtentContainerFromTop(deltaY);
	}
	void MainWindow::ReduceWindowFromLeft(int &deltaX) {
		mContainer->ReduceContainerFromLeft(deltaX);
	}
	void MainWindow::ReduceWindowFromRight(int &deltaX) {
		mContainer->ReduceContainerFromRight(deltaX);
	}
	void MainWindow::ReduceWindowFromBottom(int &deltaY) {
		mContainer->ReduceContainerFromBottom(deltaY);
	}
	void MainWindow::ReduceWindowFromTop(int &deltaY) {
		mContainer->ReduceContainerFromTop(deltaY);
	}
	void MainWindow::OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */){
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
	void MainWindow::OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */){
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		ReleaseCapture();
		mbDraging = false;
	}
	void MainWindow::OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */){
		if (mbDraging){
			POINT pos;
			GetCursorPos(&pos);
			SetRect(pos.x - mDeltaWhenDrag.X, pos.y - mDeltaWhenDrag.Y, mRect.Width, mRect.Height);
			MoveWindow(pos.x - mDeltaWhenDrag.X, pos.y - mDeltaWhenDrag.Y, mRect.Width, mRect.Height);
		}
		else {
		}
		DoubleBufferedWindow::OnMouseMove(wParam, lParam, reserved);
	}
	void MainWindow::DrawContent(Gdiplus::Graphics&painter) {
		if (mUIRoot!=nullptr){
			mUIRoot->DrawRecursively(painter);
		}
	}
	void MainWindow::RenderChildren(Gdiplus::Graphics&painter, const Gdiplus::Rect & rect_need_update) {
		HWND cWnd = GetWindow(mhWnd, GW_CHILD);
		while (cWnd != nullptr) {
			BaseWindow*vw = WindowInstance<BaseWindow>(cWnd);
			vw->OnParentPaint(painter);
			cWnd = GetNextWindow(cWnd, GW_HWNDNEXT);
		}
	}
	void MainWindow::OnClearBKG(Gdiplus::Graphics&painter) {
		//painter.ExcludeClip(Gdiplus::Rect(mLeftNCSize, mTopNCSize, mBufferWidth - mLeftNCSize - mRightNCSize, mBufferHeight - mTopNCSize - mBottomNCSize));
		painter.Clear(mBKGColor);
		//painter.ResetClip();
	}

	void MainWindow::Init() {
		DWORD windowStyle = WS_OVERLAPPED | WS_CLIPCHILDREN;
		mhWnd = CreateWindowEx(NULL, L"MainWindow", NULL,
			windowStyle, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, GetModuleHandle(NULL), nullptr);
		SetWindowLongPtr(mhWnd, GWL_USERDATA, (LONG_PTR)this);
		mHDC = GetWindowDC(mhWnd);
		SetNCSize(6, 6, 6, 26);
	}
	void MainWindow::SetContainer(WindowContainer *container) {
		mContainer = container;
		Gdiplus::Rect container_min_rect = container->GetMinRect();
		SetMinRect(0, 0, container_min_rect.Width+mLeftNCSize+mRightNCSize, container_min_rect.Height+mTopNCSize+mBottomNCSize);
		Debug("%s MainWindow::SetContainer min rect[%dx%d]",mName,mMinRect.Width,mMinRect.Height);
	}
	void MainWindow::RemoveChildWindowAt(ChildWindowLocation location, BaseWindow*window) {
	}
	void MainWindow::InitWindowClasses() {
		RegisterWindowClass(CS_VREDRAW | CS_HREDRAW | CS_DROPSHADOW | CS_DBLCLKS , L"MainWindow", WindowEventProc);
	}
}