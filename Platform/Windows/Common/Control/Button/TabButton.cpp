#include "TabButton.h"
#include "Runtime/Debugger/Logger.h"
namespace Editor{
	TabButton::TabButton(){
		mOriginalPos.X = 0;
		mOriginalPos.Y = 0;
	}
	void TabButton::SetImage(const char * path) {
		mImage.SetImagePath(path);
	}
	void TabButton::SetImageData(StreamImageData*data) {
		mImage.SetImageData(data);
	}
	void TabButton::SetIconImageData(StreamImageData*data) {
		mIcon.SetImageData(data);
	}
	void TabButton::SetRect(int x, int y, int width, int height) {
		mImage.SetRect(x, y, width, height);
		mIcon.SetRect(x + 2, y + 2, 16, 16);
		mTitle.SetRect(x + 20, y+2, width - 20, height);
		UINode::SetRect(x, y, width, height);
	}
	void TabButton::Init(StreamImageData*bkg_image, StreamImageData*icon_image, const char *text) {
		SetImageData(bkg_image);
		SetIconImageData(icon_image);
		mTitle.SetAligning(Editor::AligningModeLeft);
		mTitle.SetText(text);
	}
	void TabButton::Draw(Gdiplus::Graphics&painter) {
		mImage.Draw(painter);
		mIcon.Draw(painter);
		mTitle.Draw(painter);
	}
	void TabButton::OnTouchBegin(int x, int y, int touch_id/* =0 */) {
		Gdiplus::Rect original_rect = mImage.GetRect();
		mOriginalPos.X = original_rect.X;
		mOriginalPos.Y = original_rect.Y;
		mImage.SetRect(mOriginalPos.X, mOriginalPos.Y + 2, original_rect.Width, original_rect.Height);
	}
	void TabButton::OnTouchEnd(int x, int y, int touch_id /* = 0 */) {
		Gdiplus::Rect original_rect = mImage.GetRect();
		mImage.SetRect(mOriginalPos.X, mOriginalPos.Y, original_rect.Width, original_rect.Height);
		if (mRect.Contains(x, y)) {
			OnClicked(x, y);
		}
	}
	void TabButton::OnTouchCanceled(int x, int y, int touch_id /* = 0 */) {
		Gdiplus::Rect original_rect = mImage.GetRect();
		mImage.SetRect(mOriginalPos.X, mOriginalPos.Y, original_rect.Width, original_rect.Height);
	}
	void TabButton::OnTouchMove(int x, int y, int touch_id /* = 0 */) {

	}
	void TabButton::OnClicked(int x, int y, int touch_id /* = 0 */) {
		if (mOnClicked != nullptr) {
			mOnClicked(this);
		}
	}
}