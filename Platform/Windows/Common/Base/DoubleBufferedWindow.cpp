#include "DoubleBufferedWindow.h"
#include "Runtime/Debugger/Logger.h"
namespace Editor{
	DoubleBufferedWindow::DoubleBufferedWindow():mBKGDC(nullptr),mBKGBMP(nullptr){
		mBufferWidth = 0;
		mChild = nullptr;
	}
	void DoubleBufferedWindow::OnPaint(const Gdiplus::Rect & rect_need_update){
		if (mBufferWidth != mRect.Width || mBufferHeight != mRect.Height){
			InitDoubleBuffer();
		}
		//Debug("DoubleBufferedWindow::OnPaint %s rect[%d,%d,%d,%d] wrect[%d,%d,%d,%d]", mName, 
		//	rect_need_update.X, rect_need_update.Y, rect_need_update.Width, rect_need_update.Height,
		//	mRect.X, mRect.Y, mRect.Width, mRect.Height);
		Gdiplus::Graphics painter(mBKGDC);
		OnClearBKG(painter);
		DrawContent(painter);
		RenderChildren(painter,rect_need_update);
		OnEndPaint();
	}
	void DoubleBufferedWindow::OnPaintNoUpdateRect() {
	}
	void DoubleBufferedWindow::OnEndPaint(){
		BitBlt(mHDC, 0, 0, mBufferWidth, mBufferHeight, mBKGDC, 0, 0, SRCCOPY);
	}
	LRESULT DoubleBufferedWindow::OnNCPAINT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		return 0;
	}
	void DoubleBufferedWindow::InitDoubleBuffer(){
		if (mBufferWidth == 0) {//first time come here
			mBKGDC = CreateCompatibleDC(mHDC);
		}
		if (mBKGBMP != nullptr){
			DeleteObject(mBKGBMP);
		}
		mBufferWidth = mRect.Width;
		mBufferHeight = mRect.Height;
		mBKGBMP = CreateCompatibleBitmap(mHDC, mBufferWidth, mBufferHeight);
		SelectObject(mBKGDC, mBKGBMP);
	}
}