#include "EditorMainWindow.h"
#include "Platform/Windows/Common/WinEnviroment.h"
#include <dwmapi.h>
#include "Runtime/Debugger/Logger.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/IO/FileSystem.h"
#pragma comment(lib,"dwmapi.lib")
namespace Editor {
	void EditorMainWindow::DrawContent(Gdiplus::Graphics&painter){
		if (mUIRoot!=nullptr){
			mUIRoot->DrawRecursively(painter);
		}
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
				InvalidateRect(mhWnd, NULL, true);
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
		if (mLastTouchObject!=nullptr){
			mLastTouchObject->OnTouchEnd(x, y);
			mLastTouchObject = nullptr;
			InvalidateRect(mhWnd, nullptr, true);
			ReleaseCapture();
		}
		else {
			MainWindow::OnLButtonUp(wParam, lParam, reserved);
		}
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
			break;
		}
	}
	void EditorMainWindow::Init() {
		DWORD windowStyle = WS_OVERLAPPED | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		mhWnd = CreateWindowEx(NULL, L"MainWindow", L"BattleFire Editor", windowStyle, 0, 0, 32, 32, NULL, NULL, WinEnviroment::Instance(), NULL);
		SetWindowLongPtr(mhWnd, GWL_USERDATA, (LONG_PTR)this);
		mHDC = GetWindowDC(mhWnd);
		SetRect(0, 0, 32, 32);
		mBKGColor = Gdiplus::Color(0, 150, 200);
		mTitle = new Editor::StaticText;
		mTitle->SetAligning(Editor::AligningModeMiddle);
		mTitle->SetRect(0, mMarginLeft, 1286-mMarginLeft-mMarginRight, mMarginTop);
		mTitle->SetText("BattleFire Editor");
		mUIRoot = mTitle;
		mCloseBtn = nullptr;
		if (Alice::FileSystem::Exists("Resource/close_btn.png")) {
			mCloseBtn = new Editor::ImageButton;
			mCloseBtn->SetImage("Resource/close_btn.png");
			mCloseBtn->SetRect(1248, 2, 24, 24);
			mCloseBtn->SetOnClickedHandler([](void*)->void{
				Debug("quit");
				PostQuitMessage(0);
			});
			mUIRoot->AppendChild(mCloseBtn);
		}
		mAccel = WinEnviroment::InitAccel();
	}
}