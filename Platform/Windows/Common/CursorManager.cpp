#include "CursorManager.h"
namespace Editor {
	CursorManager*CursorManager::mSingleton = nullptr;
	CursorManager::CursorManager() {
		mbCursorLocked = false;
	}
	CursorManager*CursorManager::Singleton() {
		if (mSingleton==nullptr){
			mSingleton = new CursorManager;
		}
		return mSingleton;
	}
	HCURSOR CursorManager::GetCursor(const char * cursor_name) {
		std::unordered_map<std::string, HCURSOR>::iterator iter = mCursors.find(cursor_name);
		if (iter != mCursors.end()){
			return iter->second;
		}
		return mCursors["arrow"];
	}
	void CursorManager::Init() {
		HCURSOR cursor = LoadCursor(NULL, IDC_SIZEWE);
		//<-> left right arrow
		mCursors.insert(std::pair<std::string, HCURSOR>("hslide", cursor));
		cursor = LoadCursor(NULL, IDC_SIZENS);
		//up-down direction arrow
		mCursors.insert(std::pair<std::string, HCURSOR>("vslide", cursor));
		//cross arrow
		cursor = LoadCursor(NULL, IDC_CROSS);
		mCursors.insert(std::pair<std::string, HCURSOR>("drag", cursor));
		//normal arrow
		cursor = LoadCursor(NULL, IDC_ARROW);
		mCursors.insert(std::pair<std::string, HCURSOR>("arrow", cursor));
		cursor = LoadCursor(NULL, IDC_IBEAM);
		mCursors.insert(std::pair<std::string, HCURSOR>("edit", cursor));
	}
	void CursorManager::ShowCursor(const char * cursor_name) {
		if (mbCursorLocked){
			return;
		}
		::SetCursor(GetCursor(cursor_name));
	}
	bool CursorManager::IsCursorLocked() {
		return mbCursorLocked;
	}
	void CursorManager::LockCursor() {
		mbCursorLocked = true;
	}
	void CursorManager::UnlockCursor() {
		mbCursorLocked = false;
	}
}