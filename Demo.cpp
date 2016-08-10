// Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SerialPort.h"
#include "Print.h"

CPrint * g_clsPrint = new CPrint();


char * pString = "-------------------------------\n"
				"购货总额：                11.00\n"
				"付款：   现金 人民币     101.00\n"
				"找零：   现金 人民币      90.00\n"
				"   售出商品数量：4件\n"
				"      2005-09-13  16:50:19\n"
				"       欢迎光临   多谢惠顾\n"
				"        （开发票当月有效）\n"
				"       满家福百货南邮店\n"
				"小票：270500027721 收银员：01012\n"
				"-------------------------------\n";
char *pString2 = "123456789101112131415161718192021222324252627282930\n";

int main(int argc, char* argv[])
{

	g_clsPrint->setPortName(2);
	g_clsPrint->setBaud(115200);
	g_clsPrint->InitPort();
//////////////////////////////////////////////////////////
//以下是打印机的功能测试
// 	//1. 打印位图
//	g_clsPrint->MW_PrintBitmap("D:\\VC_Project\\Demo\\熊猫.bmp");
	
// 	// 2. 实时状态传送【1 参数不可用】
// 	unsigned char err;
// 	for(int i = 1; i < 5; i++)
// 	{	
// 		err = 0;
// 		err = g_clsPrint->MW_RealTimeStatus(i);
// 		printf("i = %d err = 0x%X\n", i, err);
// 	}

//     // 3. 设置字符右间距，如 A B 变成 A    B
//  	g_clsPrint->MW_SetCharRightSpace(10);
//     
//     // 4. 打印字符串
//     unsigned char err;
//     err = g_clsPrint->MW_PrintString(pString2);
//     printf("err = %d\n", err);



//////////////////////////////////////////////////////////
	g_clsPrint->ClosePort();



	return 0;
}
