#include "FixedString.h"
#include "Runtime/Allocator/AliceMemory.h"
#include "StringUtils.h"

namespace Alice{
	FixedString::FixedString() :mText(nullptr), mLen(0),mBufferSize(0){
	}
	FixedString::FixedString(int len):mLen(0), mBufferSize(len){
		mText = new (kMemDefaultId)char[len];
		memset(mText, 0, len);
	}
	FixedString::~FixedString(){
		if (mText != nullptr){
			delete [] mText;
			mText = nullptr;
		}
		mLen = 0;
		mBufferSize = 0;
	}
	void FixedString::Resize(int len){
		if (mText!=nullptr){
			delete [] mText;
		}
		mText = new (kMemDefaultId)char[len];
		memset(mText, 0, len);
		mBufferSize = len;
		mLen = 0;
	}
	bool FixedString::operator==(const char*text){
		return strcmp(text, mText) == 0;
	}
	bool FixedString::operator==(const FixedString&str){
		return strcmp(str.mText, mText) == 0;
	}
	bool FixedString::operator==(const std::string&str){
		return strcmp(str.c_str(), mText) == 0;
	}
	void FixedString::operator=(const char*text){
		int nLen = strlen(text);
		if (nLen < mBufferSize) {
			memset(mText, 0, mBufferSize);
			strcpy(mText, text);
			mLen = nLen;
		}else{
			memcpy(mText, text, mBufferSize-1);
			mLen = mBufferSize;
			mText[mBufferSize - 1] = '\0';
		}
	}
	void FixedString::operator=(const FixedString&str) {
		*this = str.mText;
	}
	void FixedString::operator=(const std::string&str) {
		*this = str.c_str();
	}
	void FixedString::Set(const char * str, int len) {
		if ((len+1) > mBufferSize) {
			Resize(len + 1);
		}
		memcpy(mText, str, len);
		mLen = len;
		mText[len] = '\0';
	}
	bool FixedString::operator!=(const char*text){
		return strcmp(text, mText) != 0;
	}
	bool FixedString::operator!=(const FixedString&str){
		return strcmp(str.mText, mText) != 0;
	}
	bool FixedString::operator!=(const std::string&str){
		return strcmp(str.c_str(), mText) != 0;
	}
	void FixedString::operator >> (char*buffer){
		strcpy(buffer, mText);
	}
	void FixedString::operator<< (const char*buffer){
		int nLen = strlen(buffer);
		if (nLen + mLen < mBufferSize) {
			strcat(mText, buffer);
			mLen += nLen;
		}
		else {
			memcpy(mText+mLen, buffer, mBufferSize - 1 - mLen);
			mLen = mBufferSize;
			mText[mBufferSize - 1] = '\0';
		}
	}
	bool FixedString::StartWith(const char*start){
		return StringUtils::StartWith(mText, start);
	}
	bool FixedString::EndWith(const char*end){
		return StringUtils::EndWith(mText, end);
	}
	void FixedString::TrimEnd(const char*end){
		StringUtils::TrimEnd(mText, end);
	}
	void FixedString::TrimEndWithByteCount(int nLen){
		if (nLen<=mLen){
			memset(mText + mLen - nLen, 0, nLen);
			mLen -= nLen;
		}
	}
	void FixedString::TrimExtension(){
		StringUtils::TrimFileExtension(mText);
	}
	void FixedString::ToLuaPath(){
		StringUtils::ToLuaPath(mText);
	}
	void FixedString::ToSTDPath(){
		StringUtils::ToSTDPath(mText);
	}
}
