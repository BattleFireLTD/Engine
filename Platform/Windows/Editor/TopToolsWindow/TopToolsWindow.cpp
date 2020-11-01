#include "TopToolsWindow.h"
#include "Platform/Windows/Common/WinEnviroment.h"
#include "Platform/Windows/Common/ViewWindow.h"
#include "Platform/Windows/Common/Control/Button/TwoStateButton.h"
#include "Platform/Windows/Editor/Menu/MenuItemDefine.h"
#include "Runtime/Debugger/Logger.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/IO/FileSystem.h"
namespace Editor {
	TopToolsWindow*TopToolsWindow::mSingleton = nullptr;
	void TopToolsWindow::Init(BaseWindow*parent) {
		ViewWindow*view_window = new ViewWindow;
		view_window->SetWindowName("ToolsWindow");
		view_window->SetBkgColor(Gdiplus::Color(30, 30, 30));
		view_window->SetNCSize(0, 0, 0, 0);
		view_window->Init(parent);
		view_window->MoveWindow(0, 20, 1274, 50);
		view_window->SetMinRect(0, 0, -1, 50);
		view_window->SetMaxRect(0, 0, -1, 50);
		view_window->Show();
		TwoStateButton*play_stop_button = new TwoStateButton;
		play_stop_button->SetImageData(WinResources::Singleton()->GetImageData("PlayNormal.png"), WinResources::Singleton()->GetImageData("PlayDown.png"));
		play_stop_button->SetRect(view_window->GetUILocationL(32, 1274 / 2) - 16, view_window->GetUILocationT(32, 9) - 16, 0, 0);
		play_stop_button->SetLocationCatagory(UINode::kUILocationCatagoryPercentageAbsolute, UINode::kUILocationCatagoryRelativeToTop);
		play_stop_button->SetAnchor(0.5f, 9.0f);
		play_stop_button->SetOnClickedHandler([](void*ptr)->void {
			TwoStateButton * btn = (TwoStateButton*)ptr;
			switch (btn->GetState()) {
			case TwoStateButton::kTwoStateButtonStateOne:
				Debug("play stop button state : stopped");
				break;
			case TwoStateButton::kTwoStateButtonStateTwo:
				Debug("play stop button state : playing");
				break;
			}
		});
		view_window->AppendUI(play_stop_button);
		mViewWindow = view_window;
	}
	ViewWindow*TopToolsWindow::GetViewWindow() {
		return mViewWindow;
	}
	TopToolsWindow*TopToolsWindow::Singleton() {
		if (mSingleton==nullptr){
			mSingleton = new TopToolsWindow;
		}
		return mSingleton;
	}
}