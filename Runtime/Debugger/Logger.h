#pragma once
#include "Runtime/BattleFirePrefix.h"
#include <stdio.h>
#define MAX_PATH_FOR_LOG	256
#define MAX_LOG_LENGTH		10240
int InitEngineLog(const char *engineLogFilePath);
int FormatCurrDate(char *szTime,char *szDate);
void DebugLog(const char*file,int nLine,const char*format,...);//debug log
void InfoLog(const char*file,int nLine,const char*format,...);//info log
void ErrorLog(const char*file, int nLine, const char*format, ...);//normal error,need not to be displayed to user
void EditorErrorLog(const char*file, int nLine, const char*format, ...);//error log that need to display to the editor console window

void SetEngineErrorReporter(void(*foo)(const char*msg));
void ReportEngineError(const char*msg);

#if ALICE_PLATFORM_WIN && _DEBUG
#define Debug(f,...) DebugLog(__FILE__,__LINE__,f,##__VA_ARGS__)
#else
#define  Debug(f,...)
#endif
#define Error(f,...) ErrorLog(__FILE__,__LINE__,f,##__VA_ARGS__)
#define Info(f,...) InfoLog(__FILE__,__LINE__,f,##__VA_ARGS__)
#define errorC(f,...) EditorErrorLog(__FILE__,__LINE__,f,##__VA_ARGS__)
