#include "DefaultAllocator.h"
#include <stdio.h>
#include <stdlib.h>
#include "AliceMemory.h"
#include "tlsf/tlsf.h"
#include <unordered_map>
#include <unordered_set>

//1024 1KB
//1024x1024 1MB 1048576
//1024x1024x1024 1GB 1073741824
static unsigned long sReservedTotalMemory = 50485760;
static unsigned int mReservedSize = 504857600;//500MB
static tlsf_t sTLSF=nullptr;
void AllocateMemoryFromSystem(std::size_t size){
	void*buffer = malloc(mReservedSize);
	tlsf_add_pool(sTLSF, buffer, mReservedSize);
	sReservedTotalMemory += 50485760;
}
unsigned long GetTotalReservedMemory(){
	return sReservedTotalMemory;
}
static bool sbAppQuit = false;
void InitMemory(){
#if USE_POOL
	void*buffer = malloc(mReservedSize);
	sTLSF = tlsf_create_with_pool(buffer, mReservedSize);
#endif
}
//on windows platform the min memory block is 12 bytes
static void*GetMemory(size_t size){
	void * ptrMemory = nullptr;
	ptrMemory = tlsf_malloc(sTLSF, size);
	while (ptrMemory==nullptr){
		AllocateMemoryFromSystem(mReservedSize);
		ptrMemory = tlsf_malloc(sTLSF, size);
	}
	return ptrMemory;
}
void OnQuitMemory(){
	sbAppQuit = true;
}
static void Recycle(void*ptr){
	if (sbAppQuit){
		return;
	}
#if USE_POOL
	if (Alice::Profiler::OnFreeObject(ptr)){
		tlsf_free(sTLSF, ptr);
	}else{
		free(ptr);
#else
	//Debug("delete %p",ptr);
		free(ptr);
#endif
#if USE_POOL
	}
#endif
}

void*operator new(std::size_t size) {
	size_t alloc_size = ALICE_ADJUST_MEMORY_SIZE(size);
	void* ptr = malloc(alloc_size);
	return ptr;
}

void*operator new(std::size_t size, MemoryLabel memID){
#if USE_POOL
	if (memID==kMemEditorId){
		void*ptr = malloc(size);
		return ptr;
	}
	void*ptr = GetMemory(size);
	Alice::Profiler::OnNewObject(ptr, memID);
	return ptr;
#else
	size_t alloc_size = ALICE_ADJUST_MEMORY_SIZE(size);
	void* ptr = malloc(alloc_size);
	return ptr;
#endif
}

void*operator new[](std::size_t size, MemoryLabel memID){
#if USE_POOL
	if (memID == kMemEditorId){
		void*ptr = malloc(size);
		return ptr;
	}
	void*ptr = GetMemory(size);
	Alice::Profiler::OnNewObject(ptr, memID);
	return ptr;
#else
	size_t alloc_size = ALICE_ADJUST_MEMORY_SIZE(size);
	void* ptr = malloc(alloc_size);
	return ptr;
#endif
}

void operator delete(void*ptr,MemoryLabel memID){
	//Error("un implemented method");
}

void operator delete[](void*ptr, MemoryLabel memID){
	//Error("un implemented method");
}
#if ALICE_ANDROID
void operator delete(void*ptr)noexcept(true){
	Recycle(ptr);
}

void operator delete [](void*ptr)noexcept(true){
	Recycle(ptr);
}
#else
void operator delete(void*ptr){
	Recycle(ptr);
}

void operator delete [](void*ptr){
	Recycle(ptr);
}
#endif
