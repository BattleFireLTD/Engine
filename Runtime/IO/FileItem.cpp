#include "FileItem.h"
#include "Runtime/Allocator/AliceMemory.h"
#include "Runtime/String/StringUtils.h"

namespace Alice{
	FileItem::FileItem() {
	}
	FileItem*FileItem::Get(const char*path){
		FileItem*item = nullptr;
#if ALICE_PLATFORM_WIN
		_finddata_t info;
		long hFile = _findfirst(path, &info);
		if (hFile){
			item = new (kMemIOId)FileItem;
			memset(item->mRelativePath, 0, _MAX_PATH);
			strcpy(item->mRelativePath, path);
			item->mFileSize = info.size;
			item->mLastAccessTime = info.time_access;
			item->mLastWriteTime = info.time_write;
			item->mCreateTime = info.time_create;
			_findclose(hFile);
		}
#else
		struct stat stbuf;
		int fd;
		fd = open(path, O_RDONLY);
		if (fd != -1) {
			if ((fstat(fd, &stbuf) != 0) || (!S_ISREG(stbuf.st_mode))) {
				close(fd);
				return nullptr;
			}
			item = new (kMemIOId)FileItem;
			memset(item->mRelativePath, 0, _MAX_PATH);
			strcpy(item->mRelativePath, path);
			item->mFileSize = stbuf.st_size;
			item->mLastAccessTime = stbuf.st_atime;
			item->mLastWriteTime = stbuf.st_mtime;
			item->mCreateTime = stbuf.st_ctime;
			close(fd);
		}
#endif
		return item;
	}
}
