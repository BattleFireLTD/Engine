#pragma once
#include "Runtime/BattleFirePrefix.h"
#include "Runtime/Allocator/AliceMemory.h"
namespace Alice{
	class Data{
	public:
		Data(int buffer_size=0);
		virtual ~Data();
		void Reset();
		void SetBufferSize(int bufferSize);
		void MakeZero();
		AliceUInt8 *mData;
		AliceSInt32 mDataLen;
		AliceSInt32 mBufferSize;
	};
}