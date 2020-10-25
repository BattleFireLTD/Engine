#pragma once
#include "Window.h"
namespace Editor{
	class DoubleBufferedWindow :public BaseWindow{
	protected:
		HDC mBKGDC;
		HBITMAP mBKGBMP;
		int mBufferWidth, mBufferHeight;
	protected:
		virtual void OnPaint(const Gdiplus::Rect & rect_need_update);
		void OnPaintNoUpdateRect();
		virtual void OnEndPaint();
		virtual LRESULT OnNCPAINT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	public:
		BaseWindow * mChild;
		DoubleBufferedWindow();
		void InitDoubleBuffer();
	};
}