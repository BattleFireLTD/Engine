#include "Platform/Windows/Common/ViewWindow.h"
namespace Editor {
	class ProjectWindow {
	protected:
		static ProjectWindow * mSingleton;
	protected:
		ViewWindow * mViewWindow;
	public:
		void Init(BaseWindow*parent);
		ViewWindow*GetViewWindow();
		static ProjectWindow * Singleton();
	};
}