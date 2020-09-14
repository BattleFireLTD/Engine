#pragma once
#define ALICE_DEPRECATED __declspec(deprecated)
#if defined(__GNUC__)
#define ALIGN_OF(T) __alignof__(T)
#define ALIGN_TYPE(val) __attribute__((aligned(val)))
#define FORCE_INLINE inline __attribute__ ((always_inline))
#elif defined(_MSC_VER)
#define ALIGN_OF(T) __alignof(T)
#define ALIGN_TYPE(val) __declspec(align(val))
#define FORCE_INLINE __forceinline
#else
#define ALIGN_TYPE(size)
#define FORCE_INLINE inline
#endif
//types
#if ALICE_IPHONE || ALICE_ANDROID
#define _MAX_PATH 256
#endif

#if ALICE_EDITOR || ALICE_WIN_PLAYER
#define ALICE_PLATFORM_WIN 1
#include <WinSock2.h>
#include <windows.h>
#include <dwmapi.h>
#include <ShObjIdl.h>
#include <shellapi.h>
#include <WinNls.h>
#include <wchar.h>
#include <tchar.h>
#include <io.h>
#include <direct.h>
#include <gdiplus.h>
#define NOMINMAX
#define ALICE_STRICMP _stricmp 
#elif defined(ALICE_OSX_PLAYER) || defined(ALICE_ANDROID) || defined(ALICE_IPHONE)
#define ALICE_PLATFORM_UNIX 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <net/if.h>
#include <pthread.h>
#include <dirent.h>
#ifdef ALICE_IPHONE
#include <ifaddrs.h>
#endif//end of ALICE_IPHONE
#ifdef ALICE_ANDROID
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/native_window_jni.h>
#define JAVA_API(return_type) extern "C" JNIEXPORT return_type JNICALL
#define JFOO(CLS,METHOD) Java_com_alice_battlefire_androidplayer_##CLS##_##METHOD
#if defined(__arm64__)||defined(__aarch64__)
#define ANDROID_ARCH "arm64-v8a"
#elif defined(__arm__)
#define ANDROID_ARCH "armeabi-v7a"
#else
#define ANDROID_ARCH "Unkown"
#endif//end of defined(__arm64__)||defined(__aarch64__)
#endif//end of ALICE_ANDROID
#define ALICE_STRICMP strcasecmp
#endif//end of ALICE_PLATFORM_UNIX
#include <string>
#include <vector>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <functional>
typedef signed char AliceSInt8;
typedef unsigned char AliceUInt8;
typedef AliceUInt8 AliceUByte;
typedef AliceSInt8 AliceByte;
typedef signed short AliceSInt16;
typedef unsigned short AliceUInt16;
typedef int AliceSInt32;
typedef unsigned int AliceUInt32;
typedef unsigned long long AliceUInt64;
typedef signed long long AliceSInt64;
typedef AliceSInt32 LuaScriptHandle;
typedef void* AliceAny;
#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif
#define _00000000_00000000_00000000_00000001 1
#define _00000000_00000000_00000000_00000010 2
#define _00000000_00000000_00000000_00000100 4
#define _00000000_00000000_00000000_00001000 8
#define _00000000_00000000_00000000_00010000 16
#define _00000000_00000000_00000000_00100000 32
#define _00000000_00000000_00000000_01000000 64
#define _00000000_00000000_00000000_10000000 128
#define _00000000_00000000_00000001_00000000 256
#define _00000000_00000000_00000010_00000000 512
#define _00000000_00000000_00000100_00000000 1024
#define _00000000_00000000_00001000_00000000 2048
#define _00000000_00000000_00010000_00000000 4096
#define _00000000_00000000_00100000_00000000 8192
#define _00000000_00000000_01000000_00000000 16384
#define _00000000_00000000_10000000_00000000 32768
typedef void(*VOID_VOID_PTR)(void*);
typedef void(*VOID_NO_ARG)();