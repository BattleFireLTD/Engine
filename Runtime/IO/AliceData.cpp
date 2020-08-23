#include "AliceData.h"
namespace Alice{
	Data::Data(int buffer_size) :mData(nullptr), mDataLen(0),mBufferSize(buffer_size) {
		if (buffer_size>0){
			SetBufferSize(buffer_size);
		}
	}
	Data::~Data() {
		if (mBufferSize != 0) {
			delete[] mData;
			mDataLen = 0;
			mBufferSize = 0;
			mData = nullptr;
		}
	}
	void Data::Reset() {
		if (mData != nullptr) {
			delete[] mData;
			mDataLen = 0;
			mBufferSize = 0;
			mData = nullptr;
		}
	}
	void Data::SetBufferSize(int bufferSize) {
		if (mData!=nullptr){
			Reset();
		}
		mBufferSize = bufferSize;
		mData = new(kMemDefaultId)AliceUInt8[bufferSize];
		mDataLen = 0;
	}
	void Data::MakeZero() {
		memset(mData, 0, mBufferSize);
		mDataLen = 0;
	}
}