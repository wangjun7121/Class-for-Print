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
	int MW_SelectPrintMode(unsigned char ucMode);
	int MW_PrintString(char * pcString);
	int MW_SetCharRightSpace(unsigned char n);
	int MW_PrintBitmap(char * pcPicAddr);
	unsigned char MW_RealTimeStatus(unsigned char n);
	void MW_LF(void);
	CPrint();
	virtual ~CPrint();

};

#endif // !defined(AFX_PRINT_H__9E45703F_368A_4A8A_A766_D1D4A87A9464__INCLUDED_)
