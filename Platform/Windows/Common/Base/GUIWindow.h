#pragma once
#include "DoubleBufferedWindow.h"
namespace Editor {
	class GUIWindow :public DoubleBufferedWindow {
	protected:
		Gdiplus::Point mDeltaWhenDrag;
		bool mIsChild;
	protected:
		void OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void DrawContent(Gdiplus::Graphics&painter);
		void OnSize(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void OnKeyDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void OnCommand(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		LRESULT OnNCACTIVATE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		LRESULT OnNCCALCSIZE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		LRESULT OnNCPAINT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		LRESULT OnNCHITTEST(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	public:
		void Init(HWND parent=nullptr);
		void ToChildWindow();
		void ToMainWindow();
		void UpdateTitle();
		static void InitWindowClasses();
	};
}