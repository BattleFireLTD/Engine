#pragma once
#include "Runtime/BattleFirePrefix.h"
namespace Alice
{
	struct UTF32Char
	{
		AliceUInt32 mCharCode;
		AliceUInt32 mByteCount;
	};
	class StringUtils
	{
	public:
		static int UTF8ToUTF32(const unsigned char*src, int srcLen, UTF32Char *dst);
		static int UTF8ToUTF32(const unsigned char*src,int srcLen,AliceUInt32 *dst);
		static bool UTF32ToUTF8(const AliceUInt32*src, int srcLen, unsigned char*dst);
		static bool UTF32CharToUTF8(const AliceUInt32 src, unsigned char*dst, int &len);
		static bool UTF32CharToUTF16(const char32_t u32Ch, AliceSInt16 * u16Ch);
		static int UTF8CharLen(char ch);
		static bool IsUTF8Char(char ch);
		static int UTF82UTF32(const AliceUInt8 * src, AliceUInt32 & dst);
#ifdef _WIN32
		static int UnicodeToUTF32(const wchar_t*src, AliceUInt32 *dst);
		static void UnicodeToUTF8(const wchar_t*src,char *dst);
		static void UnicodeToASCII(const wchar_t *wchar, char *chr, int length);
		static void ASCIIToUnicode(const char*pStr, LPTSTR pDst);
		static void UTF8ToUnicode(const char*pStr, LPTSTR pDst);
		static int GetStringLen(LPTSTR str);
		template<typename T>
		static bool SortName(const T &l, const T &r)
		{
			char lName[64];
			char rName[64];
			memset(lName, 0, 64);
			memset(rName, 0, 64);
			StringUtils::UnicodeToASCII(l->mName, lName, 64);
			StringUtils::UnicodeToASCII(r->mName, rName, 64);
			if (strcmp(lName, rName) < 0)
			{
				return true;
			}
			return false;
		}
#endif
#if ALICE_EDITOR||ALICE_WIN_PLAYER
		static void TrimStart(LPTSTR original,LPCTSTR startToTrim);
		static void TrimEnd(LPTSTR original, LPCTSTR endToTrim);
		static bool EndWith(LPCTSTR original, LPCTSTR end);
		static LPCTSTR  GetFileExtensionFromPathT(LPCTSTR path);
#endif

		static void TrimStart(char* original, const char* startToTrim);
		static void TrimEnd(char* original, const char* endToTrim);
		static bool EndWith(const char * original, const char* end);
		static bool EndWithI(const char * original, const char* end);
		static bool StartWith(const char *original,const char*start);
		static void SplitPathToComponent(char*path,std::vector<std::string>&components);
		//return the file type extension,etc .png .jpg
		static const char* GetFileExtensionFromPath(const char*path);
		//return the dir of the file
		static void GetDirFromFullPath(const char*path,char*dir);
		static void TrimFileExtension(char*path);
		static void GetFileNameWithOutExtension(const char*path,char *name);
		//convert '\' to '/'
		static void ToSTDPath(char*buffer);
		static void ToLuaPath(char*path);
		static void LuaPathToSTDPath(char*path);
		static void ToWindowsPath(char*path);
		//
		static int GetNextPosOf(const char*buffer, int len, char c);
		static int GetNextPosOfLine(char*buffer, int len, char c);
		static int GetNextPosOfInvert(const char*buffer, int len, char c);
		static void URLEncode(const char *input, int len, char *output);
		static int URLDecode(const char *input, int len, char *output);
		static bool IsNoneSymbol(char c);
		static void BytesToHexStr(const unsigned char *bytes,int len,char * buffer);
	};
}
