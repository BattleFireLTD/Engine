#pragma once
#include "Platform/Windows/Common/Control/Image/SimpleImage.h"
#include <functional>
namespace Editor{
	class ImageButton :public UINode {
	protected:
		Gdiplus::Point mOriginalPos;
		SimpleImage mImage;
	public:
		ImageButton();
		void SetImage(const char * path);
		void SetRect(int x, int y, int width, int height);
		virtual void Draw(Gdiplus::Graphics&painter);
		virtual void OnTouchBegin(int x, int y, int touch_id = 0);
		virtual void OnTouchEnd(int x, int y, int touch_id = 0);
		virtual void OnTouchMove(int x, int y, int touch_id = 0);
		virtual void OnTouchCanceled(int x, int y, int touch_id = 0);
		virtual void OnClicked(int x, int y, int touch_id = 0);
	};
}