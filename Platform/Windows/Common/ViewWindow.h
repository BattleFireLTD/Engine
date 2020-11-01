#pragma once
#include "Base/DoubleBufferedWindow.h"
#include "Control/UINode.h"
#include "Base/WindowContainer.h"
namespace Editor {
	class ViewWindow :public DoubleBufferedWindow {
	protected:
		bool mbDraging,mbFixedPos,mbFixedWidth,mbFixedHeight;
		Gdiplus::Rect mFixedRect;
		Gdiplus::Point mDeltaWhenDrag;
		UINode * mUIRoot, *mLastTouchObject, *mLastHoverObject;
		WindowContainer *mContainer;
	protected:
		virtual void OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnMouseLeave(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnSize(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void OnCommand(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		virtual void DrawContent(Gdiplus::Graphics&painter);
		virtual void OnClearBKG(Gdiplus::Graphics&painter);
		virtual void OnPaint(const Gdiplus::Rect &rect_need_update) {}
		virtual void OnParentPaint(Gdiplus::Graphics&painter);
	public:
		ViewWindow();
		virtual void OnParentResized(int width, int height);

		virtual void MarkDirty();
		void AddWindowAtLeft(BaseWindow*window);
		void AddWindowAtRight(BaseWindow*window);
		void AddWindowAtTop(BaseWindow*window);
		void AddWindowAtBottom(BaseWindow*window);
		void Init(BaseWindow*parent);
		void AppendUI(UINode*node);
		void FixedWindow(int x, int y,int width,int height);
		static void InitWindowClasses();
	};
}