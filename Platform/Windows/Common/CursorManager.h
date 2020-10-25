#pragma once
#include <windows.h>
#include <unordered_map>
#include <string>
namespace Editor{
	class CursorManager {
	protected:
		static CursorManager * mSingleton;
		bool mbCursorLocked;
		std::unordered_map<std::string, HCURSOR> mCursors;
	protected:
		HCURSOR GetCursor(const char * cursor_name);
	public:
		CursorManager();
		void Init();
		void ShowCursor(const char * cursor_name);
		void LockCursor();
		void UnlockCursor();
		bool IsCursorLocked();
		static CursorManager * Singleton();
	};
}