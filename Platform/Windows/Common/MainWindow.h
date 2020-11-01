#pragma once
#include "Base/DoubleBufferedWindow.h"
#include "Control/UINode.h"
#include "Base/WindowContainer.h"
namespace Editor {
	class MainWindow :public DoubleBufferedWindow {
	protected:
		Gdiplus::Point mDeltaWhenDrag;
		Gdiplus::Color mTitleBKGColor;
		bool mbDraging;
		UINode * mUIRoot, *mLastTouchObject;
		WindowContainer *mContainer;
	protected:
		virtual void OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnSize(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnSizing(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void DrawContent(Gdiplus::Graphics&painter);
		virtual void RenderChildren(Gdiplus::Graphics&painter, const Gdiplus::Rect & rect_need_update);
		virtual void OnClearBKG(Gdiplus::Graphics&painter);
		void ExtentWindowFromLeft(int &deltaX);
		void ReduceWindowFromLeft(int &deltaX);
		void ExtentWindowFromRight(int &deltaX);
		void ReduceWindowFromRight(int &deltaX);
		void ExtentWindowFromTop(int &deltaY);
		void ReduceWindowFromTop(int &deltaY);
		void ExtentWindowFromBottom(int &deltaY);
		void ReduceWindowFromBottom(int &deltaY);
	public:
		MainWindow();
		void SetContainer(WindowContainer *container);
		void RemoveChildWindowAt(ChildWindowLocation location, BaseWindow*window);
		void Init();
		static void InitWindowClasses();
	};
}