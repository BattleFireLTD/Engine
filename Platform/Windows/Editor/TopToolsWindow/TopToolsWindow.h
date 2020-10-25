#include "Platform/Windows/Common/ViewWindow.h"
namespace Editor {
	class TopToolsWindow {
	protected:
		static TopToolsWindow * mSingleton;
	protected:
		ViewWindow * mViewWindow;
	public:
		void Init(BaseWindow*parent);
		ViewWindow*GetViewWindow();
		static TopToolsWindow * Singleton();
	};
}