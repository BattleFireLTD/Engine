#include "ViewWindow.h"
#include "Base/WindowContainer.h"
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

		mContainer = nullptr;

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
		//Debug("ViewWindow::OnParentPaint %s %d,%d,%d,%d",mName, self_rect.left - parent_rect.left, self_rect.top - parent_rect.top, mRect.Width, mRect.Height);
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
	void ViewWindow::OnParentResized(int width, int height) {
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

	LRESULT ViewWindow::OnSizing(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		RECT * ptr_new_rect = (RECT*)lParam;
		RECT current_rect;
		GetWindowRect(mhWnd, &current_rect);
		int new_width = ptr_new_rect->right - ptr_new_rect->left;
		int new_height = ptr_new_rect->bottom - ptr_new_rect->top;
		int last_width = current_rect.right - current_rect.left;
		int last_height = current_rect.bottom - current_rect.top;
		switch (wParam) {
		case WMSZ_LEFT: {
			int deltaX = new_width - last_width;
			if (deltaX > 0) {
				ExtentWindowFromLeft(deltaX);
			}
			else if (deltaX < 0) {
				ReduceWindowFromLeft(deltaX);
			}
			ptr_new_rect->left += deltaX;
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
			ptr_new_rect->right -= deltaX;
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
			ptr_new_rect->top += deltaY;
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
	void ViewWindow::ExtentWindowFromLeft(int &deltaX) {
		if (mLeftSiblingWindows!=nullptr){//ok
			int consumed_deltaX = deltaX;
			WindowHolder * wh = mLeftSiblingWindows;
			while (wh!=nullptr){
				int current_sibling_min_width = wh->mWindow->GetMinWidth();
				int current_width = wh->mWindow->GetWidth();
				int max_consumable_width = current_width - current_sibling_min_width;
				if (consumed_deltaX > max_consumable_width){
					consumed_deltaX = max_consumable_width;
				}
				wh = wh->Next<WindowHolder>();
			}
			deltaX -= consumed_deltaX;
			if (consumed_deltaX > 0) {
				wh = mLeftSiblingWindows;
				while (wh != nullptr) {
					Gdiplus::Rect rect;
					wh->mWindow->GetRelativeWindowRect(rect);
					wh->mWindow->MoveWindow(rect.X, rect.Y, rect.Width - consumed_deltaX, rect.Height);
					wh = wh->Next<WindowHolder>();
				}
			}
		}
		else {
			WindowContainer*upper_level_container = mParentContainer;
			while (upper_level_container!=nullptr){
				if (upper_level_container->mLeftSiblingContainers!=nullptr){
					break;
				}
				upper_level_container = upper_level_container->mParentContainer;
			}
			if (upper_level_container != nullptr) {
				int consumed_deltaX = deltaX;
				WindowContainerHolder * wch = upper_level_container->mLeftSiblingContainers;
				while (wch != nullptr) {
					int sibling_min_width = wch->mContainer->mMinRect.Width;
					int sibling_width = wch->mContainer->mRect.Width;
					int max_consumable_width = sibling_width - sibling_min_width;
					if (consumed_deltaX > max_consumable_width) {
						consumed_deltaX = max_consumable_width;
					}
					wch = wch->Next<WindowContainerHolder>();
				}
				deltaX -= consumed_deltaX;
				if (consumed_deltaX > 0) {
					wch = upper_level_container->mLeftSiblingContainers;
					while (wch != nullptr) {
						int temp_deltaX = -consumed_deltaX;
						wch->mContainer->ReduceContainerFromRight(temp_deltaX);
						wch = wch->Next<WindowContainerHolder>();
					}
				}
			}
			else {
				Error("Extent View Window From Left,no left content %s",mName);
			}
		}
	}
	void ViewWindow::ExtentWindowFromRight(int &deltaX) {
		if (mRightSiblingWindows != nullptr) {//ok
			int consumed_deltaX = deltaX;
			WindowHolder * wh = mRightSiblingWindows;
			while (wh != nullptr) {
				int current_sibling_min_width = wh->mWindow->GetMinWidth();
				int current_width = wh->mWindow->GetWidth();
				int max_consumable_width = current_width - current_sibling_min_width;
				if (consumed_deltaX > max_consumable_width) {
					consumed_deltaX = max_consumable_width;
				}
				wh = wh->Next<WindowHolder>();
			}
			deltaX -= consumed_deltaX;
			if (consumed_deltaX > 0) {
				wh = mRightSiblingWindows;
				while (wh != nullptr) {
					Gdiplus::Rect rect;
					wh->mWindow->GetRelativeWindowRect(rect);
					wh->mWindow->MoveWindow(rect.X + consumed_deltaX, rect.Y, rect.Width - consumed_deltaX, rect.Height);
					wh = wh->Next<WindowHolder>();
				}
			}
		}
		else {//ok
			WindowContainer*upper_level_container = mParentContainer;
			while (upper_level_container != nullptr) {
				if (upper_level_container->mRightSiblingContainers != nullptr) {
					break;
				}
				upper_level_container = upper_level_container->mParentContainer;
			}
			if (upper_level_container != nullptr) {
				int consumed_deltaX = deltaX;
				int max_consumable_width = upper_level_container->mRightSiblingContainers->mContainer->mRect.Width - upper_level_container->mRightSiblingContainers->mContainer->mMinRect.Width;
				if (consumed_deltaX > max_consumable_width) {
					consumed_deltaX = max_consumable_width;
				}
				deltaX -= consumed_deltaX;
				if (consumed_deltaX>0) {
					int temp_deltaX = consumed_deltaX;
					upper_level_container->ExtentContainerFromRight(temp_deltaX);
					temp_deltaX = -consumed_deltaX;
					upper_level_container->mRightSiblingContainers->mContainer->ReduceContainerFromLeft(temp_deltaX,&upper_level_container->mRect);
				}
			}
			else {
				Error("Extent View Window From Right,no left content %s", mName);
			}
		}
	}
	void ViewWindow::ExtentWindowFromBottom(int &deltaY) {
		if (mBottomSiblingWindows != nullptr) {
		}
	}
	void ViewWindow::ExtentWindowFromTop(int &deltaY) {
		if (mTopSiblingWindows != nullptr) {
		}
		else {
			WindowContainer*upper_level_container = mParentContainer;
			while (upper_level_container != nullptr) {
				if (upper_level_container->mTopSiblingContainers != nullptr) {
					break;
				}
				upper_level_container = upper_level_container->mParentContainer;
			}
			if (upper_level_container != nullptr) {
				int consumed_deltaY = deltaY;
				int max_consumable_height = upper_level_container->mTopSiblingContainers->mContainer->mRect.Height - upper_level_container->mTopSiblingContainers->mContainer->mMinRect.Height;
				if (consumed_deltaY > max_consumable_height) {
					consumed_deltaY = max_consumable_height;
				}
				deltaY -= consumed_deltaY; 
				if (consumed_deltaY>0)
				{
					int temp_deltaY = -consumed_deltaY;
					upper_level_container->mTopSiblingContainers->mContainer->ReduceContainerFromBottom(temp_deltaY);
					temp_deltaY = consumed_deltaY;
					upper_level_container->ExtentContainerFromTop(temp_deltaY, &upper_level_container->mTopSiblingContainers->mContainer->mRect);
				}
			}
		}
	}
	void ViewWindow::ReduceWindowFromLeft(int &deltaX) {
		if (mLeftSiblingWindows != nullptr) {//ok
			int consumed_deltaX = deltaX;
			WindowHolder * wh = mLeftSiblingWindows;
			while (wh != nullptr) {
				Gdiplus::Rect rect;
				wh->mWindow->GetRelativeWindowRect(rect);
				wh->mWindow->MoveWindow(rect.X, rect.Y, rect.Width - consumed_deltaX, rect.Height);
				wh = wh->Next<WindowHolder>();
			}
			deltaX = 0;
		}
		else {//ok
			WindowContainer*upper_level_container = mParentContainer;
			while (upper_level_container != nullptr) {
				if (upper_level_container->mLeftSiblingContainers != nullptr) {
					break;
				}
				upper_level_container = upper_level_container->mParentContainer;
			}
			if (upper_level_container != nullptr) {
				int consumed_deltaX = deltaX;
				deltaX -= consumed_deltaX;
				int temp_deltaX = -consumed_deltaX;
				upper_level_container->mLeftSiblingContainers->mContainer->ExtentContainerFromRight(temp_deltaX);
				temp_deltaX = consumed_deltaX;
				upper_level_container->ReduceContainerFromLeft(temp_deltaX,&upper_level_container->mLeftSiblingContainers->mContainer->mRect);
			}
			else {
				Error("Extent View Window From Right,no left content %s", mName);
			}
		}
	}
	void ViewWindow::ReduceWindowFromRight(int &deltaX) {
		if (mRightSiblingWindows != nullptr) {//ok
			int consumed_deltaX = deltaX;
			WindowHolder * wh = mRightSiblingWindows;
			while (wh != nullptr) {
				Gdiplus::Rect rect;
				wh->mWindow->GetRelativeWindowRect(rect);
				wh->mWindow->MoveWindow(rect.X + consumed_deltaX, rect.Y, rect.Width - consumed_deltaX, rect.Height);
				wh = wh->Next<WindowHolder>();
			}
			deltaX = 0;
		}else if (mParentContainer!=nullptr){//ok
			WindowContainer*upper_level_container = mParentContainer;
			while (upper_level_container != nullptr) {
				if (upper_level_container->mRightSiblingContainers != nullptr) {
					break;
				}
				upper_level_container = upper_level_container->mParentContainer;
			}
			if (upper_level_container != nullptr) {
				int consumed_deltaX = deltaX;
				int max_consumable_width = upper_level_container->mMinRect.Width - upper_level_container->mRect.Width;
				if (consumed_deltaX < max_consumable_width) {
					consumed_deltaX = max_consumable_width;
				}
				deltaX -= consumed_deltaX;
				int temp_deltaX = consumed_deltaX;
				upper_level_container->ReduceContainerFromRight(temp_deltaX);
				temp_deltaX = -consumed_deltaX;
				upper_level_container->mRightSiblingContainers->mContainer->ExtentContainerFromLeft(temp_deltaX, &upper_level_container->mRect);
			}
		}
	}
	void ViewWindow::ReduceWindowFromBottom(int &deltaY) {
		if (mBottomSiblingWindows != nullptr) {
		}
	}
	void ViewWindow::ReduceWindowFromTop(int &deltaY) {
		if (mTopSiblingWindows != nullptr) {
		}
		else {
			WindowContainer*upper_level_container = mParentContainer;
			while (upper_level_container != nullptr) {
				if (upper_level_container->mTopSiblingContainers != nullptr) {
					break;
				}
				upper_level_container = upper_level_container->mParentContainer;
			}
			if (upper_level_container != nullptr) {
				int consumed_deltaY = deltaY;
				deltaY -= consumed_deltaY;
				int temp_deltaY = -consumed_deltaY;
				upper_level_container->mTopSiblingContainers->mContainer->ExtentContainerFromBottom(temp_deltaY);
				temp_deltaY = consumed_deltaY;
				upper_level_container->ExtentContainerFromTop(temp_deltaY, &upper_level_container->mTopSiblingContainers->mContainer->mRect);
			}
		}
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