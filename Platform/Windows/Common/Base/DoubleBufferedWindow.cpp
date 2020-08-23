#include "DoubleBufferedWindow.h"

namespace Editor
{
	DoubleBufferedWindow::DoubleBufferedWindow():mBKGDC(nullptr),mBKGBMP(nullptr)
	{
		mBufferWidth = 0;
		mChild = nullptr;
	}

	void DoubleBufferedWindow::OnPaint()
	{
		if (mBufferWidth != mRect.Width || mBufferHeight != mRect.Height){
			InitDoubleBuffer();
		}
		Gdiplus::Graphics painter(mBKGDC);
		painter.Clear(mBKGColor);
		DrawContent(painter);
		OnEndPaint();
		if (!mChildren.empty()){
			for (auto iter=mChildren.begin();iter!=mChildren.end();++iter)
			{
				(*iter)->PaintWindow();
			}
		}
	}

	void DoubleBufferedWindow::OnEndPaint(){
		BitBlt(mHDC, 0, 0, mBufferWidth, mBufferHeight, mBKGDC, 0, 0, SRCCOPY);
	}

	void DoubleBufferedWindow::InitDoubleBuffer()
	{
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