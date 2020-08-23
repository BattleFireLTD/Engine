#include "SelectableImage.h"
namespace Editor{
	SelectableImage::SelectableImage(SelectedMaskType t) :mbSelected(false), mSelectedMaskType(t){
		mSelectedBKGColor = Gdiplus::Color(0, 100, 150);
		mIlluminationNormal = 1.0f;
		mIlluminationSelected = 1.2f;
	}
	bool SelectableImage::IsSelected(){
		return mbSelected;
	}
	void SelectableImage::SetSelectedMaskType(SelectedMaskType t){
		mSelectedMaskType = t;
	}
	void SelectableImage::SetSelectedBKGColor(Gdiplus::Color&color){
		mSelectedBKGColor = color;
	}
	void SelectableImage::Draw(Gdiplus::Graphics&painter){
		if (mbSelected){
			if (mSelectedMaskType == kSelectedMaskTypeChangeBackgroundColor) {
				Gdiplus::SolidBrush brush(mSelectedBKGColor);
				painter.FillRectangle(&brush, mRect);
			}else if (mSelectedMaskType==kSelectedMaskTypeChangeImageIllumination){
				mIllumination = mIlluminationSelected;
			}
			SimpleImage::Draw(painter);
		}
		else {
			if (mSelectedMaskType == kSelectedMaskTypeChangeImageIllumination) {
				mIllumination = mIlluminationNormal;
				SimpleImage::Draw(painter);
			}
		}
	}
	void SelectableImage::OnTouchBegin(int x, int y, int touch_id/* =0 */) {

	}
	void SelectableImage::OnTouchEnd(int x, int y, int touch_id /* = 0 */) {

	}
	void SelectableImage::OnTouchCanceled(int x, int y, int touch_id /* = 0 */) {

	}
	void SelectableImage::OnTouchMove(int x, int y, int touch_id /* = 0 */) {

	}
	void SelectableImage::OnClicked(int x, int y, int touch_id /* = 0 */) {

	}
}