#include "Window.h"
#include "Runtime/Debugger/Logger.h"
#pragma comment(lib,"dwmapi.lib")
#pragma warning(disable:4244)
namespace Editor
{
	std::unordered_set<BaseWindow*> BaseWindow::mScheduledWindows;
	BaseWindow::BaseWindow() : mParent(nullptr){
		SetRect(0, 0, 200, 200);
		SetMinRect(0, 0, -1, -1);
		mBKGColor = Gdiplus::Color(41,41,41);
		memset(mName,0,64);
	}

	BaseWindow::~BaseWindow(){
		DestroyWindow(mhWnd);
	}


	void BaseWindow::SetMinRect(int x, int y, int width, int height)
	{
		mMinRect.X = x;
		mMinRect.Y = y;
		mMinRect.Width = width;
		mMinRect.Height = height;
	}

	Gdiplus::Rect & BaseWindow::GetMinRect()
	{
		return mMinRect;
	}

	int BaseWindow::GetMinWidth()
	{
		return mMinRect.Width;
	}

	int BaseWindow::GetMinHeight()
	{
		return mMinRect.Height;
	}

	static BaseWindow*sRootWindow=nullptr;

	void BaseWindow::ScheduleUpdate()
	{
		if (sRootWindow!=this)
		{
			sRootWindow->PushBack(this);
		}
	}

	void BaseWindow::CancelUpdate()
	{
		sRootWindow->Remove(this);
	}

	void BaseWindow::SetRootWindow(BaseWindow*window)
	{
		sRootWindow = window;
	}

	void BaseWindow::FixedUpdate()
	{
		sRootWindow->PaintWindow();
	}


	void BaseWindow::DrawContent(Gdiplus::Graphics&painter)
	{

	}

	void BaseWindow::OnEndPaint()
	{

	}

	void BaseWindow::OnPaint()
	{
		Gdiplus::Graphics painter(mHDC);
		painter.Clear(mBKGColor);
		DrawContent(painter);
		OnEndPaint();
	}

	void BaseWindow::OnSize(WPARAM wParam, LPARAM lParam, void*reserved)
	{
		mRect.Width = LOWORD(lParam);
		mRect.Height = HIWORD(lParam);
		HWND cWnd = GetWindow(mhWnd, GW_CHILD);
		while (cWnd != nullptr) {
			BaseWindow*vw = WindowInstance<BaseWindow>(cWnd);
			vw->OnParentResized(mRect.Width, mRect.Height);
			cWnd = GetNextWindow(cWnd, GW_HWNDNEXT);
		}
	}

	void BaseWindow::OnMoving(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		RECT * rect = (RECT*)lParam;
		mRect.X = rect->left;
		mRect.Y = rect->top;
	}

	void BaseWindow::OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		if (mMinRect.Width!=-1)
		{
			MINMAXINFO *ptr = (MINMAXINFO*)lParam;
			ptr->ptMinTrackSize.x = mMinRect.Width;
			ptr->ptMinTrackSize.y = mMinRect.Height;
		}
	}

	void BaseWindow::OnWindowMove(BaseWindow*movedWindow)
	{

	}

	void BaseWindow::OnEditControlLoseFocus(BaseWindow*editControl)
	{
	}

	LRESULT BaseWindow::OnNCACTIVATE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */) {
		OnPaint();
		return 0;
	}


	void BaseWindow::Show(){
		if (!IsWindowVisible(mhWnd)) {
			OnShow();
			ShowWindow(mhWnd, SW_SHOW);
			UpdateWindow(mhWnd);
		}
	}

	void BaseWindow::OnShow(){
	}

	void BaseWindow::Hide() {
		if (IsWindowVisible(mhWnd)) {
			OnShow();
			ShowWindow(mhWnd, SW_HIDE);
			UpdateWindow(mhWnd);
		}
	}

	void BaseWindow::OnHide() {
	}

	bool BaseWindow::IsVisiable()
	{
		return IsWindowVisible(mhWnd);
	}

	void BaseWindow::SetParent(BaseWindow*parent)
	{
		if (parent != nullptr)
		{
			::SetParent(mhWnd, parent->GetHwnd());
		}
		mParent = parent;
	}

	void BaseWindow::SetBkgColor(Gdiplus::Color &color)
	{
		mBKGColor = color;
	}

	void BaseWindow::Update()
	{
	}

	void BaseWindow::PaintWindow()
	{
		OnPaint();
	}

	int BaseWindow::GetX()
	{
		return mRect.X;
	}

	int BaseWindow::GetY()
	{
		return mRect.Y;
	}

	int BaseWindow::GetWidth()
	{
		return mRect.Width;
	}

	int BaseWindow::GetHeight()
	{
		return mRect.Height;
	}

	void BaseWindow::SetRect(int x, int y, int width, int height)
	{
		mRect.X = x;
		mRect.Y = y;
		mRect.Width = width;
		mRect.Height = height;
	}

	void BaseWindow::SetRect(Gdiplus::Rect &rect)
	{
		mRect = rect;
	}

	Gdiplus::Rect BaseWindow::GetRect()
	{
		return mRect;
	}

	void BaseWindow::MoveWindow(int x, int y, int width, int height)
	{
		SetRect(x, y, width, height);
		::MoveWindow(mhWnd, x, y, width, height,false);
	}

	void BaseWindow::SetSize(int width, int height,HWND param)
	{
		mRect.Width = width;
		mRect.Height = height;
		SetWindowPos(mhWnd, param, 0, 0, mRect.Width, mRect.Height, SWP_NOREDRAW);
	}

	BaseWindow*BaseWindow::GetParent()
	{
		return mParent;
	}

	HWND BaseWindow::GetHwnd()
	{
		return mhWnd;
	}

	const char*BaseWindow::GetWindowName()
	{
		return mName;
	}

	void BaseWindow::SetWindowName(const char*name)
	{
		memset(mName, 0, 64);
		strcpy(mName, name);
	}

	HDC BaseWindow::GetDC()
	{
		return mHDC;
	}

	ATOM BaseWindow::RegisterWindowClass(UINT style, LPCTSTR pWndClassName, WNDPROC wndProc){
		WNDCLASSEX wcex;
		memset(&wcex, 0, sizeof(wcex));
		wcex.cbSize = sizeof(wcex);
		wcex.style = style;
		wcex.lpfnWndProc = wndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = GetModuleHandle(NULL);
		wcex.hIcon = NULL;
		wcex.hIconSm = NULL;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = pWndClassName;
		ATOM atom = RegisterClassEx(&wcex);
		if (!atom){
			return 0;
		}
		return atom;
	}
}