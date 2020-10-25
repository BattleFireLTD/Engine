#pragma once
#include "Base/DoubleBufferedWindow.h"
#include "Control/UINode.h"
namespace Editor {
	class ViewWindow :public DoubleBufferedWindow {
	protected:
		bool mbDraging,mbFixedPos,mbFixedWidth,mbFixedHeight;
		Gdiplus::Rect mFixedRect;
		Gdiplus::Point mDeltaWhenDrag;
		UINode * mUIRoot, *mLastTouchObject, *mLastHoverObject;
		WindowHolder *mLeftSiblingWindows, *mRightSiblingWindows, *mTopSiblingWindows, *mBottomSiblingWindows;
	protected:
		virtual void OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMouseLeave(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnSize(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnCommand(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void DrawContent(Gdiplus::Graphics&painter);
		virtual void OnClearBKG(Gdiplus::Graphics&painter);
		virtual void OnPaint(const Gdiplus::Rect &rect_need_update) {}
		virtual void OnParentPaint(Gdiplus::Graphics&painter);
	public:
		ViewWindow();
		virtual void OnParentResized(int width, int height, int deltaX = 0, int deltaY = 0, BaseWindow::WindowResizeDirection d = BaseWindow::kWindowResizeDirectionNone);
		virtual void TryToExtentWindowHorizontallyFromLeft(int & deltaX);
		virtual void TryToReduceWindowHorizontallyFromLeft(int & deltaX);
		virtual void TryToExtentWindowHorizontallyFromRight(int & deltaX);
		virtual void TryToReduceWindowHorizontallyFromRight(int & deltaX);
		virtual void TryToExtentWindowVerticallyFromTop(int & deltaY);
		virtual void TryToReduceWindowVerticallyFromTop(int & deltaY);
		virtual void TryToExtentWindowVerticallyFromBottom(int & deltaY);
		virtual void TryToReduceWindowVerticallyFromBottom(int & deltaY);

		virtual void ExtentWindowHorizontallyFromLeft(int deltaX);
		virtual void ReduceWindowHorizontallyFromLeft(int deltaX);
		virtual void ExtentWindowHorizontallyFromRight(int deltaX);
		virtual void ReduceWindowHorizontallyFromRight(int deltaX);
		virtual void ExtentWindowVerticallyFromTop(int deltaY);
		virtual void ReduceWindowVerticallyFromTop(int deltaY);
		virtual void ExtentWindowVerticallyFromBottom(int deltaY);
		virtual void ReduceWindowVerticallyFromBottom(int deltaY);
		virtual void MarkDirty();
		void AddWindowAtLeft(BaseWindow*window);
		void AddWindowAtRight(BaseWindow*window);
		void AddWindowAtTop(BaseWindow*window);
		void AddWindowAtBottom(BaseWindow*window);
		void Init(BaseWindow*parent);
		void AppendUI(UINode*node);
		void FixedWindow(int x, int y,int width,int height);
		static void InitWindowClasses();
	};
}