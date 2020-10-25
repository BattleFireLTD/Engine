#include "InspectorWindow.h"
#include "Platform/Windows/Common/WinEnviroment.h"
#include "Platform/Windows/Common/ViewWindow.h"
#include "Platform/Windows/Common/Control/Button/TwoStateButton.h"
#include "Platform/Windows/Editor/Menu/MenuItemDefine.h"
#include "Runtime/Debugger/Logger.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/IO/FileSystem.h"
namespace Editor {
	InspectorWindow*InspectorWindow::mSingleton = nullptr;
	void InspectorWindow::Init(BaseWindow*parent) {
		ViewWindow*view_window = new ViewWindow;
		view_window->SetWindowName("InspectorWindow");
		view_window->Init(parent);
		view_window->SetBkgColor(Gdiplus::Color(0, 255, 255));
		view_window->MoveWindow(974, 70, 300, 650);
		view_window->Show();
		mViewWindow = view_window;
	}
	ViewWindow*InspectorWindow::GetViewWindow() {
		return mViewWindow;
	}
	InspectorWindow*InspectorWindow::Singleton() {
		if (mSingleton == nullptr) {
			mSingleton = new InspectorWindow;
		}
		return mSingleton;
	}
}