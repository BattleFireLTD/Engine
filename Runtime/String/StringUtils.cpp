#include "StringUtils.h"
#include "Runtime/Debugger/Logger.h"
namespace Alice
{
	int StringUtils::UTF82UTF32(const AliceUInt8 * src, AliceUInt32 & dst)
	{
		int		i, iLen;
		AliceUInt8	b;
		b = *src++;
		if (b < 0x80)
		{
			dst = b;
			return 1;
		}

		if (b < 0xC0 || b > 0xFD)
		{	// 非法UTF8
			return 0;
		}

		if (b < 0xE0)
		{
			dst = b & 0x1F;
			iLen = 2;
		}
		else if (b < 0xF0)
		{
			dst = b & 0x0F;
			iLen = 3;
		}
		else if (b < 0xF8)
		{
			dst = b & 7;
			iLen = 4;
		}
		else if (b < 0xFC)
		{
			dst = b & 3;
			iLen = 5;
		}
		else
		{
			dst = b & 1;
			iLen = 6;
		}

		for (i = 1; i < iLen; i++)
		{
			b = *src++;
			if (b < 0x80 || b > 0xBF)
			{	// 非法UTF8
				break;
			}

			dst = (dst << 6) + (b & 0x3F);
		}

		if (i < iLen)
		{	// 非法UTF8
			return 0;
		}
		else
		{
			return iLen;
		}
	}

	int StringUtils::UTF8ToUTF32(const unsigned char*src, int srcLen, AliceUInt32 *dst)
	{
		int currentSrcPos = 0;
		int i = 0;
		for (; currentSrcPos < srcLen;)
		{
			int currentLen = UTF82UTF32(src + currentSrcPos, dst[i++]);
			if (currentLen==0)
			{
				return 0;
			}
			currentSrcPos += currentLen;
		}
		return i;
	}

	int StringUtils::UTF8ToUTF32(const unsigned char*src, int srcLen, UTF32Char *dst)
	{
		int currentSrcPos = 0;
		int i = 0;
		for (; currentSrcPos < srcLen;)
		{
			int currentLen = UTF82UTF32(src + currentSrcPos, dst[i].mCharCode);
			dst[i++].mByteCount = currentLen;
			if (currentLen == 0)
			{
				return 0;
			}
			currentSrcPos += currentLen;
		}
		return i;
	}
    
    
    bool StringUtils::UTF32CharToUTF8(const AliceUInt32 src, unsigned char*dst, int &len) {
        if (src < 0 || src > 0x10FFFF) {
            return false;
        }
        if (src < 128) {
            dst[0] = char(src);
            dst[1] = 0;
            dst[2] = 0;
            dst[3] = 0;
            len = 1;
        }
        else if (src < 2048) {
            dst[0] = 0xC0 | char(src >> 6);
            dst[1] = 0x80 | (char(src) & 0x3F);
            dst[2] = 0;
            dst[3] = 0;
            len = 2;
        }
        else if (src < 65536) {
            dst[0] = 0xE0 | char(src >> 12);
            dst[1] = 0x80 | (char(src >> 6) & 0x3F);
            dst[2] = 0x80 | (char(src) & 0x3F);
            dst[3] = 0;
            len = 3;
        }
        else {
            dst[0] = 0xF0 | char(src >> 18);
            dst[1] = 0x80 | (char(src >> 12) & 0x3F);
            dst[2] = 0x80 | (char(src >> 6) & 0x3F);
            dst[3] = 0x80 | (char(src) & 0x3F);
            len = 4;
        }
        return true;
    }
    int StringUtils::UTF8CharLen(char ch) {
        if (0 <= uint8_t(ch) && uint8_t(ch) < 0x80) {
            return 1;
        }
        if (0xC2 <= uint8_t(ch) && uint8_t(ch) < 0xE0) {
            return 2;
        }
        if (0xE0 <= uint8_t(ch) && uint8_t(ch) < 0xF0) {
            return 3;
        }
        if (0xF0 <= uint8_t(ch) && uint8_t(ch) < 0xF8) {
            return 4;
        }
        return 0;
    }
    bool StringUtils::IsUTF8Char(char ch){
        return 0x80 <= uint8_t(ch) && uint8_t(ch) < 0xC0;
    }
    bool StringUtils::UTF32ToUTF8(const AliceUInt32*src, int srcLen, unsigned char*dst)
    {
        return true;
    }
    bool StringUtils::UTF32CharToUTF16(const char32_t u32Ch, AliceSInt16 * u16Ch) {
        if (u32Ch < 0 || u32Ch > 0x10FFFF) {
            return false;
        }
        
        if (u32Ch < 0x10000) {
            u16Ch[0] = char16_t(u32Ch);
            u16Ch[1] = 0;
        }
        else {
            u16Ch[0] = char16_t((u32Ch - 0x10000) / 0x400 + 0xD800);
            u16Ch[1] = char16_t((u32Ch - 0x10000) % 0x400 + 0xDC00);
        }
        
        return true;
	}
#ifdef _WIN32
	void StringUtils::UTF8ToUnicode(const char*pStr, LPTSTR pDst)
	{
		MultiByteToWideChar(CP_UTF8, 0, pStr, -1, pDst, 200);
	}
	void StringUtils::ASCIIToUnicode(const char*pStr, LPTSTR pDst)
	{
		MultiByteToWideChar(CP_ACP, 0, pStr, -1, pDst, 200);
	}

	void StringUtils::UnicodeToASCII(const wchar_t *wchar, char *chr, int length)
	{
		WideCharToMultiByte(CP_ACP, 0, wchar, -1,chr, length, NULL, NULL);
	}

	int StringUtils::UnicodeToUTF32(const wchar_t*src, AliceUInt32 *dst)
	{
		char szTemp[256];
		memset(szTemp, 0, 256);
		UnicodeToUTF8(src,szTemp);
		return UTF8ToUTF32((const unsigned char*)szTemp, strlen(szTemp), dst);
	}


	void StringUtils::UnicodeToUTF8(const wchar_t*src, char *dst)
	{
		WideCharToMultiByte(CP_UTF8, 0, src, -1, dst, 256, NULL, NULL);
	}

	int StringUtils::GetStringLen(LPTSTR str)
	{
		return lstrlen(str);
	}

#endif
#if ALICE_EDITOR||ALICE_WIN_PLAYER
	void StringUtils::TrimStart(LPTSTR original, LPCTSTR startToTrim)
	{
		if (wmemcmp(startToTrim,original,lstrlen(startToTrim))==0)
		{
			TCHAR szBuffer[256];
			wmemset(szBuffer, 0, 256);
			lstrcpy(szBuffer, original + lstrlen(startToTrim));
			wmemset(original, 0, lstrlen(original));
			lstrcpy(original, szBuffer);
		}
	}

	void StringUtils::TrimEnd(LPTSTR original, LPCTSTR endToTrim)
	{
		wmemset(original + lstrlen(original)-lstrlen(endToTrim), 0, lstrlen(endToTrim));
	}

	bool StringUtils::EndWith(LPCTSTR original, LPCTSTR end)
	{
		if (lstrcmp(original + lstrlen(original) - lstrlen(end), end) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	LPCTSTR StringUtils::GetFileExtensionFromPathT(LPCTSTR path)
	{
		size_t len = lstrlen(path);
		LPCTSTR ret = nullptr;
		for (int i = len - 1; i > 0; i--)
		{
			if (path[i] == L'.')
			{
				ret = path + i;
			}
		}
		return ret;
	}

#endif
	void StringUtils::TrimStart(char* original, const char* startToTrim){
		if (memcmp(startToTrim, original, strlen(startToTrim)) == 0){
			char szBuffer[256];
			memset(szBuffer, 0, 256);
			strcpy(szBuffer, original + strlen(startToTrim));
			memset(original, 0, strlen(original));
			strcpy(original, szBuffer);
		}
	}
	void StringUtils::TrimEnd(char* original, const char* endToTrim){
		if (EndWith(original,endToTrim)){
			memset(original + strlen(original) - strlen(endToTrim), 0, strlen(endToTrim));
		}
	}

	bool StringUtils::EndWith(const char * original, const char* end)
	{
		if (strcmp(original + strlen(original) - strlen(end), end) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool StringUtils::EndWithI(const char * original, const char* end)
	{
#if ALICE_EDITOR||ALICE_WIN_PLAYER
		if (_stricmp(original + strlen(original) - strlen(end), end) == 0)
#else
		if (strcasecmp(original + strlen(original) - strlen(end), end) == 0)
#endif
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void StringUtils::SplitPathToComponent(char*path, std::vector<std::string>&components)
	{
		ToSTDPath(path);
		int nlen = strlen(path);
		char szTemp[64];
		int nStartPos = 0;

		memset(szTemp, 0, 64);
		for (int i = 0; i < nlen;i++)
		{
			if (path[i]=='/')
			{
				nStartPos = i + 1;
				components.push_back(szTemp);
				memset(szTemp, 0, 64);
			}
			else
			{
				szTemp[i - nStartPos] = path[i];
			}
		}
		components.push_back(szTemp);//last component
	}

	bool StringUtils::StartWith(const char* original, const char* start)
	{
		if (memcmp(original, start,strlen(start)) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	const char* StringUtils::GetFileExtensionFromPath(const char*path)
	{
		size_t len = strlen(path);
		const char *ret = nullptr;
		for (int i = len - 1; i > 0; i--)
		{
			if (path[i]=='.')
			{
				ret = path + i;
				break;
			}
		}
		return ret;
	}

	void StringUtils::GetDirFromFullPath(const char*path,char*dir)
	{
		size_t len = strlen(path);
		for (int i = len - 1; i > 0; i--)
		{
			if (path[i] == '/'||path[i]=='\\')
			{
				memcpy(dir,path,i);
				return;
			}
		}
	}

	void StringUtils::TrimFileExtension(char*path)
	{
		const char*extension=GetFileExtensionFromPath(path);
		if (extension!=nullptr)
		{
			TrimEnd(path, extension);
		}
	}

	void StringUtils::GetFileNameWithOutExtension(const char*path, char *name)
	{
		const char*extension = GetFileExtensionFromPath(path);
		size_t len = strlen(path);
		bool bFilledData = false;
		for (int i = len - 1; i >=0; i--)
		{
			if (path[i] == '/'||path[i]=='\\')
			{
				strcpy(name, path + i+1);
				bFilledData = true;
				break;
			}
		}
		if (!bFilledData)
		{
			strcpy(name, path);
		}
		if (extension!=NULL)
		{
			TrimEnd(name, extension);
		}
	}

	void StringUtils::ToSTDPath(char*buffer)
	{
		size_t nCount = strlen(buffer);
		for (size_t i = 0; i < nCount;++i)
		{
			if (buffer[i]=='\\')
			{
				buffer[i] = '/';
			}
		}
	}

	void StringUtils::ToLuaPath(char *path)
	{
		int nLen = strlen(path);
		for (int i = 0; i < nLen; i++)
		{
			if (path[i] == '/' ||
				path[i] == '\\')
			{
				path[i] = '.';
			}
		}
	}

	void StringUtils::LuaPathToSTDPath(char *path)
	{
		int nLen = strlen(path);
		for (int i = 0; i < nLen; i++)
		{
			if (path[i] == '.')
			{
				path[i] = '/';
			}
		}
	}

	void StringUtils::ToWindowsPath(char*buffer)
	{
		size_t nCount = strlen(buffer);
		for (size_t i = 0; i < nCount; ++i)
		{
			if (buffer[i] == '/')
			{
				buffer[i] = '\\';
			}
		}
	}
	int StringUtils::GetNextPosOfLine(char*buffer, int len, char c) {
		for (int i = 0; i < len; ++i) {
			if (buffer[i] == c) {
				return i;
			}else if (buffer[i]=='\n'){
				return i;
			}
		}
		return -1;
	}
	int StringUtils::GetNextPosOf(const char*buffer, int len, char c) {
		for (int i = 0; i < len; ++i) {
			if (buffer[i] == c) {
				return i;
			}
		}
		return -1;
	}
	int StringUtils::GetNextPosOfInvert(const char*buffer, int len, char c) {
		for (int i = len - 1; i >= 0; --i) {
			if (buffer[i] == c) {
				return i;
			}
		}
		return -1;
	}
	bool StringUtils::IsNoneSymbol(char c) {
		if (c == '\0') return 1; //we want to write null regardless
		int c_int = (int)c;
		return (c_int >= 48 && c_int <= 57) || (c_int >= 65 && c_int <= 90) || (c_int >= 97 && c_int <= 122);
	}
	void StringUtils::BytesToHexStr(const unsigned char *bytes, int len, char * buffer) {
		int out_buffer_index = 0;
		for (int i=0;i<len;++i){
			sprintf(buffer + out_buffer_index, "%02x", bytes[i]);
			out_buffer_index+=2;
		}
	}
}
