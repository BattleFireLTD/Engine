#pragma once
#include "AliceData.h"
#include "Runtime/String/FixedString.h"

namespace Alice{
	class ResourceManager{
	public:
		static bool Exist(const char*path);
		static bool DeleteFileWithPath(const char*path);
		static bool CreateDir(const char*path);
		static bool DeleteDir(const char*path);
		static void CopyInternalFileOut(const char*internalPath);
	public:
		static bool ExistInternal(const char*path);
		static bool LoadInternalData(const char*path,Data&data);
		static bool RenameInternalFileName(const char*srcFile,const char*dstFile);
		static bool DeleteInternalFile(const char*targetFile);
		static bool CopyFileToInternal(const char*targetFile, const char*dstFile);
		static bool MoveInternalFile(const char*srcFile, const char*dstFile);
		static bool SaveInternalData(const char*targetPath, Data&data);
		static bool CreateInternalFile(const char*targetPath);

		static bool LoadOuterData(const char*path,Data&data);
		static bool LoadBuiltinData(const char*path,Data&data);
		//the caller is response to invoke the delete method to destroy the resource
		static bool LoadData(const char*path,Data&data,FixedString *realPath=nullptr);
	private:
		static bool LoadDataFromRuntimeDataPath(const char*path, Data&data, FixedString *realPath = nullptr);
		static bool LoadDataFromBuiltinDataPath(const char*path, Data&data, FixedString *realPath = nullptr);
	public:
		static void Init();
		static FixedString*mAssetsDir;
		static FixedString*mDataDir;
        static FixedString*mDocumentDir;
        static FixedString*mDesktopDir;
	};
}
