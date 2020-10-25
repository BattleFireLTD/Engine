#pragma once
#include "EventDispatcher.h"
namespace Editor {
	enum ChildWindowLocation {
		kChildWindowLocationUnkown,
		kChildWindowLocationLeftEdge,
		kChildWindowLocationRightEdge,
		kChildWindowLocationTopEdge,
		kChildWindowLocationBottomEdge
	};
	enum SiblingWindowLocation {
		kSiblingWindowLocationAtUnkown,
		kSiblingWindowLocationAtLeft,
		kSiblingWindowLocationAtRight,
		kSiblingWindowLocationAtTop,
		kSiblingWindowLocationAtBottom
	};
	class BaseWindow:public EventDispatcher{
	public:
		enum WindowResizeDirection {
			kWindowResizeDirectionNone,
			kWindowResizeDirectionFromLeft=1,
			kWindowResizeDirectionFromTop=2,
			kWindowResizeDirectionFromRight=4,
			kWindowResizeDirectionFromBottom=8,
			kWindowResizeDirectionFromLeftTop=16,
			kWindowResizeDirectionFromRightTop=32,
			kWindowResizeDirectionFromRightBottom=64,
			kWindowResizeDirectionFromLeftBottom=128
		};
	protected:
		HDC mHDC;
		BaseWindow*mParent;
		Gdiplus::Color mBKGColor;
		Gdiplus::Rect mRect, mMinRect, mMaxRect;
		int mTopNCSize, mBottomNCSize, mLeftNCSize, mRightNCSize;
		int mSizingBorderSize;
		SiblingWindowLocation mLocation;
		char mName[64];
	protected:
		virtual void DrawContent(Gdiplus::Graphics&painter);
		virtual void OnEraseBKG();
		virtual void OnPaint(const Gdiplus::Rect & rect_need_update);
		virtual void OnPostPaint();
		virtual void RenderChildren(Gdiplus::Graphics&painter,const Gdiplus::Rect & rect_need_update){}
		virtual void OnSize(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMoving(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnNCACTIVATE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnNCHITTEST(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnNCCALCSIZE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnNCPAINT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	public:
		std::unordered_set<BaseWindow*> mChildren;
		BaseWindow();
		virtual ~BaseWindow();
		virtual void OnWindowMove(BaseWindow*movedWindow);
		virtual void OnEditControlLoseFocus(BaseWindow*editControl);

		void SetMinRect(int x, int y, int width, int height);
		void SetBkgColor(Gdiplus::Color &color);
		void SetParent(BaseWindow*parent);
		void SetRect(int x, int y, int width, int height);
		void SetRect(Gdiplus::Rect &rect);
		void SetSize(int width, int height, HWND param);
		void SetNCSize(int left, int right, int bottom, int top);
		void SetSizingBorderSize(int size);
		void SetWindowName(const char*name);
		void GetRelativeWindowRect(Gdiplus::Rect &rect);
		int GetUILocationL(int size, int distance);
		int GetUILocationR(int size, int distance);
		int GetUILocationT(int size, int distance);
		int GetUILocationB(int size, int distance);
	public://rect
		Gdiplus::Rect GetRect();
		int GetWidth();
		int GetHeight();
		Gdiplus::Rect &GetMinRect();
		int GetMinWidth();
		int GetMinHeight();

		void ScheduleUpdate();
		void CancelUpdate();
		void Update();
		virtual void MarkDirty();
		virtual void MoveWindow(int x, int y, int width, int height);
		virtual void OnParentResized(int width, int height, int deltaX = 0, int deltaY = 0, WindowResizeDirection d = kWindowResizeDirectionNone) {}
		virtual void TryToExtentWindowHorizontallyFromLeft(int & deltaX) { }
		virtual void TryToReduceWindowHorizontallyFromLeft(int & deltaX) { }
		virtual void TryToExtentWindowHorizontallyFromRight(int & deltaX) { }
		virtual void TryToReduceWindowHorizontallyFromRight(int & deltaX) { }
		virtual void TryToExtentWindowVerticallyFromTop(int & deltaY) { }
		virtual void TryToReduceWindowVerticallyFromTop(int & deltaY) { }
		virtual void TryToExtentWindowVerticallyFromBottom(int & deltaY) { }
		virtual void TryToReduceWindowVerticallyFromBottom(int & deltaY) { }
		virtual void ExtentWindowHorizontallyFromLeft(int deltaX) { }
		virtual void ReduceWindowHorizontallyFromLeft(int deltaX) { }
		virtual void ExtentWindowHorizontallyFromRight(int deltaX) { }
		virtual void ReduceWindowHorizontallyFromRight(int deltaX) { }
		virtual void ExtentWindowVerticallyFromTop(int deltaY) { }
		virtual void ReduceWindowVerticallyFromTop(int deltaY) { }
		virtual void ExtentWindowVerticallyFromBottom(int deltaY) { }
		virtual void ReduceWindowVerticallyFromBottom(int deltaY) { }
		virtual bool ConsumeDeltaX(int & deltaX) { return true; }
		virtual bool ConsumeDeltaY(int & deltaY) { return true; }
		virtual void OnParentPaint(Gdiplus::Graphics&painter) {}
		int GetX();
		int GetY();
		BaseWindow*GetParent();
		HWND GetHwnd();
		const char*GetWindowName();
		HDC GetDC();
		bool IsVisiable();
		void Show();
		virtual void OnShow();
		void Hide();
		virtual void OnHide();
		template<typename T>
		static T* WindowInstance(HWND hWnd) {
			return (T*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		}
	public:
		static ATOM RegisterWindowClass(UINT style, LPCTSTR pWndClassName, WNDPROC wndProc);
		static std::unordered_set<BaseWindow*> mScheduledWindows;
		static void SetRootWindow(BaseWindow*window);
	};
	class WindowHolder : public Alice::LinkedList {
	public:
		BaseWindow*mWindow;
		WindowHolder(BaseWindow*window = nullptr) {
			mWindow = window;
		}
	};
}