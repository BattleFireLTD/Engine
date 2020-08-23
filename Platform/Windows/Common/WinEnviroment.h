#pragma once
#include "Runtime/BattleFirePrefix.h"
namespace Editor{
	enum kAccelKeyCombination {
		kAccelKeyCombinationUnkown,
		kAccelKeyCombinationCTRL_C,
		kAccelKeyCombinationCTRL_V,
		kAccelKeyCombinationCount
	};
	class WinEnviroment {
	protected:
		static HINSTANCE mInstance;
	public:
		static void Init(HINSTANCE instance);
		static HINSTANCE Instance();
		static HACCEL InitAccel();
	};
}