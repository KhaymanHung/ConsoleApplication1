// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//�N��r ch ���I�}�Ϧs�J buf ��, �I�}�Ϥj�p�O���� bytes ���Ǧ^
//�r��=face, �r�Τj�p=size, [���禡�b Unicode �Ҧ��U���ե��`]

int text_bitmap(HDC hdc, UINT ch, char* buf, GLYPHMETRICS& gm,
	int size = 16, wchar_t* face = L"�ө���")
{
	MAT2    mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };    //��m�x�}
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
*	���o��rbuffer
*	�q�r��s�X���o�r�骺�w��
*
*	@param hDC	[in] �]�ƥ��媫��
*	@param Code	[in] �r��s�X
*	@param pGM	[in] GLYPHMETRICS���c������
*	@param ppData	[in] �r��w��
*	@param Alpha	[in] antialias
*
*	@return	�w�Ĥؤo
*
*/
long GetFontBuffer(HDC hDC, USHORT Code, char* ppData, GLYPHMETRICS& pGm)
{
	//!-
	//! �x�}�ഫ
	//!	| 1 0 |
	//!	| 0 1 |
	//!-
	static long m11 = (long)(1.0f * 65536.0f);
	static long m12 = (long)(0.0f * 65536.0f);
	static long m21 = (long)(0.0f * 65536.0f);
	static long m22 = (long)(1.0f * 65536.0f);

	//!	TF�r����m�x�}
	MAT2 Matrix;
	Matrix.eM11 = *((FIXED *)&m11);
	Matrix.eM12 = *((FIXED *)&m12);
	Matrix.eM21 = *((FIXED *)&m21);
	Matrix.eM22 = *((FIXED *)&m22);

	HFONT   font = CreateFont(16, 0, 0, 0, 400, 0, 0, 0, 0, 0, 0, 0, 0, L"�ө���");
	HGDIOBJ oldFont = SelectObject(hDC, font);

	//! �եΨ��GetGlyphOutline�i�H�o��@�Ӧr�������V�q�Ϊ̦��
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
	//int sz = text_bitmap(hdc, L'��', a, gm);
	int sz = GetFontBuffer(hdc, L'���', a, gm);
	int nx = sz / gm.gmBlackBoxY;     //�C��e�XByte, gmBlackBoxX=�줸�e��
									  //��Y ((gm.gmBlackBoxX+31)>>5)<<2   

	for (int x, j, i = 0; i<sz; puts(""))
		for (x = 0; x<nx; x++, i++)
			for (j = 1 << 8; j; j >>= 1)
				printf(a[i] & j ? "��" : "��");

	DeleteDC(hdc);

	return system("pause");
}