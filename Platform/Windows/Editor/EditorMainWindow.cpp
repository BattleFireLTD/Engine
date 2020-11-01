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

		//view window relationship
		top_menu_window->AddWindowAtBottom(tools_window);
		tools_window->AddWindowAtTop(top_menu_window);

		hierarchy_window->AddWindowAtRight(scene_window);
		scene_window->AddWindowAtLeft(hierarchy_window);
/*
	_________________________________________________________________
	|																 |
	|						  Top Container							 |
	|________________________________________________________________|
	|												|				 |
	|												|				 |
	|												|			     |
	|												|			     |
	|					Left						|	   Right	 |
	|												|			     |
	|												|			     |
	|												|			     |
	|												|			     |
	|												|			     |
	|_______________________________________________|________________|
*/
		//level 0
		WindowContainer*main_container = new WindowContainer("MainContainer");
		//level 1
		WindowContainer*top_container = new WindowContainer("TopContainer");
		WindowContainer*bottom_container = new WindowContainer("BottomContainer");
		//level 2
		WindowContainer*left_container = new WindowContainer("BottomLeftContainer");
		WindowContainer*right_container = new WindowContainer("BottomRightContainer");
		//level 3
		WindowContainer*left_container_top = new WindowContainer("BottomLeftTopContainer");
		WindowContainer*left_container_bottom = new WindowContainer("BottomLeftBottomContainer");

		//level 0 -> level 1
		main_container->AddTopEdgeChildContainer(top_container);
		main_container->AddLeftEdgeChildContainer(top_container);
		main_container->AddLeftEdgeChildContainer(bottom_container);
		main_container->AddRightEdgeChildContainer(top_container);
		main_container->AddRightEdgeChildContainer(bottom_container);
		main_container->AddBottomEdgeChildContainer(bottom_container);

		top_container->AddSiblingContainerAtBottom(bottom_container);
		bottom_container->AddSiblingContainerAtTop(top_container);

		//level 1 -> level 2
		bottom_container->AddTopEdgeChildContainer(left_container);
		bottom_container->AddTopEdgeChildContainer(right_container);
		bottom_container->AddLeftEdgeChildContainer(left_container);
		bottom_container->AddRightEdgeChildContainer(right_container);
		bottom_container->AddBottomEdgeChildContainer(left_container);
		bottom_container->AddBottomEdgeChildContainer(right_container);
		left_container->AddSiblingContainerAtRight(right_container);
		right_container->AddSiblingContainerAtLeft(left_container);
		//level 1 top_container content
		top_container->AddChildWindowAt(kChildWindowLocationTopEdge, top_menu_window);
		top_container->AddChildWindowAt(kChildWindowLocationBottomEdge, tools_window);
		top_container->AddChildWindowAt(kChildWindowLocationLeftEdge, top_menu_window);
		top_container->AddChildWindowAt(kChildWindowLocationLeftEdge, tools_window);
		top_container->AddChildWindowAt(kChildWindowLocationRightEdge, top_menu_window);
		top_container->AddChildWindowAt(kChildWindowLocationRightEdge, tools_window);
		top_container->SetMaxRect(0, 0, -1, 70);
		top_container->Complete();
		//level 2 right_container content
		right_container->AddChildWindowAt(kChildWindowLocationLeftEdge, inspector_window);
		right_container->AddChildWindowAt(kChildWindowLocationRightEdge, inspector_window);
		right_container->AddChildWindowAt(kChildWindowLocationBottomEdge, inspector_window);
		right_container->AddChildWindowAt(kChildWindowLocationTopEdge, inspector_window);
		right_container->Complete();
		//level 2 -> level 3
		left_container->AddTopEdgeChildContainer(left_container_top);
		left_container->AddLeftEdgeChildContainer(left_container_top);
		left_container->AddLeftEdgeChildContainer(left_container_bottom);
		left_container->AddRightEdgeChildContainer(left_container_top);
		left_container->AddRightEdgeChildContainer(left_container_bottom);
		left_container->AddBottomEdgeChildContainer(left_container_bottom);
		left_container_top->AddSiblingContainerAtBottom(left_container_bottom);
		left_container_bottom->AddSiblingContainerAtTop(left_container_top);
		//level 3 left_container_bottom content
		left_container_bottom->AddChildWindowAt(kChildWindowLocationLeftEdge, project_window);
		left_container_bottom->AddChildWindowAt(kChildWindowLocationRightEdge, project_window);
		left_container_bottom->AddChildWindowAt(kChildWindowLocationBottomEdge, project_window);
		left_container_bottom->AddChildWindowAt(kChildWindowLocationTopEdge, project_window);
		left_container_bottom->Complete();
		//level 3 left_container_top content
		left_container_top->AddChildWindowAt(kChildWindowLocationLeftEdge, hierarchy_window);
		left_container_top->AddChildWindowAt(kChildWindowLocationRightEdge, scene_window);
		left_container_top->AddChildWindowAt(kChildWindowLocationTopEdge, hierarchy_window);
		left_container_top->AddChildWindowAt(kChildWindowLocationTopEdge, scene_window);
		left_container_top->AddChildWindowAt(kChildWindowLocationBottomEdge, hierarchy_window);
		left_container_top->AddChildWindowAt(kChildWindowLocationBottomEdge, scene_window);
		left_container_top->Complete();
		left_container->Complete();
		bottom_container->Complete();
		main_container->Complete();
		SetContainer(main_container);
	}
}