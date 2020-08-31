#pragma once
#include <iostream>
#include <io.h>
#include <direct.h>
#include <string>
using namespace std;
#include "Platform/Windows/Common/PlatformWindowsSeriallizer.pb.h"
#include "External/ProtoBuffer3.13/src/google/protobuf/text_format.h"
int main(int argc, char **argv){
	PlatformWindowsSeriallizer::WinResources *editorResources = new PlatformWindowsSeriallizer::WinResources;
	_chdir("Raw");
	long hFile;
	_finddata_t fileinfo;
	if ((hFile = _findfirst("*.*", &fileinfo)) != -1){
		do{
			if (!(fileinfo.attrib & _A_SUBDIR)){
				FILE *pFile = NULL;
				pFile = fopen(fileinfo.name, "rb");
				if (pFile != NULL){
					fseek(pFile, 0, SEEK_END);
					int mDataLen = ftell(pFile);
					rewind(pFile);
					if (mDataLen > 0){
						char*data=new char[sizeof(char) *mDataLen];
						mDataLen = fread(data, sizeof(char), mDataLen, pFile);
						editorResources->mutable_res()->insert(google::protobuf::MapPair<std::string,std::string>(fileinfo.name,std::string(data,mDataLen)));
					}
					fclose(pFile);
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
		cout << "total size "<<editorResources->ByteSizeLong()/1024<<"KB" << endl;

		char *szBuffer=new char[editorResources->ByteSizeLong()];
		editorResources->SerializeToArray(szBuffer, editorResources->ByteSizeLong());
		FILE *pFile = NULL;
		pFile = fopen("../res", "wb");
		if (pFile != NULL){
			fwrite(szBuffer,1,editorResources->ByteSizeLong(),pFile);
			fclose(pFile);
		}
	}
	return 0;
}