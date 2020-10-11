#pragma once
#include "Platform/Windows/Common/Control/Image/SimpleImage.h"
#include <functional>
namespace Editor{
	class TwoStateButton :public UINode {
	public:
		enum TwoStateButtonState {
			kTwoStateButtonStateOne,
			kTwoStateButtonStateTwo,
			kTwoStateButtonStateCount
		};
	protected:
		Gdiplus::Point mOriginalPos;
		SimpleImage mImage[2];
		TwoStateButtonState mState;
	public:
		TwoStateButton();
		void SetImage(const char * path1,const char *path2);
		void SetImageData(StreamImageData*data1, StreamImageData*data2);
		void SetRect(int x, int y, int width, int height);
		TwoStateButtonState GetState() const { return mState; }
		virtual void Draw(Gdiplus::Graphics&painter);
		virtual void OnTouchBegin(int x, int y, int touch_id = 0);
		virtual void OnTouchEnd(int x, int y, int touch_id = 0);
		virtual void OnTouchMove(int x, int y, int touch_id = 0);
		virtual void OnTouchCanceled(int x, int y, int touch_id = 0);
		virtual void OnClicked(int x, int y, int touch_id = 0);
		virtual void OnContainerSizeChanged(int width, int height);
	};
}