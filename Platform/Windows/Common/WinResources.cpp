#include "WinResources.h"
#include "Runtime/Allocator/AliceMemory.h"
namespace Editor {
	WinResources*WinResources::mSelf = nullptr;
	WinResources*WinResources::Singleton() {
		if (mSelf != nullptr) {
			mSelf = new WinResources;
		}
		return mSelf;
	}
	void WinResources::Init(const char * path) {

	}
	StreamImageData*WinResources::GetImageData(const char * path) {
		if (mCachedImages.find(path)!=mCachedImages.end()){
			return mCachedImages[path];
		}
		if (mWinRes.res().contains(path)) {
			const std::string & original_binary_data = mWinRes.res().at(path);
			StreamImageData *sid = InitStreamImageDataFromRawBuffer((unsigned char*)original_binary_data.c_str(), original_binary_data.length());
			sid->mbShared = true;
			mCachedImages.insert(std::pair<std::string,StreamImageData*>(path,sid));
			return  sid;
		}
		return nullptr;
	}
	StreamImageData*WinResources::InitStreamImageDataFromRawBuffer(const unsigned char * data, int len) {
		HGLOBAL hMem = GlobalAlloc(GMEM_FIXED, len);
		BYTE* pMem = (BYTE*)GlobalLock(hMem);
		memcpy(pMem, data, len);
		IStream *stream = nullptr;
		HRESULT hr = CreateStreamOnHGlobal(pMem, TRUE, &stream);
		Gdiplus::Image*image = Gdiplus::Image::FromStream(stream);
		GlobalUnlock(hMem);
		return new(kMemEditorId)StreamImageData(stream, image);
	}
}