#pragma once
#include "Base/DoubleBufferedWindow.h"
#include "Control/UINode.h"
namespace Editor {
	class MainWindow :public DoubleBufferedWindow {
	protected:
		Gdiplus::Point mDeltaWhenDrag;
		int mMarginLeft, mMarginRight, mMarginTop, mMarginBottom, mFrameEdgeWidth;
		bool mbDraging;
		UINode * mUIRoot,*mLastTouchObject;
	protected:
		virtual void OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnSize(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		LRESULT OnNCACTIVATE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		LRESULT OnNCCALCSIZE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		LRESULT OnNCPAINT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		LRESULT OnNCHITTEST(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	public:
		MainWindow();
		void Init();
		static void InitWindowClasses();
	};
}