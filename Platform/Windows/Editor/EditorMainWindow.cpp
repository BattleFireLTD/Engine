#include "EditorMainWindow.h"
#include "Platform/Windows/Common/WinEnviroment.h"
#include "Platform/Windows/Common/ViewWindow.h"
#include "Platform/Windows/Editor/Menu/MenuItemDefine.h"
#include <dwmapi.h>
#include "Runtime/Debugger/Logger.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/IO/FileSystem.h"
#pragma comment(lib,"dwmapi.lib")
namespace Editor {
	void EditorMainWindow::DrawContent(Gdiplus::Graphics&painter){
	}
	void EditorMainWindow::OnMouseWheel(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
	}
	void EditorMainWindow::OnSize(WPARAM wParam, LPARAM lParam, void*reserved) {
		MainWindow::OnSize(wParam, lParam, reserved);
		if (mCloseBtn != nullptr)mCloseBtn->SetRect(mRect.Width - 32, 2, 24, 24);
	}
	void EditorMainWindow::OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */){
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (y<mMarginTop){
			UINode*node = mUIRoot->Intersect(x, y);
			if (node != nullptr && node == mCloseBtn) {
				node->OnTouchBegin(x,y);
				mLastTouchObject = node;
				InvalidateRect(mhWnd, nullptr, true);
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
			InvalidateRect(mhWnd, nullptr, true);
			ReleaseCapture();
		}
		else {
			if (mLastTouchObject != nullptr) {
				mLastTouchObject->OnTouchCanceled(x, y);
				mLastTouchObject = nullptr;
				InvalidateRect(mhWnd, nullptr, true);
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
				InvalidateRect(mhWnd, nullptr, true);
			}
		}
		else {
			if (mLastHoverObject!=nullptr){
				mLastHoverObject->OnTouchLeave(x, y);
				mLastHoverObject = nullptr;
				InvalidateRect(mhWnd, nullptr, true);
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
			InvalidateRect(mhWnd, nullptr, true);
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
		DWORD windowStyle = WS_OVERLAPPED | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		mhWnd = CreateWindowEx(NULL, L"MainWindow", L"BattleFire Editor", windowStyle, 0, 0, 32, 32, NULL, NULL, WinEnviroment::Instance(), NULL);
		SetWindowLongPtr(mhWnd, GWL_USERDATA, (LONG_PTR)this);
		mHDC = GetWindowDC(mhWnd);
		SetRect(0, 0, 32, 32);
		mBKGColor = Gdiplus::Color(30, 30, 30);
		mTitle = new Editor::StaticText;
		mTitle->SetAligning(Editor::AligningModeLeft);
		mTitle->SetRect(12, 6, 1286-mMarginLeft-mMarginRight, mMarginTop);
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
		InitTopMenu();
	}
	void EditorMainWindow::InitTopMenu() {
		ViewWindow*top_view_window = new ViewWindow;
		top_view_window->Init(this);
		top_view_window->SetBkgColor(Gdiplus::Color(255, 255, 255));

		PopupMenuButton*popupmenu = new (kMemEditorId)PopupMenuButton(top_view_window->GetHwnd());
		popupmenu->SetText("File");
		popupmenu->SetAligning(AligningModeMiddle);
		popupmenu->SetRect(0, 0, 50, 20);
		popupmenu->SetBkgColorRect(0, 0, 50, 20);
		top_view_window->AppendUI(popupmenu);
		popupmenu->AppendMenuOption(Menu_Option_File_NewProject, TEXT("New Project"), []()->void { Debug("new project"); });
		popupmenu->AppendMenuOption(Menu_Option_File_OpenProject, TEXT("Open Project"), []()->void { Debug("open project"); });
		popupmenu->AppendMenuOption(Menu_Option_File_NewScene, TEXT("New Scene"), []()->void { Debug("new scene"); });
		popupmenu->AppendMenuOption(Menu_Option_File_SaveCurrentScene, TEXT("Save Scene"), []()->void { Debug("save scene"); });
		popupmenu->AppendMenuOption(Menu_Option_File_Build, TEXT("Build Run"), []()->void { Debug("build run"); });
		popupmenu->AppendMenuOption(Menu_Option_File_Exit, TEXT("Exit"), []()->void { Debug("exit"); });
		popupmenu = new (kMemEditorId)PopupMenuButton(top_view_window->GetHwnd());
		popupmenu->SetText("Edit");
		popupmenu->SetAligning(AligningModeMiddle);
		popupmenu->SetRect(50, 0, 50, 20);
		popupmenu->SetBkgColorRect(50, 0, 50, 20);
		top_view_window->AppendUI(popupmenu);
		popupmenu->AppendMenuOption(Menu_Option_Edit_Duplicate, TEXT("Duplicate"), []()->void { Debug("Duplicate"); });
		popupmenu->AppendMenuOption(Menu_Option_Edit_Undo, TEXT("Undo"), []()->void { Debug("Undo"); });
		popupmenu = new (kMemEditorId)PopupMenuButton(top_view_window->GetHwnd());
		popupmenu->SetText("Component");
		popupmenu->SetAligning(AligningModeMiddle);
		popupmenu->SetRect(100, 0, 80, 20);
		popupmenu->SetBkgColorRect(100, 0, 80, 20);
		top_view_window->AppendUI(popupmenu);

		popupmenu->AppendMenuOption(Menu_Option_Component_ImageSprite, TEXT("ImageSprite"), []()->void { Debug("ImageSprite"); });
		popupmenu->AppendMenuOption(Menu_Option_Component_Camera, TEXT("Camera"), []()->void { Debug("Camera"); });
		popupmenu->AppendMenuOption(Menu_Option_Component_Label, TEXT("Label"), []()->void { Debug("Label"); });
		popupmenu->AppendMenuOption(Menu_Option_Component_Animator, TEXT("Animator"), []()->void { Debug("Animator"); });
		popupmenu->AppendMenuOption(Menu_Option_Component_ImageSprite9, TEXT("ImageSprite9"), []()->void { Debug("ImageSprite9"); });
		popupmenu->AppendMenuOption(Menu_Option_Component_Button, TEXT("Button"), []()->void { Debug("Button"); });
		popupmenu->AppendMenuOption(Menu_Option_Component_Particle, TEXT("Particle"), []()->void { Debug("Particle"); });
		popupmenu->AppendMenuOption(Menu_Option_Component_AudioSource, TEXT("AudioSource"), []()->void { Debug("AudioSource"); });
		popupmenu->AppendMenuOption(Menu_Option_Component_Physic2DComponent, TEXT("Physic2DComponent"), []()->void { Debug("Physic2DComponent"); });
		popupmenu = new (kMemEditorId)PopupMenuButton(top_view_window->GetHwnd());
		popupmenu->SetText("Window");
		popupmenu->SetAligning(AligningModeMiddle);
		popupmenu->SetRect(180, 0, 60, 20);
		popupmenu->SetBkgColorRect(180, 0, 60, 20);
		top_view_window->AppendUI(popupmenu);
		popupmenu->AppendMenuOption(Menu_Option_Window_Animation, TEXT("Animation"), []()->void { Debug("Animation"); });
		popupmenu->AppendMenuOption(Menu_Option_Window_Animator, TEXT("Animator"), []()->void { Debug("Animator"); });
		popupmenu->AppendMenuOption(Menu_Option_Window_Atlas, TEXT("Atlas"), []()->void { Debug("Atlas"); });
		popupmenu->AppendMenuOption(Menu_Option_Window_Profiler, TEXT("Profiler"), []()->void { Debug("Profiler"); });
		popupmenu->AppendMenuOption(Menu_Option_Window_Physics, TEXT("Physics"), []()->void { Debug("Physics"); });
		top_view_window->MoveWindow(0, 0, 1280, 20);
		top_view_window->Show();
	}
}