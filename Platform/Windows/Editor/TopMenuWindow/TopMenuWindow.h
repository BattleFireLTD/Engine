#include "Platform/Windows/Common/ViewWindow.h"
namespace Editor {
	class TopMenuWindow {
	protected:
		static TopMenuWindow * mSingleton;
	protected:
		ViewWindow * mViewWindow;
	public:
		void Init(BaseWindow*parent);
		ViewWindow*GetViewWindow();
		static TopMenuWindow * Singleton();
	};
}