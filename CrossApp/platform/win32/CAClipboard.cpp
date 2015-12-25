
#include "platform/CAClipboard.h"
#include "basics/CAApplication.h"
#include "support/ccUTF8.h"


NS_CC_BEGIN

std::string CAClipboard::getText()
{
	std::string str;
	if (::OpenClipboard(NULL))
	{
		HGLOBAL hGlobal = ::GetClipboardData(CF_TEXT);

		if (hGlobal)
		{
			LPSTR lpszData = (LPSTR) ::GlobalLock(hGlobal);
			if (lpszData)
			{
				str = lpszData;
				::GlobalUnlock(hGlobal);
			}
		}
		::CloseClipboard();
	}
	setlocale(LC_ALL, "");

	size_t nDsize = str.size() + 8;
	wchar_t *pDest = new wchar_t[nDsize];
	wmemset(pDest, 0, nDsize);

	mbstowcs(pDest, str.c_str(), nDsize);

	std::wstring result = pDest;
	delete[]pDest;
	return unicode_to_utf8(result.c_str());
}

void CAClipboard::setText(const std::string& cszStrText)
{
	BOOL bSuccess = FALSE;

	std::wstring strText = utf8_to_unicode(cszStrText.c_str());

	if (::OpenClipboard(NULL))
	{
		::EmptyClipboard();

		int nSize = (strText.size() + 1) * 2;

		HGLOBAL hGlobal = ::GlobalAlloc(GMEM_ZEROINIT, nSize);
		if (hGlobal)
		{
			LPTSTR lpszData = (LPTSTR) ::GlobalLock(hGlobal);
			if (lpszData)
			{
				memcpy(lpszData, strText.c_str(), nSize);
				::GlobalUnlock(hGlobal);
				::SetClipboardData(CF_UNICODETEXT, hGlobal);
			}
		}
		::CloseClipboard();
	}
}

NS_CC_END
