#pragma once
#include "Runtime/BattleFirePrefix.h"
#include "Runtime/Utils/TTree.h"
namespace Editor{
	enum UINodeIntersectPos{
		UINodeIntersectPosUpper,
		UINodeIntersectPosMiddle,
		UINodeIntersectPosBottom,
		UINodeIntersectPosNone
	};
	class UINode : public Alice::TTree {
	public:
		UINode();
		UINodeIntersectPos mIntersectPos;
	public:
		void DrawRecursively(Gdiplus::Graphics&painter);
		virtual void Draw(Gdiplus::Graphics&painter);
		virtual void SetRect(Gdiplus::Rect &rect);
		virtual void SetPos(int x,int y);
		virtual void SetRect(int x, int y, int width, int height);
		virtual Gdiplus::Rect&GetRect();
		virtual bool TestIntersect(int x, int y);
		virtual UINode*Intersect(int x, int y);
		virtual void OnTouchBegin(int x, int y,int touch_id=0);
		virtual void OnTouchEnd(int x, int y, int touch_id = 0);
		virtual void OnTouchMove(int x, int y, int touch_id = 0);
		virtual void OnTouchCanceled(int x, int y, int touch_id = 0);
		virtual void OnTouchEnter(int x, int y, int touch_id = 0);
		virtual void OnTouchLeave(int x, int y, int touch_id = 0);
		virtual void OnClicked(int x, int y, int touch_id = 0);
		virtual void ProcessEvent(int nCommandID) {}
		void SetOnTouchBeginHandler(VOID_VOID_PTR foo);
		void SetOnTouchEndHandler(VOID_VOID_PTR foo);
		void SetOnTouchMoveHandler(VOID_VOID_PTR foo);
		void SetOnTouchCanceledHandler(VOID_VOID_PTR foo);
		void SetOnClickedHandler(VOID_VOID_PTR foo);
	protected:
		Gdiplus::Rect mRect;
		int mLeftMargin,mRightMargin,mTopMargin,mBottomMargin;
		VOID_VOID_PTR mOnTouchBegin, mOnTouchEnd, mOnTouchCanceled, mOnTouchMove,mOnClicked;
	};
}