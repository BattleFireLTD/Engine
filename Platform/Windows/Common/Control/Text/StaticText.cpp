#include "StaticText.h"
#include "Runtime/String/StringUtils.h"
#pragma warning(disable:4244)
namespace Editor
{
	Gdiplus::Font* StaticText::SharedFont = nullptr;
	Gdiplus::StringFormat*StaticText::SharedStringFormat = nullptr;
	StaticText::StaticText():mBkgColor(0, 100, 150),mTextColor(200, 200, 200),mbDrawBkgColor(false),mAligning(AligningModeLeft)
	{
		if (SharedFont==nullptr)
		{
			static Gdiplus::FontFamily fontfamily(TEXT("宋体"));
			SharedFont = new Gdiplus::Font(&fontfamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);

			SharedStringFormat = new Gdiplus::StringFormat;
			SharedStringFormat->SetAlignment(Gdiplus::StringAlignmentNear);
			SharedStringFormat->SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
		}
	}

	void StaticText::SetBkgColor(BYTE r, BYTE g, BYTE b)
	{
		mBkgColor = Gdiplus::Color(r,g,b);
	}

	void StaticText::SetBkgColorRect(int x, int y, int width, int height)
	{
		mBkgColorRect = Gdiplus::Rect(x,y,width,height);
	}

	void StaticText::AdjustRect(int x, int y, int width, int height)
	{
		Gdiplus::StringFormat sf;
		sf.SetAlignment(Gdiplus::StringAlignmentNear);

		Gdiplus::RectF bound;

	}

	bool StaticText::operator==(StaticText&right)
	{
		return 0 == strcmp(right.GetText(), mText);
	}

	bool StaticText::operator ==(const char*text)
	{
		return strcmp(mText, text) == 0;
	}

	bool StaticText::operator !=(const char*text)
	{
		return strcmp(mText, text) != 0;
	}

	void StaticText::SetTextColor(BYTE r, BYTE g, BYTE b)
	{
		mTextColor = Gdiplus::Color(r,g,b);
	}

	bool StaticText::ShowBkgColor(bool bShow)
	{
		if (mbDrawBkgColor!=bShow)
		{
			mbDrawBkgColor = bShow;
			return true;
		}
		return false;
	}

	bool StaticText::IsShowBkgColor()
	{
		return mbDrawBkgColor;
	}

	Gdiplus::Color&StaticText::GetTextColor()
	{
		return mTextColor;
	}

	void StaticText::SetText(const char* text)
	{
		memset(mText, 0, 128);
		if (strlen(text)>127)
		{
			strncpy(mText, text, 124);
			strcat(mText, "...");
		}
		else
		{
			strcpy(mText, text);
		}
		wmemset(mWText, 0, 128);
		Alice::StringUtils::ASCIIToUnicode(mText, mWText);
	}

	void StaticText::operator=(const char*text)
	{
		memset(mText, 0, 128);
		strcpy(mText, text);
		wmemset(mWText, 0, 128);
		Alice::StringUtils::ASCIIToUnicode(mText, mWText);
	}

	void StaticText::operator=(LPCTSTR text)
	{
		wmemset(mWText, 0, 128);
		lstrcpy(mWText, text);
		memset(mText, 0, 128);
		Alice::StringUtils::UnicodeToASCII( mWText, mText,128);
	}

	void StaticText::SetAligning(AligningMode aligning)
	{
		mAligning=aligning;
	}

	const char* StaticText::GetText()
	{
		return mText;
	}

	LPCTSTR StaticText::GetWText()
	{
		return mWText;
	}

	void StaticText::Draw(Gdiplus::Graphics&painter)
	{
		painter.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		painter.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);

		Gdiplus::SolidBrush brush(mBkgColor);
		if (mbDrawBkgColor)
		{
			painter.FillRectangle(&brush, mBkgColorRect);
		}
		//最后在自己的Folder@16的Icon旁边协商自己的文件夹名字
		brush.SetColor(mTextColor);
		//如果被选中，则绘制一个蓝色背景条
		if (mAligning==AligningModeLeft)
		{
			painter.DrawString(mWText, -1, SharedFont, Gdiplus::PointF(mRect.X, mRect.Y + 2), SharedStringFormat, &brush);
		}
		else if(mAligning==AligningModeMiddle)
		{
			Gdiplus::RectF rect;
			painter.MeasureString(mWText,lstrlen(mWText),SharedFont, Gdiplus::PointF(0,0),SharedStringFormat,&rect);
			int offsetX=mRect.X+mRect.Width / 2-rect.Width/2;
			painter.DrawString(mWText, -1, SharedFont, Gdiplus::PointF(offsetX, mRect.Y + 2), SharedStringFormat, &brush);
		}
		UINode::Draw(painter);
	}
}