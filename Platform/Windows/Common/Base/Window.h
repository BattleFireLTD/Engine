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
	class WindowHolder;
	class BaseWindow:public EventDispatcher{
	protected:
		HDC mHDC;
		BaseWindow*mParent;
		Gdiplus::Color mBKGColor;
		Gdiplus::Rect mRect, mMinRect, mMaxRect, mPredefinedRect;
		int mTopNCSize, mBottomNCSize, mLeftNCSize, mRightNCSize;
		int mSizingBorderSize;
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
		WindowHolder *mLeftSiblingWindows, *mRightSiblingWindows, *mTopSiblingWindows, *mBottomSiblingWindows;
		std::unordered_set<BaseWindow*> mChildren;
		BaseWindow();
		virtual ~BaseWindow();
		virtual void OnWindowMove(BaseWindow*movedWindow);
		virtual void OnEditControlLoseFocus(BaseWindow*editControl);

		void SetMinRect(int x, int y, int width, int height);
		void SetMaxRect(int x, int y, int width, int height);
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
		int GenerateMinWidth();
		int GenerateMinHeight();
		int GenerateMaxWidth();
		int GenerateMaxHeight();

		void ScheduleUpdate();
		void CancelUpdate();
		void Update();
		virtual void MarkDirty();
		virtual void MoveWindow(int x, int y, int width, int height);
		virtual void OnParentResized(int width, int height) {}
		virtual void ExtentWindowFromLeft(int & deltaX, const Gdiplus::Rect * left_rect = nullptr, const Gdiplus::Rect * container_rect = nullptr);
		virtual void ReduceWindowFromLeft(int & deltaX, const Gdiplus::Rect * left_rect = nullptr, const Gdiplus::Rect * container_rect = nullptr);
		virtual void ExtentWindowFromRight(int & deltaX, const Gdiplus::Rect * right_rect = nullptr);
		virtual void ReduceWindowFromRight(int & deltaX, int shiftX, const Gdiplus::Rect * container_rect = nullptr);
		virtual void ExtentWindowFromTop(int & deltaY, const Gdiplus::Rect * top_rect = nullptr, const Gdiplus::Rect * container_rect = nullptr);
		virtual void ReduceWindowFromTop(int & deltaY, const Gdiplus::Rect * top_rect = nullptr, const Gdiplus::Rect * container_rect = nullptr);
		virtual void ExtentWindowFromBottom(int & deltaY, const Gdiplus::Rect * bottom_rect = nullptr);
		virtual void ReduceWindowFromBottom(int & deltaY, int shiftY, const Gdiplus::Rect * container_rect = nullptr);
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