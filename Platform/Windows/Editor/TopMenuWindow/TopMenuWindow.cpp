#include "TopMenuWindow.h"
#include "Platform/Windows/Common/WinEnviroment.h"
#include "Platform/Windows/Common/ViewWindow.h"
#include "Platform/Windows/Common/Control/Button/TwoStateButton.h"
#include "Platform/Windows/Editor/Menu/MenuItemDefine.h"
#include "Runtime/Debugger/Logger.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/IO/FileSystem.h"
#include "Platform/Windows/Editor/Menu/PopupMenuButton.h"
namespace Editor {
	TopMenuWindow*TopMenuWindow::mSingleton = nullptr;
	void TopMenuWindow::Init(BaseWindow*parent) {
		ViewWindow*view_window = new ViewWindow;
		view_window->SetWindowName("TopMenuWindow");
		view_window->SetNCSize(0, 0, 0, 0);
		view_window->SetMinRect(0, 0, -1, 20);
		view_window->SetMaxRect(0, 0, -1, 20);
		view_window->Init(parent);
		view_window->SetBkgColor(Gdiplus::Color(255, 255, 255));

		PopupMenuButton*popupmenu = new (kMemEditorId)PopupMenuButton(view_window->GetHwnd());
		popupmenu->SetText("File");
		popupmenu->SetAligning(AligningModeMiddle);
		popupmenu->SetRect(0, 0, 50, 20);
		popupmenu->SetBkgColorRect(0, 0, 50, 20);
		view_window->AppendUI(popupmenu);
		popupmenu->AppendMenuOption(Menu_Option_File_NewProject, TEXT("New Project"), []()->void { Debug("new project"); });
		popupmenu->AppendMenuOption(Menu_Option_File_OpenProject, TEXT("Open Project"), []()->void { Debug("open project"); });
		popupmenu->AppendMenuOption(Menu_Option_File_NewScene, TEXT("New Scene"), []()->void { Debug("new scene"); });
		popupmenu->AppendMenuOption(Menu_Option_File_SaveCurrentScene, TEXT("Save Scene"), []()->void { Debug("save scene"); });
		popupmenu->AppendMenuOption(Menu_Option_File_Build, TEXT("Build Run"), []()->void { Debug("build run"); });
		popupmenu->AppendMenuOption(Menu_Option_File_Exit, TEXT("Exit"), []()->void { Debug("exit"); });
		popupmenu = new (kMemEditorId)PopupMenuButton(view_window->GetHwnd());
		popupmenu->SetText("Edit");
		popupmenu->SetAligning(AligningModeMiddle);
		popupmenu->SetRect(50, 0, 50, 20);
		popupmenu->SetBkgColorRect(50, 0, 50, 20);
		view_window->AppendUI(popupmenu);
		popupmenu->AppendMenuOption(Menu_Option_Edit_Duplicate, TEXT("Duplicate"), []()->void { Debug("Duplicate"); });
		popupmenu->AppendMenuOption(Menu_Option_Edit_Undo, TEXT("Undo"), []()->void { Debug("Undo"); });
		popupmenu = new (kMemEditorId)PopupMenuButton(view_window->GetHwnd());
		popupmenu->SetText("Component");
		popupmenu->SetAligning(AligningModeMiddle);
		popupmenu->SetRect(100, 0, 80, 20);
		popupmenu->SetBkgColorRect(100, 0, 80, 20);
		view_window->AppendUI(popupmenu);

		popupmenu->AppendMenuOption(Menu_Option_Component_ImageSprite, TEXT("ImageSprite"), []()->void { Debug("ImageSprite"); });
		popupmenu->AppendMenuOption(Menu_Option_Component_Camera, TEXT("Camera"), []()->void { Debug("Camera"); });
		popupmenu->AppendMenuOption(Menu_Option_Component_Label, TEXT("Label"), []()->void { Debug("Label"); });
		popupmenu->AppendMenuOption(Menu_Option_Component_Animator, TEXT("Animator"), []()->void { Debug("Animator"); });
		popupmenu->AppendMenuOption(Menu_Option_Component_ImageSprite9, TEXT("ImageSprite9"), []()->void { Debug("ImageSprite9"); });
		popupmenu->AppendMenuOption(Menu_Option_Component_Button, TEXT("Button"), []()->void { Debug("Button"); });
		popupmenu->AppendMenuOption(Menu_Option_Component_Particle, TEXT("Particle"), []()->void { Debug("Particle"); });
		popupmenu->AppendMenuOption(Menu_Option_Component_AudioSource, TEXT("AudioSource"), []()->void { Debug("AudioSource"); });
		popupmenu->AppendMenuOption(Menu_Option_Component_Physic2DComponent, TEXT("Physic2DComponent"), []()->void { Debug("Physic2DComponent"); });
		popupmenu = new (kMemEditorId)PopupMenuButton(view_window->GetHwnd());
		popupmenu->SetText("Window");
		popupmenu->SetAligning(AligningModeMiddle);
		popupmenu->SetRect(180, 0, 60, 20);
		popupmenu->SetBkgColorRect(180, 0, 60, 20);
		view_window->AppendUI(popupmenu);
		popupmenu->AppendMenuOption(Menu_Option_Window_Animation, TEXT("Animation"), []()->void { Debug("Animation"); });
		popupmenu->AppendMenuOption(Menu_Option_Window_Animator, TEXT("Animator"), []()->void { Debug("Animator"); });
		popupmenu->AppendMenuOption(Menu_Option_Window_Atlas, TEXT("Atlas"), []()->void { Debug("Atlas"); });
		popupmenu->AppendMenuOption(Menu_Option_Window_Profiler, TEXT("Profiler"), []()->void { Debug("Profiler"); });
		popupmenu->AppendMenuOption(Menu_Option_Window_Physics, TEXT("Physics"), []()->void { Debug("Physics"); });

		view_window->MoveWindow(0, 0, 1274, 20);
		view_window->Show();
		mViewWindow = view_window;
	}
	ViewWindow*TopMenuWindow::GetViewWindow() {
		return mViewWindow;
	}
	TopMenuWindow*TopMenuWindow::Singleton() {
		if (mSingleton == nullptr) {
			mSingleton = new TopMenuWindow;
		}
		return mSingleton;
	}
}