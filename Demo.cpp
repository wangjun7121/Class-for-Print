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
char *pString2 = "1234567891011121314151617181920\n";
char *pString3 = "123456789\n";
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

//     // 3. 设置字符右间距，如 A B C 变成 A    B   C 
//      g_clsPrint->MW_SetCharRightSpace(10);
//     
//     // 4. 打印字符串
//     unsigned char err;
//     err = g_clsPrint->MW_PrintString(pString2);
//     printf("err = %d\n", err);

//     // 5. 打印模式选择
//     unsigned char ucMode = 0;
// 
//     // (1 << 7) 下划线  : 没什么效果, 很淡
//     // (1 << 5) 倍宽 
//     // (1 << 4) 倍高 
//     // (1 << 3) 加粗   
//     ucMode |= (1 << 3);
//     g_clsPrint->MW_SelectPrintMode(ucMode);                                         
//     g_clsPrint->MW_PrintString(pString2);

//     // 6. 设置绝对打印位置 
//     g_clsPrint->MW_SetAbsPrintPosition(65500);
//     g_clsPrint->MW_PrintString(pString2);

//     // 7. 选择/取消下划线模式
//     unsigned char ucMode = 2;
//     // 0,48            取消下划线模式
//     // 1,49            选择下划线模式(1 点宽)
//     // 2,50            选择下划线模式(2 点宽)
//     g_clsPrint->MW_SetUndlineMode(ucMode);
//     ucMode = 0;
//     ucMode |= (1 << 7)|(1 << 3); // (1 << 7) 下划线 
//     g_clsPrint->MW_SelectPrintMode(ucMode); 
//     g_clsPrint->MW_PrintString(pString2);

//     // 8. 设置行间距 
//     g_clsPrint->MW_PrintString(pString2);
//     g_clsPrint->MW_SetLineSpace(200);
//     g_clsPrint->MW_PrintString(pString2);
//     g_clsPrint->MW_SetDefaultLineSpace();
//     g_clsPrint->MW_PrintString(pString2);
    
//     // 9. 选择对齐方式
//     g_clsPrint->MW_SelectAlignMode(1);
//     g_clsPrint->MW_PrintString(pString3);
// 
//     g_clsPrint->MW_SelectAlignMode(2);
//     g_clsPrint->MW_PrintString(pString3);
// 
//     g_clsPrint->MW_SelectAlignMode(0);
//     g_clsPrint->MW_PrintString(pString3);

    // 10， 下载/打印 Flash 中的位图 
    g_clsPrint->MW_DownloadBitmapToFlash("D:\\VC_Project\\Demo\\Bitmap_中国.bmp");

//////////////////////////////////////////////////////////
	g_clsPrint->ClosePort();



	return 0;
}
