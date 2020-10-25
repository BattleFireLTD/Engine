#include "Platform/Windows/Common/WinEnviroment.h"
#include "Platform/Windows/Common/WinResources.h"
#include "Platform/Windows/Common/ViewWindow.h"
#include "Platform/Windows/Editor/EditorMainWindow.h"
#include "Platform/Windows/Common/CursorManager.h"
#include "Runtime/IO/ResourceManager.h"
#if _DEBUG
#pragma comment( linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif
Editor::EditorMainWindow * gEditorMainWindow = nullptr;
void ProcessingMessage(MSG&msg) {
	if (!TranslateAccelerator(msg.hwnd, gEditorMainWindow->mAccel, &msg)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
void MessageLoop() {
	MSG msg;
	bool isQuit = false;
	std::vector<MSG> msgs;
	int nIndex = 0;
	int nMsgCount = 0;
	while (!isQuit) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP) {
				ProcessingMessage(msg);
			}
			else {
				msgs.push_back(msg);
				if (msgs.size() == 100) {
					break;
				}
			}
		}
		nIndex = 0;
		nMsgCount = (int)msgs.size();
		while (nIndex < nMsgCount) {
			if (WM_QUIT == msgs[nIndex].message){
				isQuit = true;
			}else{
				ProcessingMessage(msgs[nIndex]);
			}
			nIndex++;
		}
		msgs.clear();
		Sleep(10);
	}
}
INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Editor::WinEnviroment::Init(hInstance);
	Alice::ResourceManager::Init();
	Editor::CursorManager::Singleton()->Init();
	Editor::WinResources::Singleton()->Init("res");
	Editor::MainWindow::InitWindowClasses();
	Editor::ViewWindow::InitWindowClasses();
	gEditorMainWindow = new Editor::EditorMainWindow;
	gEditorMainWindow->Init();
	gEditorMainWindow->MoveWindow(0, 0, 1286, 752);
	gEditorMainWindow->Show();
	MessageLoop();
	return 0;
}