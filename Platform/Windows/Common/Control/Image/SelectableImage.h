#pragma once
#include "SimpleImage.h"
#include <functional>
namespace Editor{
	enum SelectedMaskType{
		kSelectedMaskTypeChangeImageIllumination,
		kSelectedMaskTypeChangeBackgroundColor
	};
	class SelectableImage :public SimpleImage{
	protected:
		Gdiplus::Color mSelectedBKGColor;
		float mIlluminationSelected, mIlluminationNormal;
		SelectedMaskType mSelectedMaskType;
		bool mbSelected;
	public:
		SelectableImage(SelectedMaskType t= kSelectedMaskTypeChangeImageIllumination);
		bool IsSelected();
		void SetSelectedMaskType(SelectedMaskType t);
		void SetSelectedBKGColor(Gdiplus::Color&color);
	public:
		virtual void Draw(Gdiplus::Graphics&painter);
		virtual void OnTouchBegin(int x, int y, int touch_id = 0);
		virtual void OnTouchEnd(int x, int y, int touch_id = 0);
		virtual void OnTouchMove(int x, int y, int touch_id = 0);
		virtual void OnTouchCanceled(int x, int y, int touch_id = 0);
		virtual void OnClicked(int x, int y, int touch_id = 0);
	};
}