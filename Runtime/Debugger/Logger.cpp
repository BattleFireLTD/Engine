#include <iostream>
#include <fstream>
#include <stdarg.h>
#include <time.h>
#include <string.h>
using namespace std;
#include "Logger.h"
#if ALICE_ANDROID
#define  LOG_TAG    "Alice"
#endif
char szEngineLog[MAX_PATH_FOR_LOG];
static void(*gEngineErrorReporter)(const char*msg) = nullptr;
void SetEngineErrorReporter(void(*foo)(const char*msg)){
	gEngineErrorReporter = foo;
}
void ReportEngineError(const char*msg){
	if (gEngineErrorReporter!=nullptr){
		gEngineErrorReporter(msg);
	}else{
		Error("%s",msg);
	}
}
int  FormatCurrDate(char *szTime,char *szDate){
	time_t t;
	time(&t);
	struct tm *today;
	today=localtime(&t);
	strftime(szTime,32,"%H:%M:%S",today);
	strftime(szDate,32,"%y-%m-%d",today);
	return 0;
}
int InitEngineLog(const char *engineLog){
	int nLen=strlen(engineLog);
	if ((nLen>=MAX_PATH_FOR_LOG)||(nLen<=0))
		return -1;
	memset(szEngineLog,0,MAX_PATH_FOR_LOG);
	strcpy(szEngineLog, engineLog);
	return 0;
}

void WriteLog(const char* tag,const char*file,int nLine,const char* szLogContent){
	char szTime[32];
	char szDate[32];
	memset(szTime,0,sizeof(szTime));
	memset(szDate,0,sizeof(szDate));
	FormatCurrDate(szTime, szDate);
#if RELEASE && !ALICE_WIN_PREPUBLISH && !DEV_PUBLISH
	string fn;
	fn = szEngineLog;
	fn += ".";
	fn += szDate;
	ofstream fd;
	try{
		fd.open(fn.c_str(), ios::app);
		fd << szTime << " : " << tag << " " << szLogContent << endl;
		fd.close();
	}catch (...){
		cerr << "open log file exception!" << endl;
		return;
	}
#else
	printf("%s %s %s\n", szTime, tag, szLogContent);
#endif
}

void DebugLog(const char*file,int nLine,const char*format,...){
	if(strlen(format) == 0)
		return ;
	char szBuffer[MAX_LOG_LENGTH];
	memset(szBuffer,0,MAX_LOG_LENGTH);
	va_list	l_va;
	va_start(l_va,format);
	vsnprintf(szBuffer,sizeof(szBuffer),format,l_va);
	va_end(l_va);
#if _DEBUG
	WriteLog("[debug] ",file,nLine,szBuffer);
#endif
}

void InfoLog(const char*file,int nLine,const char*format,...){
	if(strlen(format) == 0)
		return ;
	char szBuffer[MAX_LOG_LENGTH];
	memset(szBuffer,0,MAX_LOG_LENGTH);
	va_list	l_va;
	va_start(l_va, format); 
	vsnprintf(szBuffer,sizeof(szBuffer),format,l_va);
	va_end(l_va);
#if ALICE_PLATFORM_UNIX
#if ALICE_ANDROID
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, " [info] %s %d %s\n", file, nLine, szBuffer);
#elif ALICE_IPHONE
	printf(" [info] %s %d %s\n", file, nLine, szBuffer);
#else
    #if ALICE_OSX_PLAYER_RELEASE_DEBUG
        printf(" [info] %s %d %s\n", file, nLine, szBuffer);
    #else
        WriteLog(" [info] ",file,nLine,szBuffer);
    #endif
#endif
#else
	WriteLog(" [info] ",file,nLine,szBuffer);
#endif
}

void ErrorLog(const char*file,int nLine,const char*format,...)
{
	if(strlen(format) == 0)
		return ;
	char szBuffer[MAX_LOG_LENGTH];
	memset(szBuffer,0,MAX_LOG_LENGTH);

	va_list	l_va;
	va_start(l_va,format);
	vsnprintf(szBuffer,sizeof(szBuffer),format,l_va);
	va_end(l_va);

#if ALICE_PLATFORM_UNIX
#if ALICE_ANDROID
	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "[error] %s %d %s\n", file, nLine, szBuffer);
#elif ALICE_IPHONE || ALICE_OSX_PLAYER
	printf("[error] %s %d %s\n", file, nLine, szBuffer);
#endif
#else
	WriteLog("[error]", file, nLine, szBuffer);
#endif
}

void EditorErrorLog(const char*file, int nLine, const char*format, ...){
	if (strlen(format) == 0)
		return;
	char szBuffer[MAX_LOG_LENGTH];
	memset(szBuffer, 0, MAX_LOG_LENGTH);
	va_list	l_va;
	va_start(l_va, format);
	vsnprintf(szBuffer, sizeof(szBuffer), format, l_va);
	va_end(l_va);
	if (gEngineErrorReporter) {
		char szTime[32];
		char szDate[32];
		string fn;
		memset(szTime, 0, sizeof(szTime));
		memset(szDate, 0, sizeof(szDate));
		FormatCurrDate(szTime, szDate);
		fn = szEngineLog;
		fn += ".";
		fn += szDate;
		gEngineErrorReporter(fn.c_str());
	}else{

#if ALICE_PLATFORM_UNIX
#if ALICE_ANDROID
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "[errorC] %s %d %s\n", file, nLine, szBuffer);
#elif ALICE_IPHONE || ALICE_OSX_PLAYER
		printf("[errorC] %s %d %s\n", file, nLine, szBuffer);
#endif
#else
		WriteLog("[errorC]", file, nLine, szBuffer);
#endif
	}
}
