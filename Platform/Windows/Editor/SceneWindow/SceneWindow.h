#include "Platform/Windows/Common/ViewWindow.h"
namespace Editor {
	class SceneWindow {
	protected:
		static SceneWindow * mSingleton;
	protected:
		ViewWindow * mViewWindow;
	public:
		void Init(BaseWindow*parent);
		ViewWindow*GetViewWindow();
		static SceneWindow * Singleton();
	};
}