#include "Platform/Windows/Common/ViewWindow.h"
namespace Editor {
	class HierarchyWindow {
	protected:
		static HierarchyWindow * mSingleton;
	protected:
		ViewWindow * mViewWindow;
	public:
		void Init(BaseWindow*parent);
		ViewWindow*GetViewWindow();
		static HierarchyWindow * Singleton();
	};
}