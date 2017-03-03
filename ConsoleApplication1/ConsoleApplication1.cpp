// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//將文字 ch 的點陣圖存入 buf 中, 點陣圖大小記錄於 bytes 中傳回
//字體=face, 字形大小=size, [此函式在 Unicode 模式下測試正常]

int text_bitmap(HDC hdc, UINT ch, char* buf, GLYPHMETRICS& gm,
	int size = 16, wchar_t* face = L"細明體")
{
	MAT2    mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };    //轉置矩陣
	HFONT   font = CreateFont(size, 0, 0, 0, 400, 0, 0, 0, 0, 0, 0, 0, 0, face);
	HGDIOBJ oldFont = SelectObject(hdc, font);
	int bytes = GetGlyphOutline(hdc, ch, 1, &gm, 0, 0, &mat);
	if (bytes != GDI_ERROR)
		GetGlyphOutline(hdc, ch, 1, &gm, bytes, buf, &mat);
	SelectObject(hdc, oldFont);
	DeleteObject(font);
	return bytes;
}

//-
/**
*	取得文字buffer
*	從字串編碼取得字體的緩衝
*
*	@param hDC	[in] 設備本文物件
*	@param Code	[in] 字串編碼
*	@param pGM	[in] GLYPHMETRICS結構的指標
*	@param ppData	[in] 字體緩衝
*	@param Alpha	[in] antialias
*
*	@return	緩衝尺寸
*
*/
long GetFontBuffer(HDC hDC, USHORT Code, char* ppData, GLYPHMETRICS& pGm)
{
	//!-
	//! 矩陣轉換
	//!	| 1 0 |
	//!	| 0 1 |
	//!-
	static long m11 = (long)(1.0f * 65536.0f);
	static long m12 = (long)(0.0f * 65536.0f);
	static long m21 = (long)(0.0f * 65536.0f);
	static long m22 = (long)(1.0f * 65536.0f);

	//!	TF字體轉置矩陣
	MAT2 Matrix;
	Matrix.eM11 = *((FIXED *)&m11);
	Matrix.eM12 = *((FIXED *)&m12);
	Matrix.eM21 = *((FIXED *)&m21);
	Matrix.eM22 = *((FIXED *)&m22);

	HFONT   font = CreateFont(16, 0, 0, 0, 400, 0, 0, 0, 0, 0, 0, 0, 0, L"細明體");
	HGDIOBJ oldFont = SelectObject(hDC, font);

	//! 調用函數GetGlyphOutline可以得到一個字的輪廓向量或者位圖
	long Size = ::GetGlyphOutline(hDC, Code, 1, &pGm, 0, NULL, &Matrix);
	::GetGlyphOutline(hDC, Code, 1, &pGm, Size, ppData, &Matrix);

	SelectObject(hDC, oldFont);
	DeleteObject(font);

	return Size;
}

int main()
{
	SetConsoleTitle(L"Conso");
	HDC hdc = GetDC(FindWindow(0, L"Conso"));

	GLYPHMETRICS gm;
	char a[512];
	//int sz = text_bitmap(hdc, L'選', a, gm);
	int sz = GetFontBuffer(hdc, L'選擇', a, gm);
	int nx = sz / gm.gmBlackBoxY;     //每行占幾Byte, gmBlackBoxX=位元寬度
									  //亦即 ((gm.gmBlackBoxX+31)>>5)<<2   

	for (int x, j, i = 0; i<sz; puts(""))
		for (x = 0; x<nx; x++, i++)
			for (j = 1 << 8; j; j >>= 1)
				printf(a[i] & j ? "○" : "●");

	DeleteDC(hdc);

	return system("pause");
}