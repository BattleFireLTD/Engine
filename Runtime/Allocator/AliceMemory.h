#pragma once
#include "Runtime/BattleFirePrefix.h"
#include "DefaultAllocator.h"
#include "MemoryLabel.h"
extern "C" void InitMemory();
extern "C" void OnQuitMemory();
unsigned long GetTotalReservedMemory();