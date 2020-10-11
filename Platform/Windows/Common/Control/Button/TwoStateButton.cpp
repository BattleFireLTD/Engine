#include "TwoStateButton.h"
#include "Runtime/Debugger/Logger.h"
namespace Editor{
	TwoStateButton::TwoStateButton(){
		mOriginalPos.X = 0;
		mOriginalPos.Y = 0;
		mState = kTwoStateButtonStateOne;
	}
	void TwoStateButton::SetImage(const char * path1, const char * path2) {
		mImage[0].SetImagePath(path1);
		mImage[1].SetImagePath(path2);
	}
	void TwoStateButton::SetImageData(StreamImageData*data1, StreamImageData*data2) {
		mImage[0].SetImageData(data1);
		mImage[1].SetImageData(data2);
	}
	void TwoStateButton::SetRect(int x, int y, int width, int height) {
		mImage[0].SetRect(x, y, width, height);
		mImage[1].SetRect(x, y, width, height);
		UINode::SetRect(x, y, mImage[0].GetRect().Width, mImage[0].GetRect().Height);
	}
	void TwoStateButton::Draw(Gdiplus::Graphics&painter) {
		mImage[mState].Draw(painter);
	}
	void TwoStateButton::OnTouchBegin(int x, int y, int touch_id/* =0 */) {
		Gdiplus::Rect original_rect=mImage[mState].GetRect();
		mOriginalPos.X = original_rect.X;
		mOriginalPos.Y = original_rect.Y;
		mImage[mState].SetRect(mOriginalPos.X, mOriginalPos.Y + 2, original_rect.Width, original_rect.Height);
	}
	void TwoStateButton::OnTouchEnd(int x, int y, int touch_id /* = 0 */) {
		Gdiplus::Rect original_rect = mImage[mState].GetRect();
		mImage[mState].SetRect(mOriginalPos.X, mOriginalPos.Y, original_rect.Width, original_rect.Height);
		if (mRect.Contains(x, y)) {
			mState = (TwoStateButtonState)((mState + 1) % 2);
			OnClicked(x, y);
		}
	}
	void TwoStateButton::OnTouchCanceled(int x, int y, int touch_id /* = 0 */) {
		Gdiplus::Rect original_rect = mImage[mState].GetRect();
		mImage[mState].SetRect(mOriginalPos.X, mOriginalPos.Y, original_rect.Width, original_rect.Height);
	}
	void TwoStateButton::OnTouchMove(int x, int y, int touch_id /* = 0 */) {

	}
	void TwoStateButton::OnClicked(int x, int y, int touch_id /* = 0 */) {
		if (mOnClicked != nullptr) {
			mOnClicked(this);
		}
	}
	void TwoStateButton::OnContainerSizeChanged(int width, int height) {
		UINode::OnContainerSizeChanged(width, height);
		mImage[0].SetRect(mRect.X, mRect.Y, mRect.Width, mRect.Height);
		mImage[1].SetRect(mRect.X, mRect.Y, mRect.Width, mRect.Height);
	}
}