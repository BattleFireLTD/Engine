#pragma once
#include "Base/DoubleBufferedWindow.h"
#include "Control/UINode.h"
namespace Editor {
	class ViewWindow :public DoubleBufferedWindow {
	protected:
		bool mbDraging;
		Gdiplus::Point mDeltaWhenDrag;
		UINode * mUIRoot,*mLastTouchObject,*mLastHoverObject;
	protected:
		virtual void OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMouseLeave(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnSize(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnCommand(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void DrawContent(Gdiplus::Graphics&painter);
	public:
		ViewWindow();
		virtual void OnParentResized(int width, int height);
		void Init(BaseWindow*parent);
		void AppendUI(UINode*node);
		static void InitWindowClasses();
	};
}