#include "WinEnviroment.h"
namespace Editor {
	HINSTANCE WinEnviroment::mInstance = nullptr;
	void WinEnviroment::Init(HINSTANCE instance) {
		static Gdiplus::GdiplusStartupInput sGdiplusStartupInput;
		static ULONG_PTR sGdiplusToken;
		mInstance = instance;
		GdiplusStartup(&sGdiplusToken, &sGdiplusStartupInput, NULL);
		CoInitialize(NULL);
	}
	HINSTANCE WinEnviroment::Instance() {
		return mInstance;
	}
	HACCEL WinEnviroment::InitAccel() {
		ACCEL accel[] = {
			{ FCONTROL | FVIRTKEY, 'C', kAccelKeyCombinationCTRL_C },
			{ FCONTROL | FVIRTKEY, 'V', kAccelKeyCombinationCTRL_V }
		};
		int accel_key_count = sizeof(accel);
		return CreateAcceleratorTable(accel, accel_key_count);
	}
}