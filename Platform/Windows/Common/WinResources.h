#pragma once
#include "Runtime/BattleFirePrefix.h"
#include "PlatformWindowsSeriallizer.pb.h"
#include "google/protobuf/text_format.h"
namespace Editor{
	struct StreamImageData{
		StreamImageData(IStream*stream, Gdiplus::Image*image) :mStream(stream), mImage(image),mbShared(false){
		}
		~StreamImageData(){
			if (mbShared) {
				if (mImage != nullptr) {
					delete mImage;
				}
				if (mStream != nullptr) {
					mStream->Release();
				}
			}
		}
		IStream *mStream;
		Gdiplus::Image*mImage;
		bool mbShared;
	};
	class WinResources {
	protected:
		static WinResources*mSelf;
		PlatformWindowsSeriallizer::WinResources mWinRes;
		std::unordered_map<std::string, StreamImageData*> mCachedImages;
	public:
		static WinResources*Singleton();
		void Init(const char * path);
		StreamImageData*GetImageData(const char * path);
		StreamImageData*InitStreamImageDataFromRawBuffer(const unsigned char * data, int len);
	};
}