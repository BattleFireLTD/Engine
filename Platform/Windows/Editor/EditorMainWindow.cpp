#include "EditorMainWindow.h"
#include "Platform/Windows/Common/WinEnviroment.h"
#include "Platform/Windows/Common/ViewWindow.h"
#include "Platform/Windows/Common/Control/Button/TwoStateButton.h"
#include "Platform/Windows/Editor/Menu/MenuItemDefine.h"
#include <dwmapi.h>
#include "Runtime/Debugger/Logger.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/IO/FileSystem.h"
#include "TopMenuWindow/TopMenuWindow.h"
#include "TopToolsWindow/TopToolsWindow.h"
#include "HierarchyWindow/HierarchyWindow.h"
#include "ProjectWindow/ProjectWindow.h"
#include "SceneWindow/SceneWindow.h"
#include "InspectorWindow/InspectorWindow.h"
#pragma comment(lib,"dwmapi.lib")
namespace Editor {
	void EditorMainWindow::OnMouseWheel(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
	}
	void EditorMainWindow::OnSize(WPARAM wParam, LPARAM lParam, void*reserved) {
		MainWindow::OnSize(wParam, lParam, reserved);
		if (mCloseBtn != nullptr)mCloseBtn->SetRect(mRect.Width - 32, 2, 24, 24);
	}
	void EditorMainWindow::OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */){
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (y<mTopNCSize){
			UINode*node = mUIRoot->Intersect(x, y);
			if (node != nullptr && node == mCloseBtn) {
				node->OnTouchBegin(x,y);
				mLastTouchObject = node;
				//Debug("EditorMainWindow::OnLButtonDown OnTouchBegin InvalidateRect");
				InvalidateRect(mhWnd, nullptr, false);
				SetCapture(mhWnd);
			}
			else {
				MainWindow::OnLButtonDown(wParam, lParam, reserved);
			}
		}
	}
	void EditorMainWindow::OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		UINode*node = mUIRoot->Intersect(x, y);
		if (node != nullptr&&node==mLastTouchObject) {
			node->OnTouchEnd(x, y);
			//Debug("EditorMainWindow::OnLButtonUp OnTouchEnd InvalidateRect");
			InvalidateRect(mhWnd, nullptr, false);
			ReleaseCapture();
		}
		else {
			if (mLastTouchObject != nullptr) {
				mLastTouchObject->OnTouchCanceled(x, y);
				mLastTouchObject = nullptr;
				//Debug("EditorMainWindow::OnLButtonUp OnTouchCanceled InvalidateRect");
				InvalidateRect(mhWnd, nullptr, false);
				ReleaseCapture();
			}
			else {
				MainWindow::OnLButtonUp(wParam, lParam, reserved);
			}
		}
	}
	void EditorMainWindow::OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		UINode*node = mUIRoot->Intersect(x, y);
		if (node!=nullptr){
			if (mLastHoverObject !=node){
				if (mLastHoverObject != nullptr) {
					mLastHoverObject->OnTouchLeave(x, y);
				}
				mLastHoverObject = node;
				mLastHoverObject->OnTouchEnter(x, y);
				//Debug("EditorMainWindow::OnMouseMove mLastHoverObject->OnTouchEnter");
				RECT rect = {
					0,
					0,
					mRect.Width,
					mTopNCSize
				};
				//InvalidateRect(mhWnd, &rect, false);
				InvalidateRect(mhWnd, nullptr, false);
			}
		}
		else {
			if (mLastHoverObject!=nullptr){
				mLastHoverObject->OnTouchLeave(x, y);
				mLastHoverObject = nullptr;
				//Debug("EditorMainWindow::OnMouseMove mLastHoverObject->OnTouchLeave");
				InvalidateRect(mhWnd, nullptr, false);
			}
		}
		MainWindow::OnMouseMove(wParam, lParam, reserved);
	}
	void EditorMainWindow::OnMouseLeave(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		if (mLastHoverObject != nullptr) {
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			mLastHoverObject->OnTouchLeave(x, y);
			mLastHoverObject = nullptr;
			//Debug("EditorMainWindow::OnMouseLeave OnMouseLeave InvalidateRect 2");
			InvalidateRect(mhWnd, nullptr, false);
		}
		MainWindow::OnMouseLeave(wParam, lParam, reserved);
	}
	void EditorMainWindow::OnKeyDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
	}
	void EditorMainWindow::OnKeyUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
	}
	void EditorMainWindow::OnChar(WPARAM wParam, LPARAM lParam, void*reserved){
	}
	void EditorMainWindow::OnIMEChar(WPARAM wParam, LPARAM lParam, void*reserved){
		WCHAR temp[2] = { 0 };
		temp[0] = wParam;
		char szBuffer[8] = { 0 };
		Alice::StringUtils::UnicodeToUTF8(temp, szBuffer);
	}
	void EditorMainWindow::OnCompositionIMEString(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		HIMC himc = ImmGetContext(mhWnd);
		if (himc) {
			ImmReleaseContext(mhWnd, himc);
		}
	}
	void EditorMainWindow::OnCommand(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		UINT nNotify = HIWORD(wParam);
		UINT commandID = LOWORD(wParam);
		switch (commandID){
		case kAccelKeyCombinationCTRL_C:
			break;
		case kAccelKeyCombinationCTRL_V:
			break;
		default:
			Debug("unprocessed command %d",commandID);
			break;
		}
	}
	void EditorMainWindow::Init() {
		SetWindowName("EditorMainWindow");
		DWORD windowStyle = WS_OVERLAPPED;// | WS_CLIPCHILDREN;
		mhWnd = CreateWindowEx(NULL, L"MainWindow", L"BattleFire Editor", windowStyle, 0, 0, 32, 32, NULL, NULL, WinEnviroment::Instance(), NULL);
		SetWindowLongPtr(mhWnd, GWL_USERDATA, (LONG_PTR)this);
		mHDC = GetWindowDC(mhWnd);
		SetRect(0, 0, 32, 32);
		mBKGColor = Gdiplus::Color(30, 30, 30);
		mTitle = new Editor::StaticText;
		mTitle->SetAligning(Editor::AligningModeLeft);
		mTitle->SetRect(12, 3, 1286-mLeftNCSize-mRightNCSize, mTopNCSize);
		mTitle->SetText("Engine - None");
		mTitle->SetTextColor(0, 150, 200);
		mUIRoot = mTitle;
		mCloseBtn = new Editor::ImageButton;
		mCloseBtn->SetImageData(WinResources::Singleton()->GetImageData("close_btn.png"));
		mCloseBtn->SetRect(1248, 2, 24, 24);
		mCloseBtn->SetOnClickedHandler([](void*)->void {
			Debug("quit");
			PostQuitMessage(0);
		});
		mUIRoot->AppendChild(mCloseBtn);
		mAccel = WinEnviroment::InitAccel();
		mLastHoverObject = nullptr;
		TopMenuWindow::Singleton()->Init(this);
		TopToolsWindow::Singleton()->Init(this);
		HierarchyWindow::Singleton()->Init(this);
		ProjectWindow::Singleton()->Init(this);
		SceneWindow::Singleton()->Init(this);
		InspectorWindow::Singleton()->Init(this);
		InitSubWindows();
	}
	void EditorMainWindow::InitSubWindows() {
		ViewWindow*top_menu_window = TopMenuWindow::Singleton()->GetViewWindow();
		ViewWindow*tools_window = TopToolsWindow::Singleton()->GetViewWindow();
		ViewWindow*hierarchy_window = HierarchyWindow::Singleton()->GetViewWindow();
		ViewWindow*project_window = ProjectWindow::Singleton()->GetViewWindow();
		ViewWindow*scene_window = SceneWindow::Singleton()->GetViewWindow();
		ViewWindow*inspector_window = InspectorWindow::Singleton()->GetViewWindow();

		//sub view window location
		AddChildWindowAt(kChildWindowLocationTopEdge, top_menu_window);

		AddChildWindowAt(kChildWindowLocationLeftEdge, top_menu_window);
		AddChildWindowAt(kChildWindowLocationLeftEdge, tools_window);
		AddChildWindowAt(kChildWindowLocationLeftEdge, hierarchy_window);
		AddChildWindowAt(kChildWindowLocationLeftEdge, project_window);

		AddChildWindowAt(kChildWindowLocationRightEdge, top_menu_window);
		AddChildWindowAt(kChildWindowLocationRightEdge, tools_window);
		AddChildWindowAt(kChildWindowLocationRightEdge, inspector_window);

		AddChildWindowAt(kChildWindowLocationBottomEdge, project_window);
		AddChildWindowAt(kChildWindowLocationBottomEdge, inspector_window);
		
		//view window relationship
		top_menu_window->AddWindowAtBottom(tools_window);
		tools_window->AddWindowAtTop(top_menu_window);

		tools_window->AddWindowAtBottom(hierarchy_window);
		tools_window->AddWindowAtBottom(scene_window);
		tools_window->AddWindowAtBottom(inspector_window);
		hierarchy_window->AddWindowAtBottom(tools_window);
		scene_window->AddWindowAtBottom(tools_window);
		inspector_window->AddWindowAtBottom(tools_window);

		hierarchy_window->AddWindowAtBottom(project_window);
		project_window->AddWindowAtTop(hierarchy_window);

		hierarchy_window->AddWindowAtRight(scene_window);
		scene_window->AddWindowAtLeft(hierarchy_window);

		project_window->AddWindowAtTop(scene_window);
		scene_window->AddWindowAtBottom(project_window);

		project_window->AddWindowAtRight(inspector_window);
		inspector_window->AddWindowAtLeft(project_window);

		scene_window->AddWindowAtRight(inspector_window);
		inspector_window->AddWindowAtLeft(scene_window);
	}
}