#include "Platform/Windows/Common/ViewWindow.h"
namespace Editor {
	class TwoStateButton;
	class TopToolsWindow {
	protected:
		static TopToolsWindow * mSingleton;
	protected:
		ViewWindow * mViewWindow;
		TwoStateButton*mPlayStopButton;
	public:
		void Init(BaseWindow*parent);
		ViewWindow*GetViewWindow();
		static TopToolsWindow * Singleton();
	};
}