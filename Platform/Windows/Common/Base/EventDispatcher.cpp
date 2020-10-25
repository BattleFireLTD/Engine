#include "EventDispatcher.h"
#include "Runtime/Debugger/Logger.h"
namespace Editor
{
	EventDispatcher::EventDispatcher() :mbMouseLeaved(false)
	{
		mbIsIMEInput = false;
	}

	void EventDispatcher::OnLButtonDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void EventDispatcher::OnLButtonUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void EventDispatcher::OnLButtonDoubleClick(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void EventDispatcher::OnKeyDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}
	void EventDispatcher::OnKeyUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}
	void EventDispatcher::OnChar(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}
	void EventDispatcher::OnIMEChar(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}
	void EventDispatcher::OnMouseWheel(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}
	void EventDispatcher::OnCloseWindow(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void EventDispatcher::OnFocus(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void EventDispatcher::OnSize(WPARAM wParam, LPARAM lParam, void*reserved)
	{

	}

	LRESULT EventDispatcher::OnSizing(WPARAM wParam, LPARAM lParam, void*reserved)
	{
		return DefWindowProc(mhWnd, WM_SIZING, wParam, lParam);
	}

	void EventDispatcher::OnMove(WPARAM wParam, LPARAM lParam, void*reserved)
	{
	}

	void EventDispatcher::OnMoving(WPARAM wParam, LPARAM lParam, void*reserved)
	{
	}

	void EventDispatcher::OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void EventDispatcher::OnPaint(const Gdiplus::Rect & rect_need_update){
	}

	void EventDispatcher::OnTimer(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void EventDispatcher::OnCommand(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}


	void EventDispatcher::OnRButtonDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void EventDispatcher::OnRButtonUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void EventDispatcher::OnMiddleButtonDown(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void EventDispatcher::OnMiddleButtonUp(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void EventDispatcher::OnDropFiles(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{

	}

	void EventDispatcher::OnMouseMove(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		if (!mbMouseLeaved)
		{
			TRACKMOUSEEVENT csTME;
			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE | TME_HOVER;
			csTME.hwndTrack = mhWnd;
			csTME.dwHoverTime = 10;
			TrackMouseEvent(&csTME);
			mbMouseLeaved = true;
		}
	}

	void EventDispatcher::OnMouseLeave(WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		mbMouseLeaved = false;
	}

	LRESULT EventDispatcher::OnNCPAINT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT EventDispatcher::OnNCACTIVATE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT EventDispatcher::OnNCCALCSIZE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT EventDispatcher::OnNCHITTEST(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT EventDispatcher::OnNCLBUTTONUP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT EventDispatcher::OnNCLBUTTONDOWN(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT EventDispatcher::OnNCLBUTTONDBLCLK(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT EventDispatcher::OnNCMOUSEMOVE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT EventDispatcher::OnNCRBUTTONUP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT EventDispatcher::OnNCRBUTTONDOWN(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, void*reserved /* = nullptr */)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT EventDispatcher::WindowEventProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
		EventDispatcher*self = (EventDispatcher*)GetWindowLongPtr(hWnd,GWLP_USERDATA);
		if (self==nullptr){
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		switch (message){
		case WM_MOUSEMOVE:{
			POINT pos;
			GetCursorPos(&pos);
			RECT rect;
			GetWindowRect(hWnd, &rect);
			self->OnMouseMove(wParam, MAKELPARAM(pos.x - rect.left, pos.y - rect.top));
		}
			return 0;
		case WM_MOUSELEAVE:
			self->OnMouseLeave(wParam, lParam);
			return 0;
		case WM_LBUTTONDOWN:{
			POINT pos;
			GetCursorPos(&pos);
			RECT rect;
			GetWindowRect(hWnd, &rect);
			self->OnLButtonDown(wParam, MAKELPARAM(pos.x - rect.left, pos.y - rect.top));
		}
		return 0;
		case WM_LBUTTONUP:{
			POINT pos;
			GetCursorPos(&pos);
			RECT rect;
			GetWindowRect(hWnd, &rect);
			self->OnLButtonUp(wParam, MAKELPARAM(pos.x - rect.left, pos.y - rect.top));
		}
			return 0;
		case WM_LBUTTONDBLCLK:
			self->OnLButtonDoubleClick(wParam, lParam);
			return 0;
		case WM_RBUTTONDOWN:
			self->OnRButtonDown(wParam, lParam);
			return 0;
		case WM_RBUTTONUP:
			self->OnRButtonUp(wParam, lParam);
			return 0;
		case WM_MBUTTONDOWN:
			self->OnMiddleButtonDown(wParam, lParam);
			return 0;
		case WM_MBUTTONUP:
			self->OnMiddleButtonUp(wParam, lParam);
			return 0;
		case WM_MOUSEWHEEL:
			self->OnMouseWheel(wParam, lParam);
			return 0;
		case WM_KEYDOWN:
			//debug("WM_KEYDOWN");
			self->OnKeyDown(wParam, lParam);
			return 0;
		case WM_KEYUP:
			//debug("WM_KEYUP");
			self->OnKeyUp(wParam, lParam);
			return 0;
		case WM_CHAR:
			//debug("WM_CHAR");
			self->OnChar(wParam, lParam);
			return 0;
			/*case WM_IME_COMPOSITION:
				debug("WM_IME_COMPOSITION");
				break;
			case WM_IME_COMPOSITIONFULL:
				debug("WM_IME_COMPOSITIONFULL");
				break;
			case WM_IME_CONTROL:
				debug("WM_IME_CONTROL");
				break;
			case WM_IME_KEYDOWN:
				debug("WM_IME_KEYDOWN");
				break;
			case WM_IME_KEYUP:
				debug("WM_IME_KEYUP");
				break;
			case WM_IME_NOTIFY:
				debug("WM_IME_NOTIFY");
				break;
			case WM_IME_REQUEST:
				debug("WM_IME_REQUEST");
				return 0;
			case WM_IME_SELECT:
				debug("WM_IME_SELECT");
				break;
			case WM_IME_SETCONTEXT:
				debug("WM_IME_SETCONTEXT");
				break;*/
		case WM_IME_STARTCOMPOSITION:
			//debug("WM_IME_STARTCOMPOSITION");
			self->mbIsIMEInput = true;
			break;
		case WM_IME_ENDCOMPOSITION:
			//debug("WM_IME_ENDCOMPOSITION");
			self->mbIsIMEInput = false;
			break;
		case WM_IME_CHAR:
			//debug("WM_IME_CHAR");
			self->OnIMEChar(wParam, lParam);
			return 0;
		case WM_COMMAND:
			self->OnCommand(wParam, lParam);
			return 0;
		case WM_DROPFILES:
			self->OnDropFiles(wParam, lParam);
			return 0;
		case WM_CLOSE:
			self->OnCloseWindow(wParam, lParam);
			return 0;
		case WM_SETFOCUS:
			self->OnFocus(wParam, lParam);
			return 0;
		case WM_ACTIVATE:
			return 0;
		case WM_ACTIVATEAPP:
			return 0;
		case WM_SIZE:
			self->OnSize(wParam, lParam);
			return 0;
		case WM_SIZING:
			return self->OnSizing(wParam, lParam);
		case WM_MOVE:
			self->OnMove(wParam, lParam);
			return 0;
		case WM_MOVING:
			self->OnMoving(wParam, lParam);
			return TRUE;
		case WM_GETMINMAXINFO:
			self->OnGetMinMaxInfo(wParam, lParam);
			return 0;
		case WM_TIMER:
			self->OnTimer(wParam, lParam);
			return 0;
		case WM_ERASEBKGND:
			self->OnEraseBKG();
			return 1;
		case WM_PAINT:{
				RECT invalid_rect;
				if (GetUpdateRect(hWnd, &invalid_rect, true)) {
					PAINTSTRUCT ps;
					HDC hdc = BeginPaint(hWnd, &ps);
					self->OnPaint(Gdiplus::Rect(invalid_rect.left,invalid_rect.top,invalid_rect.right-invalid_rect.left,invalid_rect.bottom-invalid_rect.top));
					EndPaint(hWnd, &ps);
					self->OnPostPaint();
				}
			}
			return 0;
		case WM_NCPAINT:
			return self->OnNCPAINT(hWnd, message, wParam, lParam);
		case WM_NCACTIVATE:
			return self->OnNCACTIVATE(hWnd, message, wParam, lParam);
		case WM_NCCALCSIZE:
			return self->OnNCCALCSIZE(hWnd, message, wParam, lParam);
		case WM_NCHITTEST:
			return self->OnNCHITTEST(hWnd, message, wParam, lParam);
		case WM_NCLBUTTONUP:
			return self->OnNCLBUTTONUP(hWnd, message, wParam, lParam);
		case WM_NCLBUTTONDOWN:
			return self->OnNCLBUTTONDOWN(hWnd, message, wParam, lParam);
		case WM_NCLBUTTONDBLCLK:
			return self->OnNCLBUTTONDBLCLK(hWnd, message, wParam, lParam);
		case WM_NCMOUSEMOVE:
			return self->OnNCMOUSEMOVE(hWnd, message, wParam, lParam);
		case WM_NCRBUTTONUP:
			return self->OnNCRBUTTONUP(hWnd, message, wParam, lParam);
		case WM_NCRBUTTONDOWN:
			return self->OnNCRBUTTONDOWN(hWnd, message, wParam, lParam);
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}