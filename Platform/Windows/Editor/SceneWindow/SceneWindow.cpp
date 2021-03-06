#include "SceneWindow.h"
#include "Platform/Windows/Common/WinEnviroment.h"
#include "Platform/Windows/Common/TabWindow.h"
#include "Platform/Windows/Editor/Menu/MenuItemDefine.h"
#include "Runtime/Debugger/Logger.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/IO/FileSystem.h"
namespace Editor {
	SceneWindow*SceneWindow::mSingleton = nullptr;
	void SceneWindow::Init(BaseWindow*parent) {
		TabWindow*view_window = new TabWindow;
		view_window->SetWindowName("SceneWindow");
		view_window->SetMinRect(0, 0, 400, 320);
		view_window->SetNCSize(3, 3, 0, 0);
		view_window->Init(parent);
		//view_window->SetBkgColor(Gdiplus::Color(41, 77, 121));
		view_window->MoveWindow(300, 70, 674, 400);
		view_window->Show();
		view_window->InitTab("SceneViewIcon.png", "Scene");
		mViewWindow = view_window;
	}
	ViewWindow*SceneWindow::GetViewWindow() {
		return mViewWindow;
	}
	SceneWindow*SceneWindow::Singleton() {
		if (mSingleton == nullptr) {
			mSingleton = new SceneWindow;
		}
		return mSingleton;
	}
}