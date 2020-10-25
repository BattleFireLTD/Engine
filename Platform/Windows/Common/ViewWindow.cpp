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

		mLeftSiblingWindows = nullptr;
		mRightSiblingWindows = nullptr;
		mTopSiblingWindows = nullptr;
		mBottomSiblingWindows = nullptr;

		SetWindowName("ViewWindow");
	}
	void ViewWindow::DrawContent(Gdiplus::Graphics&painter) {
		if (mUIRoot != nullptr) {
			mUIRoot->DrawRecursively(painter);
		}
	}
	void ViewWindow::OnParentPaint(Gdiplus::Graphics&painter) {
		RECT parent_rect, self_rect;
		GetWindowRect(mParent->GetHwnd(), &parent_rect);
		GetWindowRect(mhWnd, &self_rect);
		painter.TranslateTransform(float(self_rect.left - parent_rect.left), float(self_rect.top - parent_rect.top));
		painter.SetClip(Gdiplus::Rect(0, 0, mRect.Width, mRect.Height));
		painter.Clear(mBKGColor);
		painter.ResetClip();
		if (mUIRoot != nullptr) {
			mUIRoot->DrawRecursively(painter);
		}
		painter.ResetTransform();
	}
	void ViewWindow::OnClearBKG(Gdiplus::Graphics&painter) {
		painter.Clear(mBKGColor);
	}
	void ViewWindow::OnCommand(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		UINT nNotify = HIWORD(wParam);
		UINT commandID = LOWORD(wParam);
		mUIRoot->ProcessEvent(commandID);
	}
	void ViewWindow::OnSize(WPARAM wParam, LPARAM lParam, void*reserved) {
		GetRelativeWindowRect(mRect);
		//MoveWindow(0,20, 1280, mRect.Height);
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
			MarkDirty();
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
			MarkDirty();
			ReleaseCapture();
		}
		else {
			if (mLastTouchObject != nullptr) {
				mLastTouchObject->OnTouchCanceled(x, y);
				mLastTouchObject = nullptr;
				MarkDirty();
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
				MarkDirty();
			}
		}
		else {
			if (mLastHoverObject != nullptr) {
				mLastHoverObject->OnTouchLeave(x, y);
				mLastHoverObject = nullptr;
				MarkDirty();
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
			MarkDirty();
		}
		DoubleBufferedWindow::OnMouseLeave(wParam, lParam, reserved);
	}

	void ViewWindow::Init(BaseWindow*parent){
		DWORD windowStyle = WS_CHILD | WS_THICKFRAME ;
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
	void ViewWindow::OnParentResized(int width, int height, int deltaX , int deltaY , BaseWindow::WindowResizeDirection direction) {
		//int x = 0, y = 0, w = width, h = height;
		//if (mbFixedPos){
		//	x = mFixedRect.X;
		//	y = mFixedRect.Y;
		//}
		//if (mbFixedWidth){
		//	w = mFixedRect.Width;
		//}
		//if (mbFixedHeight){
		//	h = mFixedRect.Height;
		//}
		//if (w==mRect.Width&&h==mRect.Height){
		//	return;
		//}
		//if (mUIRoot != nullptr) {
		//	mUIRoot->OnContainerSizeChangedRecursively(w, h);
		//}
		//MoveWindow(x, y, w, h);
	}
	void ViewWindow::TryToExtentWindowHorizontallyFromLeft(int & deltaX) {
		deltaX = 0;
	}
	void ViewWindow::TryToReduceWindowHorizontallyFromLeft(int & deltaX) {
		//Gdiplus::Rect rect;
		//GetRelativeWindowRect(rect);
		//MoveWindow(rect.X, rect.Y, rect.Width + deltaX, rect.Height);
		//deltaX = 0;
	}
	void ViewWindow::TryToExtentWindowHorizontallyFromRight(int & deltaX) {
		deltaX = 0;
	}
	void ViewWindow::TryToReduceWindowHorizontallyFromRight(int & deltaX) {

	}
	void ViewWindow::TryToExtentWindowVerticallyFromTop(int & deltaY) {

	}
	void ViewWindow::TryToReduceWindowVerticallyFromTop(int & deltaY) {

	}
	void ViewWindow::TryToExtentWindowVerticallyFromBottom(int & deltaY) {

	}
	void ViewWindow::TryToReduceWindowVerticallyFromBottom(int & deltaY) {

	}
	void ViewWindow::ExtentWindowHorizontallyFromLeft(int deltaX) {
		Gdiplus::Rect rect;
		GetRelativeWindowRect(rect);
		MoveWindow(rect.X, rect.Y, rect.Width + deltaX, rect.Height);
	}
	void ViewWindow::ReduceWindowHorizontallyFromLeft(int deltaX) {
		//Gdiplus::Rect rect;
		//GetRelativeWindowRect(rect);
		//MoveWindow(rect.X, rect.Y, rect.Width + deltaX, rect.Height);
		//deltaX = 0;
	}
	void ViewWindow::ExtentWindowHorizontallyFromRight(int deltaX) {
		Gdiplus::Rect rect;
		GetRelativeWindowRect(rect);
		MoveWindow(rect.X, rect.Y, rect.Width + deltaX, rect.Height);
		Debug("%s ExtentWindowHorizontallyFromRight", mName);
	}
	void ViewWindow::ReduceWindowHorizontallyFromRight(int deltaX) {

	}
	void ViewWindow::ExtentWindowVerticallyFromTop(int deltaY) {

	}
	void ViewWindow::ReduceWindowVerticallyFromTop(int deltaY) {

	}
	void ViewWindow::ExtentWindowVerticallyFromBottom(int deltaY) {

	}
	void ViewWindow::ReduceWindowVerticallyFromBottom(int deltaY) {

	}
	void ViewWindow::MarkDirty() {
		mParent->MarkDirty();
	}
	void ViewWindow::AddWindowAtLeft(BaseWindow*window) {
		if (mLeftSiblingWindows == nullptr) {
			mLeftSiblingWindows = new WindowHolder(window);
		}
		else {
			mLeftSiblingWindows->PushBack(new WindowHolder(window));
		}
	}
	void ViewWindow::AddWindowAtRight(BaseWindow*window) {
		if (mRightSiblingWindows == nullptr) {
			mRightSiblingWindows = new WindowHolder(window);
		}
		else {
			mRightSiblingWindows->PushBack(new WindowHolder(window));
		}
	}
	void ViewWindow::AddWindowAtTop(BaseWindow*window) {
		if (mTopSiblingWindows == nullptr) {
			mTopSiblingWindows = new WindowHolder(window);
		}
		else {
			mTopSiblingWindows->PushBack(new WindowHolder(window));
		}
	}
	void ViewWindow::AddWindowAtBottom(BaseWindow*window) {
		if (mBottomSiblingWindows == nullptr) {
			mBottomSiblingWindows = new WindowHolder(window);
		}
		else {
			mBottomSiblingWindows->PushBack(new WindowHolder(window));
		}
	}
	void ViewWindow::InitWindowClasses() {
		RegisterWindowClass(CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS , L"ViewWindow", WindowEventProc);
	}
}