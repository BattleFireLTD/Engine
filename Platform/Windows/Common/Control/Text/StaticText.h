#pragma once
#include "../UINode.h"

namespace Editor
{
	enum AligningMode
	{
		AligningModeLeft,
		AligningModeMiddle
	};
	class StaticText:public UINode
	{
	protected:
		char mText[128];
		TCHAR mWText[128];
		Gdiplus::Rect mBkgColorRect;
		Gdiplus::Color mBkgColor;
		bool mbDrawBkgColor;
		Gdiplus::Color mTextColor;
		AligningMode mAligning;
	public:
		StaticText();
		void SetAligning(AligningMode aligning);
		void SetText(const char* text);
		const char* GetText();
		LPCTSTR GetWText();
		void SetTextColor(BYTE r,BYTE g,BYTE b);
		Gdiplus::Color&GetTextColor();
		void SetBkgColor(BYTE r, BYTE g, BYTE b);
		bool ShowBkgColor(bool bShow);
		bool IsShowBkgColor();
		void SetBkgColorRect(int x,int y,int width,int height);
		void AdjustRect(int x,int y,int width,int height);

		bool operator==(StaticText&right);
		bool operator==(const char*text);
		bool operator!=(const char*text);
		void operator=(const char*text);
		void operator=(LPCTSTR text);
	public:
		virtual void Draw(Gdiplus::Graphics&painter);
	public:
		static Gdiplus::Font *SharedFont;
		static Gdiplus::StringFormat*SharedStringFormat;
	};
}