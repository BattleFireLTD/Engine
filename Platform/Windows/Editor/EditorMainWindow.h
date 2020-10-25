#include "Platform/Windows/Common/MainWindow.h"
#include "Platform/Windows/Common/Control/Button/ImageButton.h"
#include "Platform/Windows/Common/Control/Image/SimpleImage.h"
#include "Platform/Windows/Common/Control/Text/StaticText.h"
#include "Menu/PopupMenuButton.h"

namespace Editor {
	class EditorMainWindow :public MainWindow {
	protected:
		StaticText*mTitle;
		ImageButton*mCloseBtn;
		UINode*mLastHoverObject;
	protected:
		void OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void OnMouseLeave(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void OnKeyDown(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void OnKeyUp(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void OnChar(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void OnIMEChar(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void OnCompositionIMEString(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void OnMouseWheel(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void OnSize(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void OnCommand(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
		void InitSubWindows();
	public:
		HACCEL mAccel;
		void Init();
	};
}