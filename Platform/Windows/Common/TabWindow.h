#pragma once
#include "ViewWindow.h"
namespace Editor {
	class TabWindow :public ViewWindow {
		UINode*mTabUIRoot;
	protected:
		virtual void DrawContent(Gdiplus::Graphics&painter);
	public:
		TabWindow();
		void InitTab(const char * icon,const char * text);
	};
}