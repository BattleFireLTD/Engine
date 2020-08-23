#pragma once
#include "Runtime/BattleFirePrefix.h"
#include "Runtime/IO/AliceData.h"
#if ALICE_PLATFORM_WIN
#include <io.h>
#endif
#include <list>
#if ALICE_PLATFORM_UNIX
#define TCHAR char
#define LPCTSTR const char*
#else
#ifndef TCHAR
#define WCHAR TCHAR
#endif
#endif
#if ALICE_PLATFORM_UNIX
typedef int64_t __int64;
#endif

namespace Alice
{
	struct FileItemNode
	{
#if ALICE_PLATFORM_WIN
		TCHAR mName[_MAX_PATH];
#elif ALICE_IPHONE || ALICE_ANDROID
		char mName[256];
#endif
		bool mbIsDir;
		__int64 mLastWriteTime;
		std::list<FileItemNode*> mChildren;
		FileItemNode()
		{
#if ALICE_PLATFORM_WIN
			wmemset(mName, 0, _MAX_PATH);
#elif ALICE_IPHONE
            memset(mName,0,_MAX_PATH);
#endif
			mbIsDir = false;
		}
		~FileItemNode()
		{
			if (mbIsDir)
			{
				for (std::list<FileItemNode*>::iterator iter=mChildren.begin();iter!=mChildren.end();iter++)
				{
					delete *iter;
				}
			}
		}
	};

	class FileSystem
	{
	public:
#if ALICE_PLATFORM_WIN
		static void GetFiles(LPCTSTR path, LPCTSTR filter, FileItemNode&root);
		static void GetFiles(LPCTSTR path, LPCTSTR relativeRootPath, LPCTSTR filter, FileItemNode&root);
		static bool DeleteDir(LPCTSTR path);
#endif
		static bool SaveData(const char*filePath,Data&data);
		static AliceUInt32 FileSizeOf(const char* path);
		static bool CreateDir(std::string path);
		static bool CreateFile(std::string path);
		static bool MoveFile(const char*srcPath, const char*dstPath);
		static bool DeleteDir(std::string path);
		static bool DeleteFileWithPath(std::string path);
		static bool CopyFile(const char*srcPath,const char*dstPath);
		static bool MoveFileToTrash(const char*path);
		static bool isDirectoryExist(const std::string& dirPath);
		static bool Exists(const char*path);
		static bool isAbsolutePath(const std::string& path);
		static char* LoadFile(const char*path);
		static void CopyDir(const char*src,const char* dst);
		static bool LoadDataFromPath(const char*path,Data&data);
	};
}
