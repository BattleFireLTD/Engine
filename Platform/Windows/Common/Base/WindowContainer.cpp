#include "WindowContainer.h"
#include "Runtime/Debugger/Logger.h"
namespace Editor {
	WindowContainer::WindowContainer(const char * name) {
		SetName(name);
		mLeftMostContainer = nullptr;
		mRightMostContainer = nullptr;
		mTopMostContainer = nullptr;
		mBottomMostContainer = nullptr;
		mLeftMostChildren = nullptr;
		mRightMostChildren = nullptr;
		mTopMostChildren = nullptr;
		mBottomMostChildren = nullptr;
		mLeftSiblingContainers = nullptr;
		mRightSiblingContainers = nullptr;
		mTopSiblingContainers = nullptr;
		mBottomSiblingContainers = nullptr;
		mParentContainer = nullptr;
		mMinRect = {0,0,-1,-1};
		mMaxRect = {0,0,-1,-1};
	}
	WindowContainer::~WindowContainer() {
	}
	void WindowContainer::SetName(const char * name) {
		memset(mName, 0, 64);
		strcpy(mName, name);
	}
	void WindowContainer::SetMinRect(int x, int y, int width, int height) {
		mMinRect = {x,y,width,height};
	}
	void WindowContainer::SetMaxRect(int x, int y, int width, int height) {
		mMaxRect = { x,y,width,height };
	}
	void WindowContainer::AddLeftEdgeChildContainer(WindowContainer*container) {
		container->mParentContainer = this;
		if (mLeftMostContainer == nullptr) {
			mLeftMostContainer = new WindowContainerHolder(container);
		}
		else {
			mLeftMostContainer->PushBack(new WindowContainerHolder(container));
		}
	}
	void WindowContainer::AddRightEdgeChildContainer(WindowContainer*container) {
		container->mParentContainer = this;
		if (mRightMostContainer == nullptr) {
			mRightMostContainer = new WindowContainerHolder(container);
		}
		else {
			mRightMostContainer->PushBack(new WindowContainerHolder(container));
		}
	}
	void WindowContainer::AddBottomEdgeChildContainer(WindowContainer*container) {
		container->mParentContainer = this;
		if (mBottomMostContainer == nullptr) {
			mBottomMostContainer = new WindowContainerHolder(container);
		}
		else {
			mBottomMostContainer->PushBack(new WindowContainerHolder(container));
		}
	}
	void WindowContainer::AddTopEdgeChildContainer(WindowContainer*container) {
		container->mParentContainer = this;
		if (mTopMostContainer == nullptr) {
			mTopMostContainer = new WindowContainerHolder(container);
		}
		else {
			mTopMostContainer->PushBack(new WindowContainerHolder(container));
		}
	}

	void WindowContainer::AddSiblingContainerAtLeft(WindowContainer*container) {
		if (mLeftSiblingContainers == nullptr) {
			mLeftSiblingContainers = new WindowContainerHolder(container);
		}
		else {
			mLeftSiblingContainers->PushBack(new WindowContainerHolder(container));
		}
	}
	void WindowContainer::AddSiblingContainerAtRight(WindowContainer*container) {
		if (mRightSiblingContainers == nullptr) {
			mRightSiblingContainers = new WindowContainerHolder(container);
		}
		else {
			mRightSiblingContainers->PushBack(new WindowContainerHolder(container));
		}
	}
	void WindowContainer::AddSiblingContainerAtBottom(WindowContainer*container) {
		if (mBottomSiblingContainers == nullptr) {
			mBottomSiblingContainers = new WindowContainerHolder(container);
		}
		else {
			mBottomSiblingContainers->PushBack(new WindowContainerHolder(container));
		}
	}
	void WindowContainer::AddSiblingContainerAtTop(WindowContainer*container) {
		if (mTopSiblingContainers == nullptr) {
			mTopSiblingContainers = new WindowContainerHolder(container);
		}
		else {
			mTopSiblingContainers->PushBack(new WindowContainerHolder(container));
		}
	}
	
	void WindowContainer::Complete() {
		//container contains containers
		if (mLeftMostContainer != nullptr) {
			int min_width = GenerateMinWidthViaChildContainers();
			int min_height = GenerateMinHeightViaChilContainers();
			int max_width = GenerateMaxWidthViaChildContainers();
			int max_height = GenerateMaxHeightViaChilContainers();
			mMinRect = { 0,0,min_width,min_height };
			mMaxRect = { 0,0,max_width,max_height };
			mRect.X = mLeftMostContainer->mContainer->mRect.X;
			mRect.Y = mTopMostContainer->mContainer->mRect.Y;
			mRect.Width = GenerateWidth();
			mRect.Height = GenerateHeight();
			Debug("%s[%d,%d,%d,%d] Min[%dx%d] Max[%dx%d]",mName, mRect.X,mRect.Y, mRect.Width, mRect.Height,
				min_width, min_height, max_width, max_height);
		} else {//container contains window
			int min_width = GenerateMinWidthViaChildWindows();
			int min_height = GenerateMinHeightViaChildWindows();
			int max_width = GenerateMaxWidthViaChildWindows();
			int max_height = GenerateMaxHeightViaChildWindows();
			mMinRect = { 0,0,min_width,min_height };
			mMaxRect = { 0,0,max_width,max_height };
			mRect.X = mLeftMostChildren->mWindow->GetX();
			mRect.Y = mTopMostChildren->mWindow->GetY();
			mRect.Width = GenerateWidth();
			mRect.Height = GenerateHeight();
			Debug("%s[%d,%d,%d,%d] Min[%dx%d] Max[%dx%d]", mName, mRect.X, mRect.Y, mRect.Width, mRect.Height,
				min_width, min_height, max_width, max_height);
		}
	}
	int WindowContainer::GenerateWidth() {
		int width = 0;
		//main container
		if (mLeftMostContainer != nullptr) {
			WindowContainerHolder*wh = mLeftMostContainer;
			while (wh != nullptr) {
				width += wh->mContainer->GenerateWidth();
				wh = wh->mContainer->mRightSiblingContainers;
			}
		}
		else {//container contains window
			WindowHolder*wh = mLeftMostChildren;
			while (wh != nullptr) {
				width += wh->mWindow->GetWidth();
				wh = wh->mWindow->mRightSiblingWindows;
			}
		}
		return width;
	}
	int WindowContainer::GenerateHeight() {
		int height = 0;
		//main container
		if (mTopMostContainer != nullptr) {
			WindowContainerHolder*wh = mTopMostContainer;
			while (wh != nullptr) {
				height += wh->mContainer->GenerateHeight();
				wh = wh->mContainer->mBottomSiblingContainers;
			}
		}
		else {//container contains window
			WindowHolder*wh = mTopMostChildren;
			while (wh != nullptr) {
				height += wh->mWindow->GetHeight();
				wh = wh->mWindow->mBottomSiblingWindows;
			}
		}
		return height;
	}
	int WindowContainer::GenerateMinWidthViaChildWindows() {
		WindowHolder*wh = mLeftMostChildren;
		int min_width = -1;
		while (wh != nullptr) {
			int current_min_width = wh->mWindow->GenerateMinWidth();
			if (min_width < current_min_width) {
				min_width = current_min_width;
			}
			wh = wh->Next<WindowHolder>();
		}
		return min_width;
	}
	int WindowContainer::GenerateMinHeightViaChildWindows() {
		WindowHolder*wh = mTopMostChildren;
		int min_height = -1;
		while (wh != nullptr) {
			int current_min_height = wh->mWindow->GenerateMinHeight();
			if (min_height < current_min_height) {
				min_height = current_min_height;
			}
			wh = wh->Next<WindowHolder>();
		}
		return min_height;
	}
	int WindowContainer::GenerateMaxWidthViaChildWindows() {
		WindowHolder*wh = mLeftMostChildren;
		int max_width = 99999;
		while (wh != nullptr) {
			int current_max_width = wh->mWindow->GenerateMaxWidth();
			if (max_width > current_max_width) {
				max_width = current_max_width;
			}
			wh = wh->Next<WindowHolder>();
		}
		return max_width;
	}
	int WindowContainer::GenerateMaxHeightViaChildWindows() {
		WindowHolder*wh = mTopMostChildren;
		int max_height = 99999;
		while (wh != nullptr) {
			int current_max_height = wh->mWindow->GenerateMaxHeight();
			if (max_height > current_max_height) {
				max_height = current_max_height;
			}
			wh = wh->Next<WindowHolder>();
		}
		return max_height;
	}
	int WindowContainer::GenerateMinWidthViaChildContainers() {
		WindowContainerHolder*wh = mLeftMostContainer;
		int min_width = -1;
		while (wh != nullptr) {
			int current_min_width = wh->mContainer->GenerateMinWidth();
			if (min_width < current_min_width) {
				min_width = current_min_width;
			}
			wh = wh->Next<WindowContainerHolder>();
		}
		return min_width;
	}
	int WindowContainer::GenerateMinHeightViaChilContainers() {
		WindowContainerHolder*wh = mTopMostContainer;
		int min_height = -1;
		while (wh != nullptr) {
			int current_min_height = wh->mContainer->GenerateMinHeight();
			if (min_height < current_min_height) {
				min_height = current_min_height;
			}
			wh = wh->Next<WindowContainerHolder>();
		}
		return min_height;
	}
	int WindowContainer::GenerateMaxWidthViaChildContainers() {
		WindowContainerHolder*wh = mLeftMostContainer;
		int max_width = 99999;
		while (wh != nullptr) {
			int current_max_width = wh->mContainer->GenerateMaxWidth();
			if (max_width > current_max_width) {
				max_width = current_max_width;
			}
			wh = wh->Next<WindowContainerHolder>();
		}
		return max_width;
	}
	int WindowContainer::GenerateMaxHeightViaChilContainers() {
		WindowContainerHolder*wh = mTopMostContainer;
		int max_height = 99999;
		while (wh != nullptr) {
			int current_max_height = wh->mContainer->GenerateMaxHeight();
			if (max_height > current_max_height) {
				max_height = current_max_height;
			}
			wh = wh->Next<WindowContainerHolder>();
		}
		return max_height;
	}
	int WindowContainer::GenerateMinWidth() {
		int min_width = mMinRect.Width;
		int right_sibling_min_width = 0;
		WindowContainerHolder*right_sibling = mRightSiblingContainers;
		while (right_sibling != nullptr) {
			int current_right_sibling_window_min_width = right_sibling->mContainer->GenerateMinWidth();
			if (right_sibling_min_width < current_right_sibling_window_min_width) {
				right_sibling_min_width = current_right_sibling_window_min_width;
			}
			right_sibling = right_sibling->Next<WindowContainerHolder>();
		}
		return min_width + right_sibling_min_width;
	}
	int WindowContainer::GenerateMinHeight() {
		int min_height = mMinRect.Height;
		int right_sibling_min_height = 0;
		WindowContainerHolder*bottom_sibling = mBottomSiblingContainers;
		while (bottom_sibling != nullptr) {
			int current_right_sibling_window_min_height = bottom_sibling->mContainer->GenerateMinHeight();
			if (right_sibling_min_height < current_right_sibling_window_min_height) {
				right_sibling_min_height = current_right_sibling_window_min_height;
			}
			bottom_sibling = bottom_sibling->Next<WindowContainerHolder>();
		}
		return min_height + right_sibling_min_height;
	}
	int WindowContainer::GenerateMaxWidth() {
		int max_width = mMaxRect.Width;
		int right_sibling_max_width = 999999;
		WindowContainerHolder*sibling = mRightSiblingContainers;
		while (sibling != nullptr) {
			int current_right_sibling_container_max_width = sibling->mContainer->GenerateMaxWidth();
			if (right_sibling_max_width > current_right_sibling_container_max_width) {
				right_sibling_max_width = current_right_sibling_container_max_width;
			}
			sibling = sibling->Next<WindowContainerHolder>();
		}
		return (max_width==-1||right_sibling_max_width==-1) ? -1 : (max_width + right_sibling_max_width);
	}
	int WindowContainer::GenerateMaxHeight() {
		int max_height = mMaxRect.Height;
		int right_sibling_max_height = 999999;
		WindowContainerHolder*sibling = mBottomSiblingContainers;
		while (sibling != nullptr) {
			int current_bottom_sibling_container_max_height = sibling->mContainer->GenerateMaxHeight();
			if (right_sibling_max_height > current_bottom_sibling_container_max_height) {
				right_sibling_max_height = current_bottom_sibling_container_max_height;
			}
			sibling = sibling->Next<WindowContainerHolder>();
		}
		return (max_height==-1||right_sibling_max_height==-1) ? -1 : (max_height + right_sibling_max_height);
	}
	void WindowContainer::AddChildWindowAt(ChildWindowLocation location, BaseWindow*window) {
		window->mParentContainer = this;
		switch (location) {
		case Editor::kChildWindowLocationLeftEdge:
			if (mLeftMostChildren == nullptr) {
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
	void WindowContainer::RemoveChildWindowAt(ChildWindowLocation location, BaseWindow*window) {

	}
	void WindowContainer::ExtentContainerFromLeft(int &deltaX, const Gdiplus::Rect *left_rect) {
		mRect.X = left_rect == nullptr ? mRect.X : left_rect->GetRight();
		mRect.Width += deltaX;
		if (mLeftMostContainer != nullptr) {
			ExtentChildContainersFromLeft(deltaX, left_rect);
		}
		if (mRightSiblingContainers != nullptr) {
			WindowContainerHolder*wch = mRightSiblingContainers;
			while (wch != nullptr) {
				int temp_deltaX = 0;
				wch->mContainer->ExtentContainerFromLeft(temp_deltaX, &mRect);
				wch = wch->Next<WindowContainerHolder>();
			}
		}
		if (mLeftMostChildren != nullptr) {
			ExtentChildWindowsFromLeft(deltaX);
		}
	}
	void WindowContainer::ExtentChildContainersFromLeft(int &deltaX, const Gdiplus::Rect *left_rect /* = nullptr */) {
		WindowContainerHolder*wch = mLeftMostContainer;
		while (wch != nullptr) {
			int temp_deltaX = deltaX;
			wch->mContainer->ExtentContainerFromLeft(temp_deltaX);
			wch = wch->Next<WindowContainerHolder>();
		}
	}
	void WindowContainer::ExtentChildWindowsFromLeft(int &deltaX, const Gdiplus::Rect *left_rect /* = nullptr */) {
		WindowHolder*wh = mLeftMostChildren;
		while (wh != nullptr) {
			int temp_deltaX = deltaX;
			wh->mWindow->ExtentWindowFromLeft(temp_deltaX,nullptr,&mRect);
			wh = wh->Next<WindowHolder>();
		}
		deltaX = 0;
	}
	void WindowContainer::ExtentContainerFromRight(int &deltaX, const Gdiplus::Rect *right_rect) {
		mRect.Width += deltaX;
		if (mRightMostContainer != nullptr) {
			ExtentChildContainersFromRight(deltaX);
		}
		if (mRightMostChildren != nullptr) {
			ExtentChildWindowsFromRight(deltaX);
		}
	}
	void WindowContainer::ExtentChildContainersFromRight(int &deltaX, const Gdiplus::Rect *right_rect /* = nullptr */) {
		WindowContainerHolder*wch = mRightMostContainer;
		while (wch != nullptr) {
			int temp_deltaX = deltaX;
			wch->mContainer->ExtentContainerFromRight(temp_deltaX);
			wch = wch->Next<WindowContainerHolder>();
		}
	}
	void WindowContainer::ExtentChildWindowsFromRight(int &deltaX, const Gdiplus::Rect *right_rect /* = nullptr */) {
		WindowHolder*wh = mRightMostChildren;
		while (wh != nullptr) {
			int temp_deltaX = deltaX;
			wh->mWindow->ExtentWindowFromRight(temp_deltaX);
			wh = wh->Next<WindowHolder>();
		}
		deltaX = 0;
	}
	void WindowContainer::ExtentContainerFromBottom(int &deltaY, const Gdiplus::Rect *bottom_rect) {
		mRect.Height += deltaY;
		if (mBottomMostContainer != nullptr) {
			ExtentChildContainersFromBottom(deltaY);
		}
		if (mBottomMostChildren != nullptr) {
			ExtentChildWindowsFromBottom(deltaY);
		}
	}
	void WindowContainer::ExtentChildContainersFromBottom(int &deltaY, const Gdiplus::Rect *bottom_rect /* = nullptr */) {
		WindowContainerHolder*wch = mBottomMostContainer;
		while (wch != nullptr) {
			int temp_deltaY = deltaY;
			wch->mContainer->ExtentContainerFromBottom(temp_deltaY);
			wch = wch->Next<WindowContainerHolder>();
		}
	}
	void WindowContainer::ExtentChildWindowsFromBottom(int &deltaY, const Gdiplus::Rect *bottom_rect /* = nullptr */) {
		WindowHolder*wh = mBottomMostChildren;
		while (wh != nullptr) {
			int temp_deltaY = deltaY;
			wh->mWindow->ExtentWindowFromBottom(temp_deltaY);
			wh = wh->Next<WindowHolder>();
		}
		deltaY = 0;
	}
	void WindowContainer::ExtentContainerFromTop(int &deltaY, const Gdiplus::Rect *top_rect) {
		int max_consumable_deltaY = (mMaxRect.Height==-1)?deltaY:mMaxRect.Height-mRect.Height;
		deltaY -= max_consumable_deltaY;
		mRect.Y = top_rect == nullptr ? mRect.Y : top_rect->GetBottom();
		mRect.Height += max_consumable_deltaY;
		if (mTopMostContainer != nullptr) {
			ExtentChildContainersFromTop(max_consumable_deltaY, top_rect);
		}
		if (mBottomSiblingContainers != nullptr) {
			WindowContainerHolder*wch = mBottomSiblingContainers;
			while (wch != nullptr) {
				int temp_deltaX = deltaY;
				wch->mContainer->ExtentContainerFromTop(temp_deltaX, &mRect);
				wch = wch->Next<WindowContainerHolder>();
			}
		}
		if (mTopMostChildren != nullptr) {
			ExtentChildWindowsFromTop(max_consumable_deltaY);
		}
	}
	void WindowContainer::ExtentChildContainersFromTop(int &deltaY, const Gdiplus::Rect *top_rect /* = nullptr */) {
		WindowContainerHolder*wch = mTopMostContainer;
		while (wch != nullptr) {
			int temp_deltaY = deltaY;
			wch->mContainer->ExtentContainerFromTop(temp_deltaY);
			wch = wch->Next<WindowContainerHolder>();
		}
	}
	void WindowContainer::ExtentChildWindowsFromTop(int &deltaY, const Gdiplus::Rect *top_rect /* = nullptr */) {
		WindowHolder*wh = mTopMostChildren;
		while (wh != nullptr) {
			int temp_deltaY = deltaY;
			wh->mWindow->ExtentWindowFromTop(temp_deltaY, nullptr, &mRect);
			wh = wh->Next<WindowHolder>();
		}
		deltaY = 0;
	}
	void WindowContainer::ReduceContainerFromLeft(int &deltaX, const Gdiplus::Rect *left_rect) {
		int left_rect_right_edge_pos = left_rect == nullptr ? mRect.X : left_rect->GetRight(); 
		int max_cosumable_deltaX = mMinRect.Width==-1? 32- mRect.Width : mMinRect.Width - mRect.Width;
		int current_container_cosumed_deltaX = deltaX > max_cosumable_deltaX ? deltaX : max_cosumable_deltaX;
		mRect.X = left_rect_right_edge_pos;
		mRect.Width += current_container_cosumed_deltaX;
		deltaX -= current_container_cosumed_deltaX;
		if (mLeftMostContainer != nullptr) {
			ReduceChildContainersFromLeft(current_container_cosumed_deltaX);
		}
		if (mRightSiblingContainers != nullptr) {
			WindowContainerHolder*wch = mRightSiblingContainers;
			while (wch != nullptr) {
				int temp_deltaX = deltaX;
				wch->mContainer->ReduceContainerFromLeft(temp_deltaX, &mRect);
				wch = wch->Next<WindowContainerHolder>();
			}
		}
		if (mLeftMostChildren != nullptr) {
			ReduceChildWindowsFromLeft(current_container_cosumed_deltaX);
		}
	}
	void WindowContainer::ReduceChildContainersFromLeft(int &deltaX, const Gdiplus::Rect *left_rect /* = nullptr */) {
		WindowContainerHolder*wch = mLeftMostContainer;
		while (wch != nullptr) {
			int temp_deltaX = deltaX;
			wch->mContainer->ReduceContainerFromLeft(temp_deltaX);
			wch = wch->Next<WindowContainerHolder>();
		}
	}
	void WindowContainer::ReduceChildWindowsFromLeft(int &deltaX, const Gdiplus::Rect *left_rect /* = nullptr */) {
		WindowHolder*wh = mLeftMostChildren;
		while (wh != nullptr) {
			int temp_deltaX = deltaX;
			wh->mWindow->ReduceWindowFromLeft(temp_deltaX,nullptr,&mRect);
			wh = wh->Next<WindowHolder>();
		}
		deltaX = 0;
	}
	void WindowContainer::ReduceContainerFromRight(int &deltaX, const Gdiplus::Rect *right_rect) {
		int max_cosumable_deltaX = mMinRect.Width == -1 ? 32 - mRect.Width : mMinRect.Width - mRect.Width;
		int current_container_cosumed_deltaX = deltaX > max_cosumable_deltaX ? deltaX : max_cosumable_deltaX;
		deltaX -= current_container_cosumed_deltaX;
		mRect.X += deltaX;
		mRect.Width += current_container_cosumed_deltaX;
		if (mRightMostContainer != nullptr) {
			ReduceChildContainersFromRight(current_container_cosumed_deltaX);
		}
		if (mLeftSiblingContainers != nullptr) {
			WindowContainerHolder*wch = mLeftSiblingContainers;
			while (wch != nullptr) {
				int temp_deltaX = deltaX;
				wch->mContainer->ReduceContainerFromRight(temp_deltaX, &mRect);
				wch = wch->Next<WindowContainerHolder>();
			}
		}
		if (mRightMostChildren != nullptr) {
			ReduceChildWindowsFromRight(current_container_cosumed_deltaX,deltaX);
		}
	}
	void WindowContainer::ReduceChildContainersFromRight(int &deltaX, const Gdiplus::Rect *right_rect /* = nullptr */) {
		WindowContainerHolder*wch = mRightMostContainer;
		while (wch != nullptr) {
			int temp_deltaX = deltaX;
			wch->mContainer->ReduceContainerFromRight(temp_deltaX);
			wch = wch->Next<WindowContainerHolder>();
		}
	}
	void WindowContainer::ReduceChildWindowsFromRight(int &deltaX, int shiftX, const Gdiplus::Rect *right_rect /* = nullptr */) {
		WindowHolder*wh = mRightMostChildren;
		while (wh != nullptr) {
			int temp_deltaX = deltaX;
			wh->mWindow->ReduceWindowFromRight(temp_deltaX, shiftX, &mRect);
			wh = wh->Next<WindowHolder>();
		}
		deltaX = 0;
	}
	void WindowContainer::ReduceContainerFromBottom(int &deltaY, const Gdiplus::Rect *bottom_rect) {
		int max_cosumable_deltaY = mMinRect.Height == -1 ? 32 - mRect.Height : mMinRect.Height - mRect.Height;
		int current_container_cosumed_deltaY = deltaY > max_cosumable_deltaY ? deltaY : max_cosumable_deltaY;
		deltaY -= current_container_cosumed_deltaY;
		mRect.Y += deltaY;
		mRect.Height += current_container_cosumed_deltaY;
		if (mBottomMostContainer != nullptr) {
			ReduceChildContainersFromBottom(current_container_cosumed_deltaY);
		}
		if (mTopSiblingContainers != nullptr) {
			WindowContainerHolder*wch = mTopSiblingContainers;
			while (wch != nullptr) {
				int temp_deltaY = deltaY;
				wch->mContainer->ReduceContainerFromBottom(temp_deltaY, &mRect);
				wch = wch->Next<WindowContainerHolder>();
			}
		}
		if (mBottomMostChildren != nullptr) {
			ReduceChildWindowsFromBottom(current_container_cosumed_deltaY, deltaY);
		}
	}
	void WindowContainer::ReduceChildContainersFromBottom(int &deltaY, const Gdiplus::Rect *bottom_rect /* = nullptr */) {
		WindowContainerHolder*wch = mBottomMostContainer;
		while (wch != nullptr) {
			int temp_deltaY = deltaY;
			wch->mContainer->ReduceContainerFromBottom(temp_deltaY);
			wch = wch->Next<WindowContainerHolder>();
		}
	}
	void WindowContainer::ReduceChildWindowsFromBottom(int &deltaY, int shiftY, const Gdiplus::Rect *bottom_rect /* = nullptr */) {
		WindowHolder*wh = mBottomMostChildren;
		while (wh != nullptr) {
			int temp_deltaY = deltaY;
			wh->mWindow->ReduceWindowFromBottom(temp_deltaY, shiftY, &mRect);
			wh = wh->Next<WindowHolder>();
		}
		deltaY = 0;
	}
	void WindowContainer::ReduceContainerFromTop(int &deltaY, const Gdiplus::Rect *top_rect) {
		int top_rect_bottom_edge_pos = top_rect == nullptr ? mRect.Y : top_rect->GetBottom();
		int max_cosumable_deltaY = mMinRect.Height==-1 ? 32 - mRect.Height : mMinRect.Height - mRect.Height;
		int current_container_cosumed_deltaY = deltaY > max_cosumable_deltaY ? deltaY : max_cosumable_deltaY;
		deltaY -= current_container_cosumed_deltaY;
		mRect.Y = top_rect_bottom_edge_pos;
		mRect.Height += current_container_cosumed_deltaY;
		if (mTopMostContainer != nullptr) {
			ReduceChildContainersFromTop(current_container_cosumed_deltaY);
		}
		if (mBottomSiblingContainers != nullptr) {
			WindowContainerHolder*wch = mBottomSiblingContainers;
			while (wch != nullptr) {
				int temp_deltaY = deltaY;
				wch->mContainer->ReduceContainerFromTop(temp_deltaY, &mRect);
				wch = wch->Next<WindowContainerHolder>();
			}
		}
		if (mTopMostChildren != nullptr) {
			ReduceChildWindowsFromTop(current_container_cosumed_deltaY);
		}
	}
	void WindowContainer::ReduceChildContainersFromTop(int &deltaY, const Gdiplus::Rect *top_rect /* = nullptr */) {
		WindowContainerHolder*wch = mTopMostContainer;
		while (wch != nullptr) {
			int temp_deltaY = deltaY;
			wch->mContainer->ReduceContainerFromTop(temp_deltaY);
			wch = wch->Next<WindowContainerHolder>();
		}
	}
	void WindowContainer::ReduceChildWindowsFromTop(int &deltaY, const Gdiplus::Rect *top_rect /* = nullptr */) {
		WindowHolder*wh = mTopMostChildren;
		while (wh != nullptr) {
			int temp_deltaY = deltaY;
			wh->mWindow->ReduceWindowFromTop(temp_deltaY, top_rect,&mRect);
			wh = wh->Next<WindowHolder>();
		}
		deltaY = 0;
	}
}