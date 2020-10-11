#include "UINode.h"
namespace Editor{
	UINode::UINode(){
		mLeftMargin = 0;
		mRightMargin = 0;
		mTopMargin = 0;
		mBottomMargin = 0;
		mIntersectPos = UINodeIntersectPosNone;
		mHorizontalLocationCatagory = kUILocationCatagoryNone;
		mVerticalLocationCatagory = kUILocationCatagoryNone;
		mOnTouchBegin = nullptr;
		mOnTouchEnd = nullptr;
		mOnTouchCanceled = nullptr;
		mOnTouchMove = nullptr;
		mOnClicked = nullptr;
	}
	bool UINode::TestIntersect(int x, int y){
		mIntersectPos = UINodeIntersectPosNone;
		if (mRect.Contains(x,y)){
			mIntersectPos = UINodeIntersectPosMiddle;
			if (y <= mRect.Y + mTopMargin){
				mIntersectPos = UINodeIntersectPosUpper;
			}else if (y >= mRect.Y + mRect.Height - mBottomMargin){
				mIntersectPos = UINodeIntersectPosBottom;
			}
			return true;
		}
		return false;
	}
	UINode*UINode::Intersect(int x, int y){
		UINode*ret = nullptr;
		if (((UINode*)mLastChild) != nullptr) {
			ret = ((UINode*)mLastChild)->Intersect(x,y);
		}
		if (ret != nullptr) {
			return ret;
		}
		if (TestIntersect(x,y)){
			return this;
		}
		if (mLeftSibling != nullptr) {
			ret = LeftSibling<UINode>()->Intersect(x, y);
		}
		return ret;
	}
	void UINode::DrawRecursively(Gdiplus::Graphics&painter) {
		Draw(painter);
		if (mChild != nullptr) {
			Child<UINode>()->DrawRecursively(painter);
		}
		if (mRightSibling != nullptr) {
			RightSibling<UINode>()->DrawRecursively(painter);
		}
	}
	void UINode::Draw(Gdiplus::Graphics&painter){
	}
	void UINode::SetAnchor(float centerX, float centerY) {
		mCenterPosX = centerX;
		mCenterPosY = centerY;
	}
	void UINode::SetRect(Gdiplus::Rect &rect){
		mRect = rect;
	}
	void UINode::SetPos(int x, int y){
		mRect.X = x;
		mRect.Y = y;
	}
	void UINode::SetRect(int x, int y, int width, int height){
		mRect.X = x;
		mRect.Y = y;
		mRect.Width = width;
		mRect.Height = height;
	}
	Gdiplus::Rect & UINode::GetRect(){
		return mRect;
	}
	void UINode::SetOnTouchBeginHandler(VOID_VOID_PTR foo) {
		mOnTouchBegin = foo;
	}
	void UINode::SetOnTouchEndHandler(VOID_VOID_PTR foo) {
		mOnTouchEnd = foo;
	}
	void UINode::SetOnTouchCanceledHandler(VOID_VOID_PTR foo) {
		mOnTouchCanceled = foo;
	}
	void UINode::SetOnTouchMoveHandler(VOID_VOID_PTR foo) {
		mOnTouchMove = foo;
	}
	void UINode::SetOnClickedHandler(VOID_VOID_PTR foo) {
		mOnClicked = foo;
	}
	void UINode::OnTouchBegin(int x, int y, int touch_id/* =0 */) {

	}
	void UINode::OnTouchEnd(int x, int y, int touch_id /* = 0 */) {

	}
	void UINode::OnTouchCanceled(int x, int y, int touch_id /* = 0 */) {

	}
	void UINode::OnTouchMove(int x, int y, int touch_id /* = 0 */) {

	}
	void UINode::OnClicked(int x, int y, int touch_id /* = 0 */) {

	}
	void UINode::OnTouchEnter(int x, int y, int touch_id /* = 0 */) {

	}
	void UINode::OnTouchLeave(int x, int y, int touch_id /* = 0 */) {

	}
	void UINode::OnContainerSizeChanged(int width, int height) {
		int new_x_pos = mRect.X;
		int new_y_pos = mRect.Y;
		switch (mHorizontalLocationCatagory){
		case Editor::UINode::kUILocationCatagoryRelativeToRight:
			new_x_pos = width - int(mCenterPosX) - mRect.Width/2;
			break;
		case Editor::UINode::kUILocationCatagoryPercentageAbsolute:
		case Editor::UINode::kUILocationCatagoryPercentageRelativeToLeft:
		case Editor::UINode::kUILocationCatagoryPercentageRelativeToRight:
			new_x_pos = int(width * mCenterPosX) - mRect.Width / 2;
			break;
		}
		switch (mVerticalLocationCatagory){
		case Editor::UINode::kUILocationCatagoryRelativeToBottom:
			new_y_pos = height - int(mCenterPosY) - mRect.Height / 2;
			break;
		case Editor::UINode::kUILocationCatagoryPercentageAbsolute:
		case Editor::UINode::kUILocationCatagoryPercentageRelativeToTop:
		case Editor::UINode::kUILocationCatagoryPercentageRelativeToBottom:
			new_y_pos = int(height * mCenterPosY) - mRect.Height / 2;
			break;
		}
		mRect.X = new_x_pos;
		mRect.Y = new_y_pos;
	}
	void UINode::OnContainerSizeChangedRecursively(int width, int height) {
		OnContainerSizeChanged(width, height);
		if (mChild != nullptr) {
			Child<UINode>()->OnContainerSizeChangedRecursively(width, height);
		}
		if (mRightSibling != nullptr) {
			RightSibling<UINode>()->OnContainerSizeChangedRecursively(width, height);
		}
	}
}