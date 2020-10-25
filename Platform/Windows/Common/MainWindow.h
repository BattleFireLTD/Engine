#pragma once
#include "Base/DoubleBufferedWindow.h"
#include "Control/UINode.h"
namespace Editor {
	class MainWindow :public DoubleBufferedWindow {
	protected:
		Gdiplus::Point mDeltaWhenDrag;
		Gdiplus::Color mTitleBKGColor;
		bool mbDraging;
		UINode * mUIRoot, *mLastTouchObject;
		WindowHolder * mLeftMostChildren, *mRightMostChildren, *mTopMostChildren, *mBottomMostChildren;
	protected:
		virtual void OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnSize(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnSizing(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		BaseWindow::WindowResizeDirection OnPostSizeChanged(int &deltaX,int &deltaY);
		virtual void DrawContent(Gdiplus::Graphics&painter);
		virtual void RenderChildren(Gdiplus::Graphics&painter, const Gdiplus::Rect & rect_need_update);
		virtual void OnClearBKG(Gdiplus::Graphics&painter);
	public:
		MainWindow();
		void AddChildWindowAt(ChildWindowLocation location, BaseWindow*window);
		void RemoveChildWindowAt(ChildWindowLocation location, BaseWindow*window);
		void Init();
		static void InitWindowClasses();
	};
}