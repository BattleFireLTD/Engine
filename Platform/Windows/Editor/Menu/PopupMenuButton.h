#pragma once
#include "Platform/Windows/Common/Control/Text/StaticText.h"
namespace Editor{
	class PopupMenuButton :public StaticText{
	protected:
		HMENU mMenu;
		HWND mParentWnd;
		Gdiplus::Point mOriginalPos;
		std::unordered_map<int, VOID_NO_ARG> mMenuEventProcessers;
	public:
		PopupMenuButton(HWND hWnd);
		void AddEventProcesser(int nCommandID, VOID_NO_ARG foo);
		void ProcessEvent(int nCommandID);
		HMENU GetMenuHandle();
		void AppendMenuOption(int commandID, LPCWSTR menu_name, VOID_NO_ARG foo);
		virtual void OnTouchBegin(int x, int y, int touch_id = 0);
		virtual void OnTouchEnd(int x, int y, int touch_id = 0);
		virtual void OnTouchMove(int x, int y, int touch_id = 0);
		virtual void OnTouchCanceled(int x, int y, int touch_id = 0);
		virtual void OnClicked(int x, int y, int touch_id = 0);
		virtual void OnTouchEnter(int x, int y, int touch_id = 0);
		virtual void OnTouchLeave(int x, int y, int touch_id = 0);
	};
}