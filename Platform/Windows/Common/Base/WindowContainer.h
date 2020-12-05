#pragma once
#include "Window.h"
namespace Editor {
	class WindowContainerHolder;
	class WindowContainer {
	protected:
		WindowHolder * mLeftMostChildren, *mRightMostChildren, *mTopMostChildren, *mBottomMostChildren;
	public:
		char mName[64];
		Gdiplus::Rect mRect, mMinRect, mMaxRect, mPredefinedRect;
		WindowContainer * mParentContainer;
		WindowContainerHolder *mLeftMostContainer, *mRightMostContainer, *mTopMostContainer, *mBottomMostContainer;
		WindowContainerHolder *mLeftSiblingContainers, *mRightSiblingContainers, *mTopSiblingContainers, *mBottomSiblingContainers;
		WindowContainer(const char * name);
		virtual ~WindowContainer();
		void SetName(const char * name);
		void SetMinRect(int x, int y, int width, int height);
		void SetMaxRect(int x, int y, int width, int height);
		Gdiplus::Rect GetMinRect() const { return mMinRect; }

		void AddLeftEdgeChildContainer(WindowContainer*container);
		void AddRightEdgeChildContainer(WindowContainer*container);
		void AddBottomEdgeChildContainer(WindowContainer*container);
		void AddTopEdgeChildContainer(WindowContainer*container);

		void AddSiblingContainerAtLeft(WindowContainer*container);
		void AddSiblingContainerAtRight(WindowContainer*container);
		void AddSiblingContainerAtTop(WindowContainer*container);
		void AddSiblingContainerAtBottom(WindowContainer*container);

		void AddChildWindowAt(ChildWindowLocation location, BaseWindow*window);
		void RemoveChildWindowAt(ChildWindowLocation location, BaseWindow*window);
		void Complete();
		int GenerateMinWidthViaChildWindows();
		int GenerateMinHeightViaChildWindows();
		int GenerateMaxWidthViaChildWindows();
		int GenerateMaxHeightViaChildWindows();

		int GenerateMinWidthViaChildContainers();
		int GenerateMinHeightViaChilContainers();
		int GenerateMaxWidthViaChildContainers();
		int GenerateMaxHeightViaChilContainers();
		int GenerateMinWidth();
		int GenerateMinHeight();
		int GenerateMaxWidth();
		int GenerateMaxHeight();
		int GenerateWidth();
		int GenerateHeight();
	public:
		void ExtentContainerFromLeft(int &deltaX, const Gdiplus::Rect *left_rect = nullptr);
		void ExtentChildContainersFromLeft(int &deltaX, const Gdiplus::Rect *left_rect = nullptr);
		void ExtentChildWindowsFromLeft(int &deltaX, const Gdiplus::Rect *left_rect = nullptr);
		void ReduceContainerFromLeft(int &deltaX, const Gdiplus::Rect *left_rect = nullptr);
		void ReduceChildContainersFromLeft(int &deltaX, const Gdiplus::Rect *left_rect = nullptr);
		void ReduceChildWindowsFromLeft(int &deltaX, const Gdiplus::Rect *left_rect = nullptr);

		void ExtentContainerFromRight(int &deltaX, const Gdiplus::Rect *right_rect = nullptr);
		void ExtentChildContainersFromRight(int &deltaX, const Gdiplus::Rect *right_rect = nullptr);
		void ExtentChildWindowsFromRight(int &deltaX, const Gdiplus::Rect *right_rect = nullptr);
		void ReduceContainerFromRight(int &deltaX, const Gdiplus::Rect *right_rect = nullptr);
		void ReduceChildContainersFromRight(int &deltaX, const Gdiplus::Rect *right_rect = nullptr);
		void ReduceChildWindowsFromRight(int &deltaX,int shiftX, const Gdiplus::Rect *right_rect = nullptr);

		void ExtentContainerFromBottom(int &deltaY, const Gdiplus::Rect *bottom_rect = nullptr);
		void ExtentChildContainersFromBottom(int &deltaY, const Gdiplus::Rect *bottom_rect = nullptr);
		void ExtentChildWindowsFromBottom(int &deltaY, const Gdiplus::Rect *bottom_rect = nullptr);
		void ReduceContainerFromBottom(int &deltaY, const Gdiplus::Rect *bottom_rect = nullptr);
		void ReduceChildContainersFromBottom(int &deltaY, const Gdiplus::Rect *bottom_rect = nullptr);
		void ReduceChildWindowsFromBottom(int &deltaY, int shiftY, const Gdiplus::Rect *bottom_rect = nullptr);

		void ExtentContainerFromTop(int &deltaY, const Gdiplus::Rect *top_rect = nullptr);
		void ExtentChildContainersFromTop(int &deltaX, const Gdiplus::Rect *top_rect = nullptr);
		void ExtentChildWindowsFromTop(int &deltaX, const Gdiplus::Rect *top_rect = nullptr);
		void ReduceContainerFromTop(int &deltaY, const Gdiplus::Rect *top_rect = nullptr);
		void ReduceChildContainersFromTop(int &deltaY, const Gdiplus::Rect *top_rect = nullptr);
		void ReduceChildWindowsFromTop(int &deltaY, const Gdiplus::Rect *top_rect = nullptr);
	};
	class WindowContainerHolder : public Alice::LinkedList{
	public:
		WindowContainer*mContainer;
		WindowContainerHolder(WindowContainer*container) {
			mContainer = container;
		}
	};
}