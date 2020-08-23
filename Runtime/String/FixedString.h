#pragma once
#include <string>
namespace Alice{
	class FixedString{
	public:
		char*mText;
		int mLen;
		int mBufferSize;
	private:
		FixedString(const FixedString&r) {}
	public:
		FixedString();
		FixedString(int len);
		~FixedString();
		void Resize(int len);
		void Set(const char * str, int len);
		bool operator==(const char*text);
		bool operator==(const FixedString&str);
		bool operator==(const std::string&str);
		void operator=(const char*text);
		void operator=(const FixedString&str);
		void operator=(const std::string&str);
		bool operator!=(const char*text);
		bool operator!=(const FixedString&str);
		bool operator!=(const std::string&str);
		//copy fixed string text to buffer
		void operator >> (char*buffer);
		//append text to text
		void operator << (const char*buffer);
		bool StartWith(const char*start);
		bool EndWith(const char*end);
		void TrimEnd(const char*end);
		void TrimEndWithByteCount(int nLen);
		void TrimExtension();
		void ToSTDPath();
		void ToLuaPath();
	};
}