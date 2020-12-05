#include "TabWindow.h"
#include "Platform/Windows/Common/Control/Button/TabButton.h"
#include "Runtime/Debugger/Logger.h"
namespace Editor {
	TabWindow::TabWindow() {
		mTabUIRoot = nullptr;
	}
	void TabWindow::DrawContent(Gdiplus::Graphics&painter) {
		mTabUIRoot->DrawRecursively(painter);
		Gdiplus::Pen p(Gdiplus::Color(30,30,30));
		painter.DrawLine(&p, Gdiplus::Point(0,20), Gdiplus::Point(mRect.Width,20));
	}
	void TabWindow::InitTab(const char * icon, const char * text) {
		TabButton *tab_button = new TabButton;
		tab_button->Init(WinResources::Singleton()->GetImageData("AliceTab.png"), WinResources::Singleton()->GetImageData(icon), text);
		tab_button->SetRect(GetUILocationL(96, 0) - 48, GetUILocationT(20, 0) - 10, 96, 20);
		mTabUIRoot = tab_button;
	}
}