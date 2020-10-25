#include "MainWindow.h"
#include "Runtime/Debugger/Logger.h"
#include "ViewWindow.h"
namespace Editor {
	MainWindow::MainWindow() {
		mbDraging = false;
		mUIRoot = nullptr;
		mLastTouchObject = nullptr;
		mTitleBKGColor = Gdiplus::Color(200, 200, 200);
		mLeftMostChildren = nullptr;
		mRightMostChildren = nullptr;
		mTopMostChildren = nullptr;
		mBottomMostChildren = nullptr;
		SetWindowName("MainWindow");
	}
	BaseWindow::WindowResizeDirection MainWindow::OnPostSizeChanged(int &deltaX, int &deltaY) {
		BaseWindow::WindowResizeDirection d = BaseWindow::kWindowResizeDirectionNone;

		RECT rect;
		GetWindowRect(mhWnd, &rect);
		int new_width = rect.right - rect.left;
		int new_height = rect.bottom - rect.top;
		if (mRect.X != rect.left) {
			d = BaseWindow::kWindowResizeDirectionFromLeft;
		}
		if (mRect.Y!=rect.top){
			if (d==BaseWindow::kWindowResizeDirectionFromLeft){
				d = BaseWindow::kWindowResizeDirectionFromLeftTop;
			}
			else {
				d = BaseWindow::kWindowResizeDirectionFromTop;
			}
		}
		deltaX = new_width - mRect.Width;
		deltaY = new_height - mRect.Height;
		if (deltaX != 0) {
			if (d== BaseWindow::kWindowResizeDirectionFromTop){
				d = BaseWindow::kWindowResizeDirectionFromRightTop;
			}else if (d != BaseWindow::kWindowResizeDirectionFromLeft || d != BaseWindow::kWindowResizeDirectionFromLeftTop){
				d = BaseWindow::kWindowResizeDirectionFromRight;
			}
		}
		if (deltaY != 0) {
			if (d == BaseWindow::kWindowResizeDirectionFromLeft) {
				d = BaseWindow::kWindowResizeDirectionFromLeftBottom;
			}
			else if (d == BaseWindow::kWindowResizeDirectionFromRight) {
				d = BaseWindow::kWindowResizeDirectionFromRightBottom;
			}else if (d != BaseWindow::kWindowResizeDirectionFromTop ||
				d != BaseWindow::kWindowResizeDirectionFromLeftTop||
				d != BaseWindow::kWindowResizeDirectionFromRightTop) {
				d = BaseWindow::kWindowResizeDirectionFromBottom;
			}
		}
		SetRect(mRect.X, mRect.Y, new_width, new_height);
		return d;
	}
	void MainWindow::OnSize(WPARAM wParam, LPARAM lParam, void*reserved) {
		RECT rect;
		GetWindowRect(mhWnd, &rect);
		mRect = {rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top};
		RECT*ptr_rect = &rect;
		//Debug("MainWindow::OnSize (%d,%d,%d,%d) %dx%d", ptr_rect->left, ptr_rect->top, ptr_rect->right, ptr_rect->bottom, ptr_rect->right - ptr_rect->left, ptr_rect->bottom - ptr_rect->top);
		/*HWND cWnd = GetWindow(mhWnd, GW_CHILD);
		while (cWnd != nullptr) {
			BaseWindow*vw = WindowInstance<BaseWindow>(cWnd);
			vw->OnParentResized(mRect.Width-mLeftNCSize-mRightNCSize, mRect.Height-mTopNCSize-mBottomNCSize,deltaX,deltaY,d);
			cWnd = GetNextWindow(cWnd, GW_HWNDNEXT);
		}*/
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
				WindowHolder *wh = mLeftMostChildren;
				int deltaX_consumed = deltaX;
				bool can_resize = true;
				while (wh != nullptr) {
					int temp_deltaX = deltaX;
					wh->mWindow->TryToExtentWindowHorizontallyFromLeft(temp_deltaX);
					if (temp_deltaX==deltaX){
						can_resize = false;
						break;
					}
					int consumued_deltaX = deltaX - temp_deltaX;
					if (deltaX_consumed > consumued_deltaX){
						deltaX_consumed = consumued_deltaX;
					}
					wh = wh->Next<WindowHolder>();
				}
				if (false==can_resize){
					ptr_new_rect->left = current_rect.left;
					break;
				}
				//int fixed_new_width = last_width + deltaX_consumed;
				//wh = mLeftMostChildren;
				//while (wh != nullptr) {
				//	wh->mWindow->OnParentResized(fixed_new_width, last_height);
				//	wh = wh->Next<WindowHolder>();
				//}
			}
			else if (deltaX < 0) {
				WindowHolder *wh = mLeftMostChildren;
				int deltaX_consumed = deltaX;
				bool can_resize = true;
				while (wh != nullptr) {
					int temp_deltaX = deltaX;
					wh->mWindow->TryToReduceWindowHorizontallyFromLeft(temp_deltaX);
					if (temp_deltaX == deltaX) {
						can_resize = false;
						break;
					}
					int consumued_deltaX = deltaX - temp_deltaX;
					if (deltaX_consumed < consumued_deltaX) {
						deltaX_consumed = consumued_deltaX;
					}
					wh = wh->Next<WindowHolder>();
				}
				if (false == can_resize) {
					ptr_new_rect->left = current_rect.left;
					break;
				}
				//int fixed_new_width = last_width + deltaX_consumed;
				//wh = mLeftMostChildren;
				//while (wh != nullptr) {
				//	wh->mWindow->OnParentResized(fixed_new_width, last_height);
				//	wh = wh->Next<WindowHolder>();
				//}
			}
		}
			break;
		case WMSZ_RIGHT: {
			int deltaX = new_width - last_width;
			if (deltaX > 0) {
				WindowHolder *wh = mRightMostChildren;
				int deltaX_consumed = deltaX;
				bool can_resize = true;
				while (wh != nullptr) {
					int temp_deltaX = deltaX;
					wh->mWindow->TryToExtentWindowHorizontallyFromRight(temp_deltaX);
					if (temp_deltaX == deltaX) {
						can_resize = false;
						break;
					}
					int consumued_deltaX = deltaX - temp_deltaX;
					if (deltaX_consumed > consumued_deltaX) {
						deltaX_consumed = consumued_deltaX;
					}
					wh = wh->Next<WindowHolder>();
				}
				if (false == can_resize) {
					ptr_new_rect->left = current_rect.left;
					break;
				}
				int fixed_new_width = last_width + deltaX_consumed;
				wh = mRightMostChildren;
				while (wh != nullptr) {
					wh->mWindow->ExtentWindowHorizontallyFromRight(deltaX_consumed);
					wh = wh->Next<WindowHolder>();
				}
			}
			else if (deltaX < 0) {
				WindowHolder *wh = mRightMostChildren;
				int deltaX_consumed = deltaX;
				bool can_resize = true;
				while (wh != nullptr) {
					int temp_deltaX = deltaX;
					wh->mWindow->TryToReduceWindowHorizontallyFromRight(temp_deltaX);
					if (temp_deltaX == deltaX) {
						can_resize = false;
						break;
					}
					int consumued_deltaX = deltaX - temp_deltaX;
					if (deltaX_consumed < consumued_deltaX) {
						deltaX_consumed = consumued_deltaX;
					}
					wh = wh->Next<WindowHolder>();
				}
				if (false == can_resize) {
					ptr_new_rect->left = current_rect.left;
					break;
				}
				//int fixed_new_width = last_width + deltaX_consumed;
				//wh = mLeftMostChildren;
				//while (wh != nullptr) {
				//	wh->mWindow->OnParentResized(fixed_new_width, last_height);
				//	wh = wh->Next<WindowHolder>();
				//}
			}
		}
			break;
		case WMSZ_TOP:
			break;
		case WMSZ_TOPLEFT:
			break;
		case WMSZ_TOPRIGHT:
			break;
		case WMSZ_BOTTOM:
			break;
		case WMSZ_BOTTOMLEFT:
			break;
		case WMSZ_BOTTOMRIGHT:
			break;
		}
		//Debug("Current OnSizing (%d,%d,%d,%d) %dx%d", current_rect.left, current_rect.top, current_rect.right, current_rect.bottom, current_rect.right - current_rect.left, current_rect.bottom - current_rect.top);
		//Debug("Next OnSizing (%d,%d,%d,%d) %dx%d", ptr_new_rect->left, ptr_new_rect->top, ptr_new_rect->right, ptr_new_rect->bottom, new_width, new_height);
		return TRUE;
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
			//if (vw->GetRect().IntersectsWith(rect_need_update)) {
				vw->OnParentPaint(painter);
			//}
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
	void MainWindow::AddChildWindowAt(ChildWindowLocation location, BaseWindow*window) {
		switch (location){
		case Editor::kChildWindowLocationLeftEdge:
			if (mLeftMostChildren==nullptr){
				mLeftMostChildren = new WindowHolder(window);
			}
			else {
				mLeftMostChildren->PushBack(new WindowHolder(window));
			}
			break;
		case Editor::kChildWindowLocationRightEdge:
			if (mRightMostChildren == nullptr) {
				mRightMostChildren = new WindowHolder(window);
			}
			else {
				mRightMostChildren->PushBack(new WindowHolder(window));
			}
			break;
		case Editor::kChildWindowLocationTopEdge:
			if (mTopMostChildren == nullptr) {
				mTopMostChildren = new WindowHolder(window);
			}
			else {
				mTopMostChildren->PushBack(new WindowHolder(window));
			}
			break;
		case Editor::kChildWindowLocationBottomEdge:
			if (mBottomMostChildren == nullptr) {
				mBottomMostChildren = new WindowHolder(window);
			}
			else {
				mBottomMostChildren->PushBack(new WindowHolder(window));
			}
			break;
		}
	}
	void MainWindow::RemoveChildWindowAt(ChildWindowLocation location, BaseWindow*window) {
		switch (location){
		case Editor::kChildWindowLocationUnkown:
			break;
		case Editor::kChildWindowLocationLeftEdge:
			break;
		case Editor::kChildWindowLocationRightEdge:
			break;
		case Editor::kChildWindowLocationTopEdge:
			break;
		case Editor::kChildWindowLocationBottomEdge:
			break;
		}
	}
	void MainWindow::InitWindowClasses() {
		RegisterWindowClass(CS_VREDRAW | CS_HREDRAW | CS_DROPSHADOW | CS_DBLCLKS , L"MainWindow", WindowEventProc);
	}
}