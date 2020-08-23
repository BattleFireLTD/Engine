#include "SimpleImage.h"
#include "Runtime/IO/AliceData.h"
#include "Runtime/IO/FileSystem.h"
namespace Editor{
	SimpleImage::SimpleImage():mRenderMode(ImageRenderModeFill),mStreamImageData(nullptr){
		mIllumination = 1.0f;
	}
	SimpleImage::~SimpleImage(){
		if (mStreamImageData !=nullptr){
			delete mStreamImageData;
		}
	}
	Gdiplus::Image* SimpleImage::SetImageData(StreamImageData*data){
		mStreamImageData = data;
		return mStreamImageData->mImage;
	}
	Gdiplus::Image*SimpleImage::GetImageData(){
		return mStreamImageData->mImage;
	}
	Gdiplus::Image* SimpleImage::SetImagePath(const char*imagePath) {
		if (mStreamImageData != nullptr) {
			delete mStreamImageData;
			mStreamImageData = nullptr;
		}
		Alice::Data data;
		if (Alice::FileSystem::LoadDataFromPath(imagePath, data)) {
			mStreamImageData = WinResources::Singleton()->InitStreamImageDataFromRawBuffer(data.mData, data.mDataLen);
		}
		return mStreamImageData==nullptr?nullptr:mStreamImageData->mImage;
	}
	void SimpleImage::SetRenderMode(ImageRenderMode mode){
		mRenderMode = mode;
	}
	void SimpleImage::SetRect(int x, int y, int width, int height){
		if (width == 0||height==0){
			if (mStreamImageData!=nullptr){
				width = mStreamImageData->mImage->GetWidth();
				height = mStreamImageData->mImage->GetHeight();
			}
		}
		UINode::SetRect(x, y, width, height);
	}
	void SimpleImage::Draw(Gdiplus::Graphics&painter){
		if (mStreamImageData != nullptr){
			switch (mRenderMode){
			case Editor::ImageRenderModeFill:
				DrawImage(painter, mRect.X, mRect.Y, mRect.Width, mRect.Height, 0, 0, mStreamImageData->mImage->GetWidth(), mStreamImageData->mImage->GetHeight());
				break;
			case Editor::ImageRenderModeAutoAdjust:
				DrawRenderModeAutoAdjust(painter);
				break;
			case Editor::ImageRenderModeCenterNoScale:
				DrawRenderModeCenterNoScale(painter);
				break;
			default:
				break;
			}
		}
		UINode::Draw(painter);
	}
	void SimpleImage::DrawRenderModeAutoAdjust(Gdiplus::Graphics&painter) {
		Gdiplus::ImageAttributes current_image_attributes;
		InitIllumination(current_image_attributes);
		UINT width = mStreamImageData->mImage->GetWidth();
		UINT height = mStreamImageData->mImage->GetHeight();
		if (width != height){
			if (width > height){
				INT rh = (INT)(mRect.Height * height / width);
				DrawImage(painter, mRect.X, mRect.Y + (mRect.Height - rh) / 2, mRect.Width, rh, 0, 0, width, height);
			}else {
				INT rw = (INT)(mRect.Width * width / height);
				DrawImage(painter, mRect.X + (mRect.Width - rw) / 2, mRect.Y, rw, mRect.Height, 0, 0, width, height);
			}
		}
		else {
			DrawImage(painter, mRect.X, mRect.Y, mRect.Width, mRect.Height, 0, 0, width, height);
		}
	}
	void SimpleImage::DrawRenderModeCenterNoScale(Gdiplus::Graphics&painter) {
		Gdiplus::ImageAttributes current_image_attributes;
		InitIllumination(current_image_attributes);
		int width = mStreamImageData->mImage->GetWidth();
		int height = mStreamImageData->mImage->GetHeight();
		int xPos = mRect.X + mRect.Width / 2 - width / 2;
		int yPos = mRect.Y + mRect.Height / 2 - height / 2;
		DrawImage(painter, xPos, yPos, width, height, 0, 0, width, height);
	}
	void SimpleImage::InitIllumination(Gdiplus::ImageAttributes&attributes) {
		Gdiplus::ColorMatrix colorMatrix = {
			mIllumination,0.0f,0.0f,0.0f,0.0f,
			0.0f,mIllumination,0.0f,0.0f,0.0f,
			0.0f,0.0f,mIllumination,0.0f,0.0f,
			0.0f,0.0f,0.0f,1.0f,0.0f,
			0.0f,0.0f,0.0f,0.0f,1.0f
		};
		attributes.SetColorMatrix(&colorMatrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);
	}
	void SimpleImage::DrawImage(Gdiplus::Graphics&painter, int dst_x, int dst_y, int dst_width, int dst_height, int src_x, int src_y, int src_width, int src_height) {
		Gdiplus::ImageAttributes current_image_attributes;
		InitIllumination(current_image_attributes);
		Gdiplus::Rect target_rect(dst_x, dst_y, dst_width, dst_height);
		painter.DrawImage(mStreamImageData->mImage, target_rect, src_x, src_y, src_width, src_height, Gdiplus::UnitPixel, &current_image_attributes);
	}
}