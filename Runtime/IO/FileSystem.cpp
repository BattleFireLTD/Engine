#include "Runtime/IO/FileSystem.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/IO/ResourceManager.h"
#include "Runtime/Debugger/Logger.h"
#if ALICE_PLATFORM_WIN
#elif ALICE_PLATFORM_UNIX
#include<unistd.h>
#include <sys/stat.h>
#include<sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include "IOSBundleManager.h"
#endif

namespace Alice{
	bool FileSystem::SaveData(const char*filePath, Data&data){
		//create dir if needed
		//FileSystem::CreateFile(filePath);
		FILE *pFile = NULL;
		pFile = fopen(filePath, "wb");
		if (pFile != NULL)
		{
			if (data.mDataLen > 0){
				fwrite(data.mData, sizeof(char), data.mDataLen, pFile);
			}
			fclose(pFile);
			return true;
		}
		else
		{
			Error("fopen error %d: FileSystem::SaveData cannot open file %s",errno, filePath);
			return false;
		}
		return false;
	}

	bool FileSystem::LoadDataFromPath(const char*path, Data&data){
		if (Exists(path)){
			FILE *pFile = fopen(path, "rb");
			if (pFile){
				fseek(pFile, 0, SEEK_END);
				int file_size = ftell(pFile);
				if (file_size > 0){
					rewind(pFile);
					data.SetBufferSize(file_size + 1);
					data.mDataLen = fread(data.mData, 1, file_size, pFile);
					data.mData[file_size] = '\0';
					fclose(pFile);
					return true;
				}else{
					Error("fread error : FileSystem::LoadDataFromPath  file %s : data size %ld",path, file_size);
					data.mDataLen = 0;
					fclose(pFile);
					return false;
				}
				return true;
			}else{
				return false;
			}
		}
		return false;
	}

	char*	FileSystem::LoadFile(const char *filePath)
	{
		FILE *pFile = NULL;
		char *pData = NULL;
		AliceSInt32 count = 0;
		if (filePath != NULL)
		{
			pFile = fopen(filePath, "rb");
			if (pFile != NULL)
			{
				fseek(pFile, 0, SEEK_END);
				count = ftell(pFile);
				rewind(pFile);
				if (count > 0)
				{
					pData = (char *)malloc(sizeof(char) * (count + 1));
					count = fread(pData, sizeof(char), count, pFile);
					pData[count] = '\0';
				}
				fclose(pFile);
			}
		}
		return pData;
	}

	AliceUInt32 FileSystem::FileSizeOf(const char* path){
		AliceUInt32 fileLen = 0;
		FILE *pFile = NULL;
		if (path != NULL){
			pFile = fopen(path, "rb");
			if (pFile != NULL){
				fseek(pFile, 0, SEEK_END);
				fileLen = ftell(pFile);
				fclose(pFile);
			}
		}
		return fileLen;
	}
#if ALICE_PLATFORM_WIN
	void FileSystem::GetFiles(LPCTSTR path, LPCTSTR relative, LPCTSTR filter, FileItemNode&root)
	{
		TCHAR targetPath[_MAX_PATH];
		TCHAR relativeRootPath[_MAX_PATH];
		wmemset(relativeRootPath, 0, _MAX_PATH);
		lstrcpy(relativeRootPath, relative);
		wmemset(targetPath, 0, _MAX_PATH);
		if (path[1] != L':')
		{
			_wgetcwd(targetPath, _MAX_PATH);
			lstrcat(targetPath, L"\\");
		}
		lstrcat(targetPath, path);
		if ((_waccess(targetPath, 0)) == -1)
		{//directory not exist
			return;
		}
		_wchdir(targetPath);
		//首先查找dir中符合要求的文件  
		long hFile;
		_wfinddata_t fileinfo;
		if ((hFile = _wfindfirst(filter, &fileinfo)) != -1)
		{
			do
			{
				//检查是不是目录  
				//如果不是,则进行处理  
				if (!(fileinfo.attrib & _A_SUBDIR))
				{
					//TCHAR filename[_MAX_PATH];
					//wmemset(filename, 0, _MAX_PATH);
					//lstrcpy(filename, targetPath);
					//lstrcat(filename, L"\\");
					//lstrcat(filename, fileinfo.name);
					//wprintf(L"find file %s", filename);
					if (StringUtils::EndWith(fileinfo.name,L".meta")==false)
					{
						FileItemNode*item = new FileItemNode;
						lstrcpy(item->mName, fileinfo.name);
						item->mbIsDir = false;
						item->mLastWriteTime = fileinfo.time_write;
						root.mChildren.push_back(item);
					}
				}
			} while (_wfindnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
		//查找dir中的子目录  
		//因为在处理dir中的文件时，派生类的ProcessFile有可能改变了  
		//当前目录，因此还要重新设置当前目录为dir。  
		//执行过_findfirst后，可能系统记录下了相关信息，因此改变目录  
		//对_findnext没有影响。  
		_wchdir(targetPath);
		if ((hFile = _wfindfirst(filter, &fileinfo)) != -1)
		{
			do
			{
				//检查是不是目录  
				//如果是,再检查是不是 . 或 ..   
				//如果不是,进行迭代  
				if ((fileinfo.attrib & _A_SUBDIR))
				{
					if (lstrcmp(fileinfo.name, L".") != 0 &&
						lstrcmp(fileinfo.name, L"..") != 0&&
						fileinfo.name[0]!='.')
					{
						TCHAR subdir[_MAX_PATH];
						wmemset(subdir, 0, _MAX_PATH);
						lstrcpy(subdir, targetPath);
						lstrcat(subdir, L"\\");
						lstrcat(subdir, fileinfo.name);
						//wprintf(L"find sub dir %s\n", subdir);

						lstrcat(relativeRootPath, L"/");
						lstrcat(relativeRootPath, fileinfo.name);

						FileItemNode*item = new FileItemNode;
						lstrcpy(item->mName, fileinfo.name);
						item->mbIsDir = true;
						item->mLastWriteTime = fileinfo.time_write;
						root.mChildren.push_back(item);

						GetFiles(subdir, relativeRootPath, filter, *item);
					}
				}
			} while (_wfindnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}

	void FileSystem::GetFiles(LPCTSTR path, LPCTSTR filter, FileItemNode&root)
	{
		TCHAR targetPath[_MAX_PATH];
		TCHAR originalWorkingDir[_MAX_PATH];
		wmemset(targetPath, 0, _MAX_PATH);
		wmemset(originalWorkingDir, 0, _MAX_PATH);
		if (path[1] != L':')
		{
			_wgetcwd(targetPath, _MAX_PATH);
			lstrcpy(originalWorkingDir,targetPath);
			lstrcat(targetPath, L"\\");
			lstrcat(targetPath, path);
		}
		GetFiles(targetPath, path, filter, root);
		//由于遍历的时候会改变当前的工作目录，所以遍历完了之后要回到原先的工作目录
		_wchdir(originalWorkingDir);
	}
#endif
	bool FileSystem::isAbsolutePath(const std::string& path)
	{
#if ALICE_PLATFORM_WIN
		return (path[1]==':');
#elif ALICE_PLATFORM_UNIX
		return (path[0] == '/');
#endif
	}

	bool FileSystem::Exists(const char*path)
	{
		if (path==nullptr)
		{
			return false;
		}
#if ALICE_PLATFORM_WIN
		if (_access(path, 0x00) != 0)
#elif ALICE_IPHONE || ALICE_OSX_PLAYER || ALICE_ANDROID
		if (access(path, 0x00) != 0)
#endif
		{//not exist
			return false;
		}
		return true;
	}

	bool FileSystem::isDirectoryExist(const std::string& dirPath)
	{
#if ALICE_PLATFORM_WIN
		if (_access(dirPath.c_str(),0x00)!=0)
#elif ALICE_IPHONE
        if (access(dirPath.c_str(),0x00)!=0)
#endif
        {//not exist
			return false;
		}
		return true;
	}

#if ALICE_PLATFORM_WIN
	bool FileSystem::DeleteDir(LPCTSTR path)
	{
		SHFILEOPSTRUCT FileOp;
		FileOp.fFlags = FOF_NOCONFIRMATION|FOF_ALLOWUNDO;
		FileOp.hNameMappings = NULL;
		FileOp.hwnd = NULL;
		FileOp.lpszProgressTitle = NULL;
		FileOp.pFrom = path;
		FileOp.pTo = NULL;
		FileOp.wFunc = FO_DELETE;
		SHFileOperation(&FileOp);

		return true;
	}
#endif

	bool FileSystem::CopyFile(const char*srcPath, const char*dstPath)
	{
#if ALICE_PLATFORM_WIN
		char srcTemp[256];
		char dstTemp[256];
		memset(srcTemp, 0, 256);
		memset(dstTemp, 0, 256);
		strcpy(srcTemp, srcPath);
		strcpy(dstTemp, dstPath);
		Alice::StringUtils::ToWindowsPath(srcTemp);
		Alice::StringUtils::ToWindowsPath(dstTemp);
		SHFILEOPSTRUCTA op;
		op.hwnd = NULL;
		op.wFunc = FO_COPY;
		op.pFrom = srcTemp;
		op.pTo = dstTemp;
		op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT | FOF_ALLOWUNDO;
		op.hNameMappings = NULL;
		op.lpszProgressTitle = NULL;
		int ret=SHFileOperationA(&op);
		return ret == 0;
#endif
		return false;
	}

	bool FileSystem::MoveFile(const char*srcPath, const char*dstPath)
	{
#if ALICE_EDITOR
		char srcTemp[256];
		char dstTemp[256];
		memset(srcTemp, 0, 256);
		memset(dstTemp, 0, 256);
		strcpy(srcTemp, srcPath);
		strcpy(dstTemp, dstPath);
		Alice::StringUtils::ToWindowsPath(srcTemp);
		Alice::StringUtils::ToWindowsPath(dstTemp);
		SHFILEOPSTRUCTA op;
		op.hwnd = NULL;
		op.wFunc = FO_MOVE;
		op.pFrom = srcTemp;
		op.pTo = dstTemp;
		op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT | FOF_ALLOWUNDO;
		op.hNameMappings = NULL;
		op.lpszProgressTitle = NULL;
		SHFileOperationA(&op);
		return GetLastError() == 0;
#endif
		return false;
	}

	bool FileSystem::MoveFileToTrash(const char*path)
	{
#if ALICE_PLATFORM_WIN
		SHFILEOPSTRUCTA op;
		op.hwnd = NULL;
		op.wFunc = FO_DELETE;
		op.pFrom = path;
		op.pTo = NULL;
		op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT | FOF_ALLOWUNDO;
		op.hNameMappings = NULL;
		op.lpszProgressTitle = NULL;
		SHFileOperationA(&op);
		return GetLastError() == 0;
#endif
		return false;
	}

	bool FileSystem::DeleteFileWithPath(std::string path)
	{
#if ALICE_PLATFORM_WIN
		char szBuffer[256];
		memset(szBuffer, 0, 256);
		strcpy(szBuffer, path.c_str());
		StringUtils::ToSTDPath(szBuffer);
		SHFILEOPSTRUCTA op;
		op.hwnd = NULL;
		op.wFunc = FO_DELETE;
		op.pFrom = szBuffer;
		op.pTo = NULL;
		op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT ;
		op.hNameMappings = NULL;
		op.lpszProgressTitle = NULL;
		SHFileOperationA(&op);
		return GetLastError() == 0;
#else
		return false;
#endif
	}

	bool FileSystem::DeleteDir(std::string path)
	{
#if ALICE_PLATFORM_WIN
		char szBuffer[256];
		memset(szBuffer, 0, 256);
		strcpy(szBuffer, path.c_str());
		StringUtils::ToSTDPath(szBuffer);
		SHFILEOPSTRUCTA op;
		op.hwnd = NULL;
		op.wFunc = FO_DELETE;
		op.pFrom = szBuffer;
		op.pTo = NULL;
		op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT | FOF_ALLOWUNDO;
		op.hNameMappings = NULL;
		op.lpszProgressTitle = NULL;
		SHFileOperationA(&op);
		return GetLastError() == 0;
#endif
		return false;
	}

	bool FileSystem::CreateFile(std::string path)
	{
		// Split the path
		size_t found = path.find_last_of("/");
		std::string subpath = path.substr(0, found);
		if (isDirectoryExist(subpath)==false)
		{
			CreateDir(subpath);
		}
		std::fstream file(path,std::ios::out);
		file.close();
		return true;
	}

	bool FileSystem::CreateDir(std::string path)
	{
		// Split the path
		size_t start = 0;
		size_t found = path.find_first_of("/\\", start);
		std::string subpath;
		std::vector<std::string> dirs;

		if (found != std::string::npos)
		{
			while (true)
			{
				subpath = path.substr(start, found - start + 1);
				if (!subpath.empty())
					dirs.push_back(subpath);
				start = found + 1;
				found = path.find_first_of("/\\", start);
				if (found == std::string::npos)
				{
					if (start < path.length())
					{
						dirs.push_back(path.substr(start));
					}
					break;
				}
			}
		}
		else
		{
			dirs.push_back(path);
		}
#if ALICE_PLATFORM_WIN
		if ((GetFileAttributesA(path.c_str())) == INVALID_FILE_ATTRIBUTES)
		{
			subpath = "";
			for (unsigned int i = 0; i < dirs.size(); ++i)
			{
				subpath += dirs[i];
				if (!isDirectoryExist(subpath))
				{
					BOOL ret = CreateDirectoryA(subpath.c_str(), NULL);
					if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
					{
						return false;
					}
				}
			}
		}
		return true;
#elif ALICE_PLATFORM_UNIX
		DIR *dir = NULL;
		// Create path recursively
		subpath = "";
		for (int i = 0; i < dirs.size(); ++i){
			subpath += dirs[i];
			dir = opendir(subpath.c_str());
			if (!dir){
				// directory doesn't exist, should create a new one
				int ret = mkdir(subpath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
				if (ret != 0 && (errno != EEXIST)){
					// current directory can not be created, sub directories can not be created too
					// should return
					return false;
				}
			}else{
				// directory exists, should close opened dir
				closedir(dir);
			}
		}
		return true;
#endif
	}

	void FileSystem::CopyDir(const char*src, const char* dst)
	{
#if ALICE_PLATFORM_WIN
		SHFILEOPSTRUCTA sfo;
		ZeroMemory(&sfo, sizeof(sfo));
		char srcPath[_MAX_PATH];
		char dstPath[_MAX_PATH];
		memset(srcPath, 0, _MAX_PATH);
		memset(dstPath, 0, _MAX_PATH);
		strcpy(srcPath, src);
		strcpy(dstPath, dst);
		// \ -> /
		StringUtils::ToWindowsPath(srcPath);
		StringUtils::ToWindowsPath(dstPath);
		strcat(srcPath, "\0\0");
		strcat(dstPath, "\0\0");
		sfo.wFunc = FO_COPY; 
		sfo.pFrom = srcPath;// "Assets\0\0";
		sfo.pTo = dstPath;// "IOSbuild/InnerAssets\0";
		sfo.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
		int ret = SHFileOperationA(&sfo);
		if (ret != 0)
			Error("copy file fail %s -> %s", srcPath, dstPath);
#endif
	}
}
