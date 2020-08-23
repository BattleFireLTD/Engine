#pragma once
#include "Runtime/BattleFirePrefix.h"
namespace Alice{
	class FileItem{
	public:
		char mRelativePath[_MAX_PATH];
		AliceSInt32 mFileSize;
		AliceSInt64 mLastWriteTime, mCreateTime, mLastAccessTime;
		FileItem();
		static FileItem* Get(const char*path);
	};
}