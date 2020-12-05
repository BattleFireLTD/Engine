#include "Platform/Windows/Common/ViewWindow.h"
namespace Editor {
	class TopMenuWindow {
	protected:
		static TopMenuWindow * mSingleton;
	protected:
		ViewWindow * mViewWindow;
		void OnSize(WPARAM wParam, LPARAM lParam, void*reserved = nullptr);
	public:
		void Init(BaseWindow*parent);
		ViewWindow*GetViewWindow();
		static TopMenuWindow * Singleton();
	};
}