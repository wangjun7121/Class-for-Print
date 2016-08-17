// Print.h: interface for the CPrint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINT_H__9E45703F_368A_4A8A_A766_D1D4A87A9464__INCLUDED_)
#define AFX_PRINT_H__9E45703F_368A_4A8A_A766_D1D4A87A9464__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SerialPort.h"

class CPrint : public CSerialPort  
{
public:
    int MW_SetPrinterAgruments(unsigned char n, unsigned char m);
    int MW_SetHanZiAsTwiceHeightAndWidth(unsigned char n);
    int MW_SetHanZiLeftAndRightSpace(unsigned char ucLeft, unsigned char ucRight);
    int MW_ShowHanZiUndline(unsigned char n);
    int MW_SetHanZiMode(unsigned char n);
    int MW_SetBarcodeWidth(unsigned char n);
    int MW_PrintBarcode(unsigned char ucMode, char *pcData);
    int MW_SetBarcodeHeight(unsigned char n);
    int MW_SetHriFont(unsigned char n);
    int MW_SetPrintWidth(unsigned int n);
	int MW_SetPrintLeftMargin(unsigned int n);
	int MW_SetHriLocation(unsigned char n);
	int MW_SelectBlackAndWhiteReverse(unsigned char n);
	int MW_SelectFontSize(unsigned char n);
	int MW_PrintFlashBitmap(unsigned char n);
	int MW_DownloadBitmapToFlash(char *pcBitmapAddr);
	int MW_SelectAlignMode(unsigned char n);
	int MW_SetRefPrintPosition(unsigned int iPost);
	int MW_SelectAsciiFont(unsigned char n);
	int MW_SetBoldMode(unsigned char n);
	int MW_InitPrint(void);
	int MW_SetLineSpace(unsigned char n);
	int MW_SetDefaultLineSpace(void);
	int MW_SetUndlineMode(unsigned char ucMode);
	int MW_SetAbsPrintPosition(unsigned int iPost);
	int MW_SelectPrintMode(unsigned char ucMode);
	int MW_PrintString(char * pcString);
	int MW_SetCharRightSpace(unsigned char n);
	int MW_PrintBitmap(char * pcPicAddr);
	unsigned char MW_RealTimeStatus(unsigned char n);
	void MW_LF(void);
	CPrint();
	virtual ~CPrint();

private:
	int GetPrintStatus();
};

#endif // !defined(AFX_PRINT_H__9E45703F_368A_4A8A_A766_D1D4A87A9464__INCLUDED_)
