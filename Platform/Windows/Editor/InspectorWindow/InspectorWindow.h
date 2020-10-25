#include "Platform/Windows/Common/ViewWindow.h"
namespace Editor {
	class InspectorWindow {
	protected:
		static InspectorWindow * mSingleton;
	protected:
		ViewWindow * mViewWindow;
	public:
		void Init(BaseWindow*parent);
		ViewWindow*GetViewWindow();
		static InspectorWindow * Singleton();
	};
}