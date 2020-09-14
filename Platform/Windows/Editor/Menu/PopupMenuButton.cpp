#include "PopupMenuButton.h"
#include "Runtime/Debugger/Logger.h"
namespace Editor{
	PopupMenuButton::PopupMenuButton(HWND hWnd){
		mParentWnd = hWnd;
		SetTextColor(0, 0, 0);
		SetBkgColor(200, 200, 200);
		mMenu = nullptr;
	}
	HMENU PopupMenuButton::GetMenuHandle() {
		if (mMenu==nullptr){
			mMenu = CreatePopupMenu();
		}
		return mMenu;
	}
	void PopupMenuButton::AppendMenuOption(int commandID, LPCWSTR menu_name, VOID_NO_ARG foo) {
		AppendMenu(GetMenuHandle(), MF_STRING, commandID, menu_name);
		AddEventProcesser(commandID, foo);
	}
	void PopupMenuButton::AddEventProcesser(int nCommandID, VOID_NO_ARG foo){
		auto iter = mMenuEventProcessers.find(nCommandID);
		if (iter == mMenuEventProcessers.end()){
			mMenuEventProcessers.insert(std::pair<int, VOID_NO_ARG>(nCommandID, foo));
		}
	}
	void PopupMenuButton::ProcessEvent(int nCommandID){
		auto iter = mMenuEventProcessers.find(nCommandID);
		if (iter != mMenuEventProcessers.end()){
			iter->second();
			return;
		}
		if (mRightSibling != nullptr){
			RightSibling<PopupMenuButton>()->ProcessEvent(nCommandID);
		}
	}
	void PopupMenuButton::OnTouchBegin(int x, int y, int touch_id/* =0 */) {
		
	}
	void PopupMenuButton::OnTouchEnd(int x, int y, int touch_id /* = 0 */) {
		OnClicked(x, y, touch_id);
	}
	void PopupMenuButton::OnTouchCanceled(int x, int y, int touch_id /* = 0 */) {
		
	}
	void PopupMenuButton::OnTouchMove(int x, int y, int touch_id /* = 0 */) {
		
	}
	void PopupMenuButton::OnClicked(int x, int y, int touch_id /* = 0 */) {
		POINT point;
		point.x = mRect.X;
		point.y = mRect.Y+mRect.Height;
		ClientToScreen(mParentWnd, &point);
		TrackPopupMenu(mMenu, TPM_LEFTALIGN, point.x, point.y, 0, mParentWnd, NULL);
	}
	void PopupMenuButton::OnTouchEnter(int x, int y, int touch_id /* = 0 */) {
		ShowBkgColor(true);
	}
	void PopupMenuButton::OnTouchLeave(int x, int y, int touch_id /* = 0 */) {
		ShowBkgColor(false);
	}
}