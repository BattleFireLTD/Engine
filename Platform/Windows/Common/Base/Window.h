#pragma once
#include "EventDispatcher.h"
namespace Editor{
	class BaseWindow:public EventDispatcher{
	protected:
		HDC mHDC;
		BaseWindow*mParent;
		Gdiplus::Color mBKGColor;
		Gdiplus::Rect mRect,mMinRect,mMaxRect;
		char mName[64];
	protected:
		virtual void DrawContent(Gdiplus::Graphics&painter);
		virtual void OnPaint();
		virtual void OnEndPaint();
		virtual void OnSize(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMoving(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual LRESULT OnNCACTIVATE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
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
		void SetWindowName(const char*name);
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
		virtual void PaintWindow();
		virtual void MoveWindow(int x, int y, int width, int height);
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
	public:
		static ATOM RegisterWindowClass(UINT style, LPCTSTR pWndClassName, WNDPROC wndProc);
		static std::unordered_set<BaseWindow*> mScheduledWindows;
		static void SetRootWindow(BaseWindow*window);
		static void FixedUpdate();
	};
}