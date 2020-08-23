#pragma once
#include "Platform/Windows/Common/Control/UINode.h"
#include "Platform/Windows/Common/WinResources.h"
namespace Editor{
	enum ImageRenderMode{
		ImageRenderModeFill,//fill the full rect of the control
		ImageRenderModeAutoAdjust,//auto scale width & height
		ImageRenderModeCenterNoScale//no width & height
	};
	class SimpleImage :public UINode {
	protected:
		ImageRenderMode mRenderMode;
		StreamImageData*mStreamImageData;
		float mIllumination;
	public:
		std::string mImagePath;
		SimpleImage();
		~SimpleImage();
		void SetRenderMode(ImageRenderMode mode);
		Gdiplus::Image* SetImageData(StreamImageData*data);
		Gdiplus::Image* SetImagePath(const char*imagePath);
		Gdiplus::Image* GetImageData();
	public:
		virtual void Draw(Gdiplus::Graphics&painter);
		virtual void SetRect(int x,int y,int width,int height);
	protected:
		void DrawRenderModeAutoAdjust(Gdiplus::Graphics&painter);
		void DrawRenderModeCenterNoScale(Gdiplus::Graphics&painter);
		void InitIllumination(Gdiplus::ImageAttributes&attributes);
		void DrawImage(Gdiplus::Graphics&painter, int dst_x, int dst_y, int dst_width, int dst_height, int src_x, int src_y, int src_width, int src_height);
	};
}