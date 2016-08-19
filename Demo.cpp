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
char *pString4 = "我是中国人";
int main(int argc, char* argv[])
{

	g_clsPrint->setPortName(2);
	g_clsPrint->setBaud(115200);

	if(!g_clsPrint->InitPort())
    {
        fputs ("init comm port failed!\n",stderr);
        return -1;
    }
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

//     // 10， 下载/打印 Flash 中的位图 
//     // 实验图片尺寸: 384 x 168          可以
//     //               1016 x 56          无法显示
//     //               400 x 160          显示不全
//     //               384 x 176          可以显示 384 x 22 = 8448 字节 
//     //               384 x 200          可以显示 384 x 25 = 9600 字节
//     //               384 x 416          超出缓存 384 x 52 = 19968 字节 
//     //               384 x 280          可以显示 384 x 35 = 13440 字节 
//     //               384 x 320          左右，上显示不全 384 x 40 = 15360 字节 
//     //               384 x 360          超出缓存 384 x 45 = 17280 字节
//     //               384 x 288          左右显示不全 384 x 36 = 13824 字节 
//     //               376 x 320          上显示不全 376 x 40 = 15040 字节 
//     //               376 x 344          超出缓存 376 x 43 = 16512 【16K 为 16384 】
//     //               376 x 336          可以显示 376 x 42 = 15792 字节 【这就是 宽与高的最大值 ?】
//     //               160 x 816          无反应  20 x 102 x 8 = 16320 字节 
//     //               160 x 640          可以显示 20 x 80 x 8 = 12800 字节 
//     //               80 x 816           可以打印 10 x 102 x 8 = 8160 字节 
//     //               160 x 800          可以打印 
//     //               376 x 312          上显示不全
//     //               16 x 16            可以打印，经验证，开头有几行会有点重叠，打印机问题
//     g_clsPrint->MW_DownloadBitmapToFlash("D:\\VC_Project\\Demo\\16x16.bmp");
//     unsigned char ucMode = 0;
//     g_clsPrint->MW_PrintFlashBitmap(ucMode);

//     // 11. 选择字体大小 
//     unsigned char ucSize;
//     ucSize = 2;             // 高度
//     ucSize |= 1 << 4;       // 宽度
//     g_clsPrint->MW_selectFontSize(ucSize);
//     g_clsPrint->MW_PrintString(pString3);

//     // 12.条码测试
//     /*
//     ---------------------------------------------------------------------------------------
//     | m(十进制)   | 条码类型       | 字符个数    | 字符                                   |
//     ---------------------------------------------------------------------------------------
//     | 数据格式一：1D 6B m xx xx 00                                                        |
//     ---------------------------------------------------------------------------------------
//     | 0(0x00)     | UPC-A          | 11          | 0-9                                    |
//     ---------------------------------------------------------------------------------------
//     | 1(0x01)     | UPC-E          | 6           | 0-9                                    |
//     ---------------------------------------------------------------------------------------
//     | 2(0x02)     | JAN13/EAN13    | 12          | 0-9                                    |
//     ---------------------------------------------------------------------------------------
//     | 3(0x03)     | JAN8/EAN8      | 7           | 0-9                                    |
//     ---------------------------------------------------------------------------------------
//     | 4(0x04)     | CODE39         | 1-255       | 0-9 A-Z SP,$,%,+,-,.,/, *(开始结束字符)|
//     ---------------------------------------------------------------------------------------
//     | 5(0x05)     | ITF            | 1-255(偶数) | 0-9                                    |
//     ---------------------------------------------------------------------------------------
//     | 6(0x06)     | CODEBAR        | 1-255       | 0-9 A-D $,+,-,.,/,                     |
//     ---------------------------------------------------------------------------------------
//     | 数据格式二：1D 6B m n xx xx                                                         |
//     ---------------------------------------------------------------------------------------
//     | 65(0x41)    | UPC-A          | 11          | 0-9                                    |
//     ---------------------------------------------------------------------------------------
//     | 66(0x42)    | UPC-E          | 6           | 0-9                                    |
//     ---------------------------------------------------------------------------------------
//     | 67(0x43)    | JAN13/EAN13    | 12          | 0-9                                    |
//     ---------------------------------------------------------------------------------------
//     | 68(0x44)    | JAN8/EAN8      | 7           | 0-9                                    |
//     ---------------------------------------------------------------------------------------
//     | 69(0x45)    | CODE39         | 1-255       | 0-9 A-Z SP,$,%,+,-,.,/, *(开始结束字符)|
//     ---------------------------------------------------------------------------------------
//     | 70(0x46)    | ITF            | 1-255(偶数) | 0-9                                    |
//     ---------------------------------------------------------------------------------------
//     | 71(0x47)    | CODEBAR        | 1-255       | 0-9 A-D $,+,-,.,/,                     |
//     ---------------------------------------------------------------------------------------
//     | 72(0x48)    | CODE93         | 1-255       | NUL-SP(7FH)                            |
//     ---------------------------------------------------------------------------------------
//     | 73(0x49)    | CODE128        | 2-255       | NUL-SP(7FH)                            |
//     ---------------------------------------------------------------------------------------
//     */
//     unsigned char ucMode;
//     ucMode = 73;
//     g_clsPrint->MW_PrintBarcode(ucMode, "01234567");

    // 13. 汉字测试
    unsigned char ucMode = 0;
//     unsigned char ucLeft,ucRight;
//     //ucMode |= (1 << 2)|(1 << 3)|(1 << 7);
//     // g_clsPrint->MW_SetHanZiMode(ucMode);
//     //ucMode = 0;
//     //g_clsPrint->MW_ShowHanZiUndline(ucMode);
//     //ucLeft = 10;
//     //ucRight = 10;
//     //g_clsPrint->MW_SetHanZiLeftAndRightSpace(ucLeft, ucRight);
//     ucMode = 1;
//     g_clsPrint->MW_SetHanZiAsTwiceHeightAndWidth(ucMode);
//     g_clsPrint->MW_PrintString(pString4);

    // 大综合测试
    unsigned char ucMode = 0;

    //打印位图
    g_clsPrint->MW_PrintBitmap(".\\花.bmp");
     
    // 下载/打印 Flash 中的位图 
    g_clsPrint->MW_DownloadBitmapToFlash(".\\狗.bmp");
    ucMode = 0;
    g_clsPrint->MW_PrintFlashBitmap(ucMode);


    // 字符右间距
    g_clsPrint->MW_PrintString("字符右间距abc123");   
    g_clsPrint->MW_SetCharRightSpace(50);
    g_clsPrint->MW_PrintString("字符右间距abc123");  
    g_clsPrint->MW_SetCharRightSpace(0);

    // 打印模式：加粗，倍高，倍宽，下划线
    ucMode |= (1 << 3); // 加粗
    g_clsPrint->MW_SelectPrintMode(ucMode);
    g_clsPrint->MW_PrintString("加粗abc123");   
    ucMode |= (1 << 4); // 倍高
    g_clsPrint->MW_SelectPrintMode(ucMode);
    g_clsPrint->MW_PrintString("加粗倍高abc123");   
    ucMode |= (1 << 5); // 倍宽
    g_clsPrint->MW_SelectPrintMode(ucMode);
    g_clsPrint->MW_PrintString("加粗倍高倍宽abc123");   
    ucMode |= (1 << 7); // 下划线
    g_clsPrint->MW_SelectPrintMode(ucMode);
    ucMode = 2; // 设置下划线宽
    g_clsPrint->MW_SetUndlineMode(ucMode);
    g_clsPrint->MW_PrintString("加粗倍高倍宽下划线abc123"); 
    ucMode = 0;
    g_clsPrint->MW_SelectPrintMode(ucMode);
    g_clsPrint->MW_PrintString("常规字体abc123");   

    // 距离行首位置
    g_clsPrint->MW_SetAbsPrintPosition(100);
    g_clsPrint->MW_PrintString("距离行首绝对100abc");  
    g_clsPrint->MW_SetAbsPrintPosition(0);
    g_clsPrint->MW_PrintString("距离行首绝对0abc");   


    // Ascii 下划线
    ucMode = 2;
    g_clsPrint->MW_SetUndlineMode(ucMode);  // 下划线
    g_clsPrint->MW_PrintString("下划线abc123");  
    ucMode = 0;
    g_clsPrint->MW_SetUndlineMode(ucMode);  // 下划线

    // 行间距
    ucMode = 100;
    g_clsPrint->MW_SetLineSpace(ucMode);
    g_clsPrint->MW_PrintString("行间距100adc");   
    g_clsPrint->MW_PrintString("行间距100adc");   
    g_clsPrint->MW_SetDefaultLineSpace();

    // 加粗
    ucMode = 1;
    g_clsPrint->MW_SetBoldMode(ucMode);
    g_clsPrint->MW_PrintString("加粗abc123");   
    ucMode = 0;
    g_clsPrint->MW_SetBoldMode(ucMode);

    // 选择 ASCII 字体
    ucMode = 1;
    g_clsPrint->MW_SelectAsciiFont(ucMode);
    g_clsPrint->MW_PrintString("Compress Ascii 9x17"); 
    ucMode = 0;
    g_clsPrint->MW_SelectAsciiFont(ucMode);
    g_clsPrint->MW_PrintString("Stander Ascii 12x24"); 

    // 对齐方式
    ucMode = 2;
    g_clsPrint->MW_SelectAlignMode(ucMode);
    g_clsPrint->MW_PrintString("右对齐adc123");
    ucMode = 1;
    g_clsPrint->MW_SelectAlignMode(ucMode);
    g_clsPrint->MW_PrintString("中间对齐adc123");
    ucMode = 0;
    g_clsPrint->MW_SelectAlignMode(ucMode);
    g_clsPrint->MW_PrintString("左对齐adc123");

    // 选择字符大小
    ucMode = (3 << 4) | (3);
    g_clsPrint->MW_SelectFontSize(ucMode);
    g_clsPrint->MW_PrintString("2倍高2倍宽adc123");
    ucMode = (2 << 4) | (2);
    g_clsPrint->MW_SelectFontSize(ucMode);
    g_clsPrint->MW_PrintString("2倍高2倍宽adc123");
    ucMode = (1 << 4) | (1);
    g_clsPrint->MW_SelectFontSize(ucMode);
    g_clsPrint->MW_PrintString("2倍高2倍宽adc123");
    ucMode = 0;
    g_clsPrint->MW_SelectFontSize(ucMode);
    g_clsPrint->MW_PrintString("0倍高0倍宽adc123");

    //黑白反显
    ucMode = 1;
    g_clsPrint->MW_SelectBlackAndWhiteReverse(ucMode);
    g_clsPrint->MW_PrintString("黑白反显abc123");
    ucMode = 0;
    g_clsPrint->MW_SelectBlackAndWhiteReverse(ucMode);
    g_clsPrint->MW_PrintString("黑白反显取消abc123");

    // 汉字下划线
    ucMode = 2;
    g_clsPrint->MW_ShowHanZiUndline(ucMode);
    g_clsPrint->MW_PrintString("汉字下划线adc123");
    ucMode = 0;
    g_clsPrint->MW_ShowHanZiUndline(ucMode);
    g_clsPrint->MW_PrintString("取消汉字下划线abc123");

    
    // 汉字左右间距
    unsigned char ucLeft,ucRight;
    ucLeft = 100;
    ucRight = 100;
    g_clsPrint->MW_SetHanZiLeftAndRightSpace(ucLeft, ucRight);
    g_clsPrint->MW_PrintString("汉字左100右100abc");
    ucLeft = ucRight = 0;
    g_clsPrint->MW_SetHanZiLeftAndRightSpace(ucLeft, ucRight);
    g_clsPrint->MW_PrintString("汉字左0右0abc");


    // 汉字倍高倍宽
    ucMode = 1;
    g_clsPrint->MW_SetHanZiAsTwiceHeightAndWidth(ucMode);
    g_clsPrint->MW_PrintString("汉字倍高倍宽abc123");
    ucMode = 0;
    g_clsPrint->MW_SetHanZiAsTwiceHeightAndWidth(ucMode);
    g_clsPrint->MW_PrintString("汉字取消倍高倍宽abc123");
    



    // 条码测试
    // UPC-A
    ucMode = 0;
    g_clsPrint->MW_PrintBarcode(ucMode, "01234567891");
    // UPC-E
    ucMode = 1;
    g_clsPrint->MW_PrintBarcode(ucMode, "012345");
    // JAN13/EAN13
    ucMode = 2;
    g_clsPrint->MW_PrintBarcode(ucMode, "012345678912");
    // JAN8/EAN8
    ucMode = 3;
    g_clsPrint->MW_PrintBarcode(ucMode, "0123456");
    // CODE39
    ucMode = 4;
    g_clsPrint->MW_PrintBarcode(ucMode, "0123ABC");
    // ITF
    ucMode = 5;
    g_clsPrint->MW_PrintBarcode(ucMode, "012345");
    // CODEBAR
    ucMode = 6;
    g_clsPrint->MW_PrintBarcode(ucMode, "A0123456A");

    // UPC-A
    ucMode = 65;
    g_clsPrint->MW_PrintBarcode(ucMode, "01234567891");
    // UPC-E
    ucMode = 66;
    g_clsPrint->MW_PrintBarcode(ucMode, "012345");
    // JAN13/EAN13
    ucMode = 67;
    g_clsPrint->MW_PrintBarcode(ucMode, "012345678912");
    // JAN8/EAN8
    ucMode = 68;
    g_clsPrint->MW_PrintBarcode(ucMode, "0123456");
    // CODE39
    ucMode = 69;
    g_clsPrint->MW_PrintBarcode(ucMode, "0123ABC");
    // ITF
    ucMode = 70;
    g_clsPrint->MW_PrintBarcode(ucMode, "012345");
    // CODEBAR
    ucMode = 71;
    g_clsPrint->MW_PrintBarcode(ucMode, "A0123456A");

    // CODE93
    ucMode = 72;
    g_clsPrint->MW_PrintBarcode(ucMode, "123456");
    // CODE128
    ucMode = 73;
    g_clsPrint->MW_PrintBarcode(ucMode, "123456");




//////////////////////////////////////////////////////////
	g_clsPrint->ClosePort();



	return 0;
}
