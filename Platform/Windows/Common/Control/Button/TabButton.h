#pragma once
#include "Platform/Windows/Common/Control/Image/SimpleImage.h"
#include "../Text/StaticText.h"
#include <functional>
namespace Editor{
	class TabButton :public UINode {
	protected:
		Gdiplus::Point mOriginalPos;
		SimpleImage mImage,mIcon;
	public:
		StaticText mTitle;
		TabButton();
		void SetImage(const char * path);
		void SetImageData(StreamImageData*data);
		void SetIconImageData(StreamImageData*data);
		void SetRect(int x, int y, int width, int height);
		void Init(StreamImageData*bkg_image, StreamImageData*icon_image,const char *text);
		virtual void Draw(Gdiplus::Graphics&painter);
		virtual void OnTouchBegin(int x, int y, int touch_id = 0);
		virtual void OnTouchEnd(int x, int y, int touch_id = 0);
		virtual void OnTouchMove(int x, int y, int touch_id = 0);
		virtual void OnTouchCanceled(int x, int y, int touch_id = 0);
		virtual void OnClicked(int x, int y, int touch_id = 0);
	};
}