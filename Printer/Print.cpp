// Print.cpp: implementation of the CPrint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Print.h"
#include <string>
using namespace std;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrint::CPrint()
{

}

CPrint::~CPrint()
{

}
/////////////////////////////////////////////////////////////////////////
// Private Function
/////////////////////////////////////////////////////////////////////////
/**********************************************************************
* 函数名称： GetPrintStatus
* 功能描述： 用于在打印时判断打印机状态
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 无
***********************************************************************/
int CPrint::GetPrintStatus()
{
    char tmp[100];
    unsigned char err = MW_RealTimeStatus(4);
    
    sprintf(tmp, "Printer error Status 0x%X\n", err);
    if (0x12 != err)
    {
        fputs (tmp, stderr); 
        return -1;
    }
    return 0;
}









/////////////////////////////////////////////////////////////////////////
// Public Function
/////////////////////////////////////////////////////////////////////////

/**********************************************************************
* 函数名称： MW_LF
* 功能描述： 换行
* 输入参数： // 输入参数说明，包括每个参数的作用、取值说明及参数间关系
* 输出参数： // 对输出参数的说明。
* 返 回 值： // 函数返回值的说明 
* 其它说明： // 其它说明
***********************************************************************/
void CPrint::MW_LF()
{
	unsigned char cWriteBuf[10];
	cWriteBuf[0] = 0x0A;
	WriteToPort(cWriteBuf, 1);
}

/**********************************************************************
* 函数名称： MW_RealTimeStatus
* 功能描述： 查询打印机实时状态 
* 输入参数： n: 指定所要传送的打印机状态    (1<= n <=4 ) 
				n = 1: 传送打印机状态
				n = 2: 传送脱机状态
				n = 3: 传送错误状态
				n = 4: 传送纸传感器状态 
* 输出参数： 无
* 返 回 值： 根据请求的不同打印机状态，有不同的错误代码
			【n = 1】：打印机状态
				位	0/1		十六进制码		十进制码	功能
				0	0		00				0			固定为0
				1	1		02				2			固定为1
				2	0		00				0			一个或两个钱箱打开
					1		04				4			两个钱箱都关闭
				3	0		00				0			联机
					1		08				8			脱机 
				4	1		10				16			固定为1
				5/6	-		--				--			未定义 
				7	0		00				0			固定为 0
			【n = 2 】：脱机状态
				位	0/1		十六进制码		十进制码	功能
				0	0		00				0			固定为0
				1	1		02				2			固定为1
				2	0		00				0			上盖关
					1		04				4			上盖开
				3	0		00				0			未按走纸键
					1		08				8			按下赴纸键
				4	1		10				16			固定为1
				5	0		00				0			打印机不缺纸
					1		20				32			打印机缺纸
				6	0		00				0			没有出错情况
					1		40				64			有错误情况
				7	0		00				0			固定为 0
			【n = 3 】：错误状态
				位	0/1		十六进制码		十进制码	功能
				0	0		00				0			固定为0
				1	1		02				2			固定为1
				2	-		--				-			未定义
				3	0		00				0			切刀无错误
					1		08				8			切刀有错误
				4	1		10				16			固定为1
				5	0		00				0			无不可恢复错误
					1		20				32			有不可恢复错误
				6	0		00				0			打印头温度和电压正常
					1		40				64			打印头温度或电压超出范围
				7	0		00				0			固定为 0
			【n = 4 】：传送纸状态
				位	0/1		十六进制码		十进制码	功能
				0	0		00				0			固定为0
				1	1		02				2			固定为1
				2/3	0		00				0			有纸
					1		04				4			纸将尽
				4	1		10				16			固定为1
				5/6	0		00				0			有纸
					1		20				32			纸尽
				7	0		00				0			固定为 0
* 其它说明： // 其它说明
***********************************************************************/
unsigned char CPrint::MW_RealTimeStatus(unsigned char n)
{
    if ((n < 1) || (n > 4))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
	unsigned char cWriteBuf[3];
	char cReadBuf[1];
	
	cWriteBuf[0] = 0x10;
	cWriteBuf[1] = 0x04;
	cWriteBuf[2] = n;
	WriteToPort(cWriteBuf, 3);
	ReceiveChar(cReadBuf,1);
	return cReadBuf[0];
}

/**********************************************************************
* 函数名称： MW_PrintBitmap
* 功能描述： 打印位图
* 输入参数： *pcPicAddr 需要打印的图片地址。	
* 输出参数： 无
* 返 回 值： 0 正常，-1 表示出错
* 其它说明：地址中的 \ 需要进行转义，如 D:\\VC_Project\\Demo\\熊猫.bmp
            目前只支持 24 位深的位图，位图宽不能超过 384 像素
***********************************************************************/
int CPrint::MW_PrintBitmap(char *pcPicAddr)
{
    if (GetPrintStatus() != 0)
        return -1;

	FILE * pFile;  
    long lSize;  
    unsigned char * pucBuffer;  
    size_t result;  
      
    /* 若要一个char不漏地读入整个文件，只能采用二进制方式打开 */   
    pFile = fopen ((char*)pcPicAddr, "rb" );  
    //pFile = fopen ("D:\\VC_Project\\Demo\\熊猫.bmp", "rb" );  
    if (pFile==NULL)  
    {  
        fputs ("File error",stderr);  
        return -1;
    }  
  
    /* 获取文件大小 */  
    fseek (pFile , 0 , SEEK_END);  
    lSize = ftell (pFile);  
    rewind (pFile);  
  
    /* 分配内存存储整个文件 */   
    pucBuffer = (unsigned char*) malloc (sizeof(unsigned char)*lSize);  
    if (pucBuffer == NULL)  
    {  
        fputs ("Memory error",stderr);   
        return -1;
    }  
	
    /* 将文件拷贝到buffer中 */  
    result = fread (pucBuffer,1,lSize,pFile);  
    if (result != (size_t)lSize)  
    {  
        fputs ("Reading error",stderr);  
         return -1;
    }  

	/////////////////////////////////////////////////////////////////////////
	BITMAPFILEHEADER *ptBitmap = (BITMAPFILEHEADER *)pucBuffer;
	BITMAPINFOHEADER *ptBitmapH = (BITMAPINFOHEADER *)&pucBuffer[14];
	int iWidth;
	int iHeight;
	int iBMPBpp;
	int iLineWidthAlign;
	int iLineWidthReal;
	int iCurBlue;
	int iCurLine;
	unsigned char ucaData[10] = {0};
	unsigned char *pucSrc; // 指向图片数据

	//设置字符行间距为 0 点行
    unsigned char data[] = { 0x1B, 0x33, 0x00 };
 	WriteToPort(data, sizeof(data));

	//ESC * m nL nH d1…dk   选择位图模式
	// ESC * m nL nH
	unsigned char escTmp[] = { 0x1B, 0x2A, 0x00, 0x00, 0x00 };


	/* 不同模式宽度限制：每行固定像素点为 384
			0x00: 8 点单密度，192 宽的位图
			0x01: 8 点双密度，384 宽的位图 
			0x32: 24 点单密度，192 宽的位图 
			0x33: 24 点双密度，384 宽的位图 
	*/
	escTmp[2] = 0x21;
	
	//nL, nH
	escTmp[3] = (unsigned char)(ptBitmapH->biWidth % 256);
	escTmp[4] = (unsigned char)(ptBitmapH->biWidth / 256);
	
	iWidth = ptBitmapH->biWidth;
	iHeight = ptBitmapH->biHeight;
	iBMPBpp = ptBitmapH->biBitCount;
	
	iLineWidthReal = iWidth * iBMPBpp / 8;
	iLineWidthAlign = (iLineWidthReal + 3) & ~0x3;   // 向 4 取整 
	
	

	pucSrc = pucBuffer + ptBitmap->bfOffBits; // 指向图片左下
    //pucSrc = pucSrc + (iHeight - 1) * iLineWidthAlign; 

//  // 8 点单密度
// 	for (int y = 0; y < (iHeight / 8 + 1); y++)// 循环组，每 8 行一组 + 1 处理最后一组
// 	{	
// 		WriteToPort(escTmp, 5);
// 		for (int x = 0; x < iWidth; x++)// 循环列，每列一字节
// 		{
// 			ucaData[0] = 0x00;
// 			for (int z = 0; z < 8; z++) // 循环每字节
// 			{
// 				//当前像素点标号(从 0 开始) = (z * iWidth + x + y * 8 * iWidth);
// 				iCurPiexl = (z * iWidth + x + y * 8 * iWidth);
// 
// 				// 判断是还在当前位图中 
// 				if (iCurPiexl < iWidth * iHeight)
// 				{
// 					// iCurPiexl / iWidth = 所在行 
// 					// iCurPiexl % iWidth = 所在列 = x
// 					// 进行 4 字节对齐后的像素位置 = 所在行 * 对齐后的列宽 + 所在列 * 3（24 位深）
// 					iCurBlue = iCurPiexl / iWidth * iLineWidthAlign + ( (iWidth - x - 1) * 3); // (iWidth - x - 1) 倒列
// 					if (pucSrc[iCurBlue] != 0xFF ) // 0x00 0x00 0x00 表示黑色，0xFF 0xFF 0xFF 表示白色
// 					{
// 						ucaData[0] |= (1 << (7-z));
// 					}
// 				}	
// 			}
// 			WriteToPort(ucaData, 1);
// 			printf("0x%X ", ucaData[0]);
// 		}
// 		MW_LF();
// 	}

//	for (int y = 0; y < (iHeight / 24 + 1); y++)// 循环组，每 24 行一组 + 1 处理最后一组
    // 图片旋转，处理组也从 左下 0 至 (iHeight / 24 + 1) 变成  (iHeight / 24 + 1) 至0
	for (int y = (iHeight / 24); y >=0 ; y--)// 循环组，每 24 行一组 + 1 处理最后一组
	{	
		WriteToPort(escTmp, 5);
		for (int x = 0; x < iWidth; x++)// 循环列，每列3字节
		{
			ucaData[0] = 0x00;
			ucaData[1] = 0x00;
			ucaData[2] = 0x00;
			for (int z = 0; z < 24; z++) // 循环每列字节位
			{
				//当前像素点标号(从 0 开始) = 第几位 * 每行像素数 + 本行第向个像素 + 第几组 * 组行数 * 每行像素数 =(z * iWidth + x + y * 8 * iWidth);
				//iCurPiexl = (z * iWidth + x + y * 24 * iWidth);

				// 所在行 = 当前第几位 + 当前组 * 组成员个数
				iCurLine = z + y * 24;


				// 判断是还在当前位图中 
				if (iCurLine < iHeight)
				{
					// iCurPiexl / iWidth = 所在行 = 当前第几位 + 当前组 * 组成员个数
					// iCurPiexl % iWidth = 所在列 = x

					// 进行 4 字节对齐后的像素位置 = 所在行 * 对齐后的列宽 + 所在列 * 3（24 位深）
					iCurBlue = iCurLine * iLineWidthAlign + ( x * 3); // (iWidth - x - 1) 行数据颠倒
					if (pucSrc[iCurBlue] != 0xFF ) // 0x00 0x00 0x00 表示黑色，0xFF 0xFF 0xFF 表示白色
					{
//                         if ( z < 8)
//                         {
//                             ucaData[0] |= (1 << (7-z));
//                         }
//                         else
//                             if ( (z >= 8) && (z < 16) )
//                         {
//                             ucaData[1] |= (1 << (15-z));
//                         }
//                         else
//                             if((z >= 16) && (z < 24))
//                         {
//                             ucaData[2] |= (1 << (23-z));
//                         }

                        // 图片旋转, 原来是 ucaData[0-2] = 0 - 24 位，现在要变成 ucaData[0-2] = 24-0
                        if ( z < 8)
                        {
                            ucaData[2] |= (1 << (z));
                        }
                        else
                            if ( (z >= 8) && (z < 16) )
                        {
                            ucaData[1] |= (1 << (z - 8));
                        }
                        else
                            if((z >= 16) && (z < 24))
                        {
                            ucaData[0] |= (1 << (z - 16));
                        }
						
						
					}
				}	
			}
                       
			WriteToPort(ucaData, 3);
		}
		MW_LF();
	}


	/////////////////////////////////////////////////////////////////////////
    /* 结束演示，关闭文件并释放内存 */  
    fclose (pFile);  
    free (pucBuffer);  
   return 0; 
	

}

/**********************************************************************
* 函数名称： MW_SetCharRightSpace
* 功能描述： 设置打印字符右边距
* 输入参数： n 表示右边距为 [ n * 横向移动单位或纵向移动单位]英寸 (0 <= n <= 255)
* 输出参数： 无
* 返 回 值： 无
* 其它说明： 默认值为 0
***********************************************************************/
int CPrint::MW_SetCharRightSpace(unsigned char n)
{
    if ((n < 0) || (n > 255))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
	unsigned char cWriteBuf[10];
	cWriteBuf[0] = 0x1B;
	cWriteBuf[1] = 0x20;
	cWriteBuf[2] = n;
	WriteToPort(cWriteBuf, 3);
	return 0;
}	


/**********************************************************************
* 函数名称： MW_PrintString
* 功能描述： 需要打印的字符串
* 输入参数： *pcString 打向需要打印的字符串数据	
* 输出参数： 无
* 返 回 值： 0 成功，失败返回 -1
* 其它说明： 字符以换行结束才会被打印出来，对于不是以换行结尾的字符串，
                程序会自动添加换行打印出来。
***********************************************************************/
int CPrint::MW_PrintString(char *pcString)
{
    if (GetPrintStatus() != 0)
        return -1;
	int iByteSent;

	iByteSent = WriteToPort((unsigned char*)pcString, strlen(pcString));
    if (pcString[iByteSent-1] != 0x0a)
    {
        MW_LF();
    }
	return -((size_t)iByteSent != strlen(pcString));
}

/**********************************************************************
* 函数名称： MW_SelectPrintMode
* 功能描述： 选择打印模式
* 输入参数： ucMode 打印模式                   0 <= ucmode <= 255
                位  1/0     十六进制码  十进制码    功能
                0   0       00          0           标准 ASCII 码字体(12x24)
                    1       01          1           压缩 ASCII 码字体(9x17)
                1/2 -       --          -           未定义
                3   0       00          0           取消加粗模式 
                    1       08          8           选择加粗模式
                4   0       00          0           取消倍高模式
                    1       10          16          选择倍高模式
                5   0       00          0           取消倍宽模式
                    1       20          32          选择倍宽模式
                6   -       --          --          --
                7   0       00          0           取消下划线模式
                    1       80          128         选择下划线模式

* 输出参数： 无
* 返 回 值： 0 成功，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SelectPrintMode(unsigned char ucMode)
{
    if ((ucMode < 0) || (ucMode > 255))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x21;
    cWriteBuf[2] = ucMode;
    WriteToPort(cWriteBuf, 3);
	return 0;
}

/**********************************************************************
* 函数名称： MW_SetAbsPrintPosition
* 功能描述： 设置绝对打印位置
* 输入参数： iPost 当前位置距离行首距离 (0 <= iPost <=65535)
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SetAbsPrintPosition(unsigned int iPost)
{
    unsigned char nL,nH;
    nL = iPost % 256;
    nH = iPost / 256;

    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x24;
    cWriteBuf[2] = nL;
    cWriteBuf[3] = nH;
    WriteToPort(cWriteBuf, 4);
	return 0;
}

/**********************************************************************
* 函数名称： MW_SetUndlineMode
* 功能描述： 选择/取消下划线模式
* 输入参数： ucMode 选择/取消下划线模式 
                    值              功能 
                    0            取消下划线模式
                    1            选择下划线模式(1 点宽)
                    2            选择下划线模式(2 点宽)
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SetUndlineMode(unsigned char ucMode)
{
    if ((ucMode < 0) || (ucMode > 2) )
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x2D;
    cWriteBuf[2] = ucMode;
    WriteToPort(cWriteBuf, 3);
	return 0;
}

/**********************************************************************
* 函数名称： MW_SetDefaultLineSpace
* 功能描述： 设置默认行间距
* 输入参数： 选择约 3.75mm 行间距
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SetDefaultLineSpace(void)
{
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x32;
    WriteToPort(cWriteBuf, 2);
	return 0;
}

/**********************************************************************
* 函数名称： MW_SetLineSpace
* 功能描述： 设置行间距
* 输入参数： 设置行间距为 n x 纵向或横向移动单位 英寸   (0 <= n <= 255)
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SetLineSpace(unsigned char n)
{
    if ((n < 0) || (n > 255))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x33;
    cWriteBuf[2] = n;
    WriteToPort(cWriteBuf, 3);
	return 0;
}

/**********************************************************************
* 函数名称： MW_InitPrint
* 功能描述： 实始化打印机，清除打印机缓冲区数据，打印模式被设为上电时的默认值模式
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_InitPrint()
{
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x40;
    WriteToPort(cWriteBuf, 2);
	return 0;
}


/**********************************************************************
* 函数名称： MW_SetBoldMode
* 功能描述： 选择/取消加粗模式
* 输入参数： n， 0 取消加粗模式，
                 1 选择加粗模式
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SetBoldMode(unsigned char n)
{
    if ((n < 0) || (n > 1))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x45;
    cWriteBuf[2] = n;
    WriteToPort(cWriteBuf, 3);
	return 0;
}

/**********************************************************************
* 函数名称： MW_SelecAsciiFont
* 功能描述： 选择字体 
* 输入参数： n， 0 选择标准 ASCII 码字体(12x24)
                 1 选择压缩 ASCII 码字体(9x17)
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SelectAsciiFont(unsigned char n)
{
    if ((n < 0) || (n > 1))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x4D;
    cWriteBuf[2] = n;
    WriteToPort(cWriteBuf, 3);
	return 0;
}
// 
// /**********************************************************************
// * 函数名称： MW_SetRefPrintPosition
// * 功能描述： 设置相对横向打印位置
// * 输入参数： iPost 距离当前位置距离 -65535 < iPost < 65535
// * 输出参数： 无
// * 返 回 值： 成功返回 0 ，失败返回 -1
// * 其它说明： 
// ***********************************************************************/
// int CPrint::MW_SetRefPrintPosition(int iPost)
// {
//     unsigned char nL,nH;
//     if (iPost < 0)
//     {
//         iPost = 65536 + iPost;
// 
//     }
//     nL = iPost % 256;
//     nH = iPost / 256;
//     
//     unsigned char cWriteBuf[10];
//     cWriteBuf[0] = 0x1B;
//     cWriteBuf[1] = 0x5C;
//     cWriteBuf[2] = nL;
//     cWriteBuf[3] = nH;
//     WriteToPort(cWriteBuf, 4);
// 	return 0;
// }

/**********************************************************************
* 函数名称： MW_SelectAlignMode
* 功能描述： 设置对齐方式
* 输入参数： n: 0    左对齐
                1    中间对齐
                2    右对齐 

* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SelectAlignMode(unsigned char n)
{
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x61;
    cWriteBuf[2] = n;
    WriteToPort(cWriteBuf, 3);
	return 0;
}

/**********************************************************************
* 函数名称： MW_DownloadBitmapToFlash
* 功能描述： 下载位图到 Flash 中
* 输入参数： *pcBitmapAddr 为位图的路径
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 打印机自带 16K 的存储。
             目前只支持 24 位深的位图，且宽，高均要是 8 的倍数！！
             位图宽满足 Width <= 376, 
             位图高满足 Width * Height  < 128K
             【注：开头3行左右会重叠打印在一行，建议留白】
***********************************************************************/
int CPrint::MW_DownloadBitmapToFlash(char *pcBitmapAddr)
{

	FILE * pFile;  
    long lSize;  
    unsigned char * pucBuffer;  
    size_t result;  
      
    /* 若要一个char不漏地读入整个文件，只能采用二进制方式打开 */   
    pFile = fopen (pcBitmapAddr, "rb" );  
    //pFile = fopen ("D:\\VC_Project\\Demo\\熊猫.bmp", "rb" );  
    if (pFile==NULL)  
    {  
        fputs ("File error",stderr);  
        return -1;
    }  
  
    /* 获取文件大小 */  
    fseek (pFile , 0 , SEEK_END);  
    lSize = ftell (pFile);  
    rewind (pFile);  
  
    /* 分配内存存储整个文件 */   
    pucBuffer = (unsigned char*) malloc (sizeof(unsigned char)*lSize);  
    if (pucBuffer == NULL)  
    {  
        fputs ("Memory error",stderr);   
        return -1;
    }  
	
    /* 将文件拷贝到buffer中 */  
    result = fread (pucBuffer,1,lSize,pFile);  
    if (result != (size_t)lSize)  
    {  
        fputs ("Reading error",stderr);  
         return -1;
    }  

	/////////////////////////////////////////////////////////////////////////
	BITMAPFILEHEADER *ptBitmap = (BITMAPFILEHEADER *)pucBuffer;
	BITMAPINFOHEADER *ptBitmapH = (BITMAPINFOHEADER *)&pucBuffer[14];
	int iWidth;
	int iHeight;
	int iBMPBpp;        // bitmap 位深
	int iLineWidthAlign;// bitmap 每行对齐后的字节数
	int iLineWidthReal; // bitmap 每行实际字节数
    int iFlashDataNum; // Flash 图片缓存区的字节数
	int iCurBlue;  // 当前 RGB 中的蓝色值 
    int iCurFlashByte;  // 当前 Flash 字节位置
    int iColBytesNum;   // 每列字节数
	unsigned char ucaData[10] = {0};
	unsigned char *pucSrc; // 指向图片数据
    unsigned char * pucFlashData; // 指向 Flash 图片缓存区

	// FS q m xL xH yL yH d1…dk   定义 Flash 位图
	// k = (xL + xH * 256) * (yL + yH * 256) * 8 
	unsigned char escTmp[] = { 0x1C, 0x71, 0x01, 0x00, 0x00, 0x00, 0x00 };
    
	//xL, xH
    long tmpX = (ptBitmapH->biWidth + 7) / 8; 
	escTmp[3] = (unsigned char)(tmpX % 256);
	escTmp[4] = (unsigned char)(tmpX / 256);
	
    //yL, yH
    long tmpY = (ptBitmapH->biHeight + 7) / 8; 
    escTmp[5] = (unsigned char)(tmpY % 256);
	escTmp[6] = (unsigned char)(tmpY / 256);

    iFlashDataNum = (tmpX * tmpY * 8);
    pucFlashData = (unsigned char*) calloc (iFlashDataNum, sizeof(unsigned char));  

	iWidth = ptBitmapH->biWidth;
	iHeight = ptBitmapH->biHeight;
	iBMPBpp = ptBitmapH->biBitCount;
	
    // 目前图片只支持行，列是 8 的倍数的图片
    if ( ((iWidth % 8) != 0) || ((iHeight % 8) != 0))
    {
        fputs ("Bitmap iWidth or iHeight can not divide by 8 !",stderr); 
        return -1;
    }

	iLineWidthReal = iWidth * iBMPBpp / 8;
	iLineWidthAlign = (iLineWidthReal + 3) & ~0x3;   // 向 4 取整 
	
	

	pucSrc = pucBuffer + ptBitmap->bfOffBits; // 指向图片左下, 即资源开始的地方 
    //pucSrc = pucSrc + (iHeight - 1) * iLineWidthAlign; 
    

    iColBytesNum = (iHeight + 7) / 8; 

    for (int x = 0; x < iWidth; x++) // 第几列
    {
        //for (int y = 0; y < iColBytesNum; y++) // 当前列第几个字节    未颠倒
        for (int y = iColBytesNum; y > 0; y--) // 当前列第几个字节
        {
            // Flash 中当前字节 = x * (iHeight / 8 + 1) + y = 当前列 * 每列字节数 + 当前所在列第几个字节
            iCurFlashByte = x * iColBytesNum + y - 1;

            for (int z = 0; z < 8; z++) // 当前字节内第几位
            {
                // 当前字节位置的颜色 = 对应的图像字节颜色
                
                // 当前 Flash 点位位置 = z + y * 8 + x * (iHeight / 8 + 1) * 8 = 当前字节内位置 + 前面列位数 + 前面行位数

                // 进行 4 字节对齐后的像素位置 = 所在行 * 对齐后的列宽 + 所在列 * 3（24 位深）
                // 当前图像中对应蓝色像素字节位置 
                //iCurBlue = (z + y * 8) * iLineWidthAlign + x * 3;             // 未颠倒
                iCurBlue = ((iHeight - 1) - ((7 - z) + (y -1) * 8)) * iLineWidthAlign + x * 3; 

                if (pucSrc[iCurBlue] != 0xFF ) // 0x00 0x00 0x00 表示黑色，0xFF 0xFF 0xFF 表示白色
                {
                    //pucFlashData[iCurFlashByte] |= (1 << (7 - z));            // 未颠倒
                    pucFlashData[iCurFlashByte] |= (1 << z);
                }
               

            }
        }
    }
    
//     for(int i = 0; i < iFlashDataNum; i++)
//     {
//         printf("0x%02X ", pucFlashData[i]);
//         if ((i % 16) == 15)   // 0-7 8-15 
//         {
//             printf("\n");
//         }
//     }
//     printf("\n");


//   printf("tmpX = %d tmpY = %d iFlashDataNum = %d\n",tmpX, tmpY, iFlashDataNum);

    WriteToPort(escTmp, 7);
    WriteToPort(pucFlashData, iFlashDataNum);
    Sleep(10000);

    free(pucFlashData);
	/////////////////////////////////////////////////////////////////////////
    /* 结束演示，关闭文件并释放内存 */  
    fclose (pFile);  
    free (pucBuffer);  
    return 0; 
}
/**********************************************************************
* 函数名称： MW_PrintFlashBitmap
* 功能描述： 打印 Flash 中的位图
* 输入参数： n 为打印模式             (0 <= n <= 3)
                            n   模式 
                            0   正常
                            1   倍宽
                            2   倍高
                            3   倍宽、倍高 
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_PrintFlashBitmap(unsigned char n)
{
    if ((n < 0) || (n > 3))
    {
        fputs ("invalid argument!\n",stderr);
		return -1;
    }
    if (GetPrintStatus() != 0)
        return -1;
	unsigned char escTmp[] = { 0x1C, 0x71, 0x01, 0x00, 0x00, 0x00, 0x00 };
    escTmp[0] = 0x1C;
    escTmp[1] = 0x70;
    escTmp[2] = 0x01;
    escTmp[3] = 0x00;
    WriteToPort(escTmp, 4);
    return 0;

}

/**********************************************************************
* 函数名称： MW_selectFontSize
* 功能描述： 选择字体大小，用 0 到 2 位选择字符高度，4 到 7 位选择字符宽度
* 输入参数： n 字体大小
                位      功能
                0-3     字符高度选择
                4-7     字符宽度选择 
             高度、宽度的范围都是 0-3，表示与正常显示的倍数关系 
    
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SelectFontSize(unsigned char n)
{
    unsigned char nL,nH;
    nL = n % 16;
    nH = n / 16;
    if ((nL < 0) || (nL > 3) || (nH < 0) || (nH > 3) )
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }

    unsigned char escTmp[10];
    escTmp[0] = 0x1D;
    escTmp[1] = 0x21;
    escTmp[2] = n;
    WriteToPort(escTmp, 3);
    return 0;    
}
/**********************************************************************
* 函数名称：MW_SelectBlackAndWhiteReverse
* 功能描述：选择/取消黑白反显打印模式
* 输入参数：n 用于反显选择：
              当 n 的最低位为 0 时，取消反显打印
              当 n 的最低位为 1 时，选择反显打印
    
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SelectBlackAndWhiteReverse(unsigned char n)
{
    unsigned char escTmp[10];
    escTmp[0] = 0x1D;
    escTmp[1] = 0x42;
    escTmp[2] = n;
    WriteToPort(escTmp, 3);
    return 0; 
}

/**********************************************************************
* 函数名称：MW_SetHriLocation
* 功能描述：设置条码字符打印位置
* 输入参数：n 用于位置选择：
                0       不打印
                1       条码上方
                2       条码下方
                3       条码上、下方都打印 
    
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： HRI 字符是对条码内容注释的字符
***********************************************************************/
int CPrint::MW_SetHriLocation(unsigned char n)
{
    if ((n < 0) || (n > 3))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    unsigned char escTmp[10];
    escTmp[0] = 0x1D;
    escTmp[1] = 0x48;
    escTmp[2] = n;
    WriteToPort(escTmp, 3);
    return 0; 
}

/**********************************************************************
* 函数名称：MW_SetPrintLeftMargin
* 功能描述：设置打印左边距
* 输入参数：n 为左边距距离，单位为英寸    
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SetPrintLeftMargin(unsigned int n)
{
    unsigned char nL,nH;
    nL = n % 256;
    nH = n / 256;
    unsigned char escTmp[10];
    escTmp[0] = 0x1D;
    escTmp[1] = 0x48;
    escTmp[2] = nL;
    escTmp[3] = nH;
    WriteToPort(escTmp, 4);
    return 0; 


}


/**********************************************************************
* 函数名称： MW_SetPrintWidth
* 功能描述： 设置打印宽度
* 输入参数： n 为打印宽度，单位为英寸 
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SetPrintWidth(unsigned int n)
{
    unsigned char nL,nH;
    nL = n % 256;
    nH = n / 256;
    unsigned char escTmp[10];
    escTmp[0] = 0x1D;
    escTmp[1] = 0x57;
    escTmp[2] = nL;
    escTmp[3] = nH;
    WriteToPort(escTmp, 4);
    return 0; 
   
}

/**********************************************************************
* 函数名称： MW_SetHriFont
* 功能描述： 设置条码 HRI 字体
* 输入参数： n 用于字体选择：
                0   标准 ASCII 码字符(12x24)
                1   压缩 ASCII 码字符(9x17)
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： HRI 字符是对条码内容注释的字符
***********************************************************************/
int CPrint::MW_SetHriFont(unsigned char n)
{
    if ((n < 0) || (n > 1))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    unsigned char escTmp[10];
    escTmp[0] = 0x1D;
    escTmp[1] = 0x66;
    escTmp[2] = n;
    WriteToPort(escTmp, 3);
    return 0; 
    
}

/**********************************************************************
* 函数名称： MW_SetBarcodeHeight
* 功能描述： 设置条码高度
* 输入参数： n 为条码高度，单位点【默认值 162】
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SetBarcodeHeight(unsigned char n)
{
    if ((n < 0) || (n > 255))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    unsigned char escTmp[10];
    escTmp[0] = 0x1D;
    escTmp[1] = 0x68;
    escTmp[2] = n;
    WriteToPort(escTmp, 3);
    return 0; 
    
}

/**********************************************************************
* 函数名称： MW_PrintBarcode
* 功能描述： 打印条码
* 输入参数： ucMode(十进制) 要打印的条码格式，*pcData指向条码数据
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
---------------------------------------------------------------------------------------
| 模式(十进制)| 条码类型       | 字符个数    | 字符                                   |
---------------------------------------------------------------------------------------
| 数据格式一：1D 6B m xx xx 00                                                        |
---------------------------------------------------------------------------------------
| 0(0x00)     | UPC-A          | 11          | 0-9                                    |
---------------------------------------------------------------------------------------
| 1(0x01)     | UPC-E          | 6           | 0-9                                    |
---------------------------------------------------------------------------------------
| 2(0x02)     | JAN13/EAN13    | 12          | 0-9                                    |
---------------------------------------------------------------------------------------
| 3(0x03)     | JAN8/EAN8      | 7           | 0-9                                    |
---------------------------------------------------------------------------------------
| 4(0x04)     | CODE39         | 1-255       | 0-9 A-Z SP,$,%,+,-,.,/, *(开始结束字符)|
---------------------------------------------------------------------------------------
| 5(0x05)     | ITF            | 1-255(偶数) | 0-9                                    |
---------------------------------------------------------------------------------------
| 6(0x06)     | CODEBAR        | 1-255       | 0-9 A-D $,+,-,.,/,                     |
---------------------------------------------------------------------------------------
| 数据格式二：1D 6B m n xx xx                                                         |
---------------------------------------------------------------------------------------
| 65(0x41)    | UPC-A          | 11          | 0-9                                    |
---------------------------------------------------------------------------------------
| 66(0x42)    | UPC-E          | 6           | 0-9                                    |
---------------------------------------------------------------------------------------
| 67(0x43)    | JAN13/EAN13    | 12          | 0-9                                    |
---------------------------------------------------------------------------------------
| 68(0x44)    | JAN8/EAN8      | 7           | 0-9                                    |
---------------------------------------------------------------------------------------
| 69(0x45)    | CODE39         | 1-255       | 0-9 A-Z SP,$,%,+,-,.,/, *(开始结束字符)|
---------------------------------------------------------------------------------------
| 70(0x46)    | ITF            | 1-255(偶数) | 0-9                                    |
---------------------------------------------------------------------------------------
| 71(0x47)    | CODEBAR        | 1-255       | 0-9 A-D $,+,-,.,/,                     |
---------------------------------------------------------------------------------------
| 72(0x48)    | CODE93         | 1-255       | NUL-SP(7FH)                            |
---------------------------------------------------------------------------------------
| 73(0x49)    | CODE128        | 2-255       | NUL-SP(7FH)                            |
---------------------------------------------------------------------------------------
***********************************************************************/
int CPrint::MW_PrintBarcode(unsigned char ucMode, char *pcData)
{

    /*
     Code39: 0123456789
        1D 68 78        // 选择条码高度
        1D 77 02        // 设置条码宽度
        1D 48 02        // 选择 HRI 字符的打印位置，条码下方
        0A 
        1D 6B 45 0A 30 31 32 33 34 35 36 37 38 39 0A 

     Code128:0123456789
        1D 68 78
        1D 77 02 
        1D 48 02 
        0a 
        1D 6B 49 0C 7B 41 30 31 32 33 34 35 36 37 38 39 0A

    //////////////////////////////////////////////////////////
    // 测试命令：
        1D 6B 00 30 31 32 33 34 35 36 37 38 39 39 00
    */
    unsigned char escTmp[270];
    int iStrLength = strlen(pcData);

    if (GetPrintStatus() != 0)
        return -1;

    if ((ucMode < 0) || ((ucMode > 6) && (ucMode < 65 )) || (ucMode > 73 ))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    if (iStrLength > 255)
    {
        fputs ("invalid string length > 255!\n",stderr);
        return -1;
    }


    escTmp[0] = 0x1D;
    escTmp[1] = 0x6B;


    switch(ucMode)
    {
    // 第一种，不带长度发送
    case 0:
        strcpy((char *)escTmp+3, pcData);
        escTmp[2] = 0x00;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 1:
        strcpy((char *)escTmp+3, pcData);
        escTmp[2] = 0x01;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 2:
        strcpy((char *)escTmp+3, pcData);
        escTmp[2] = 0x02;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 3:
        strcpy((char *)escTmp+3, pcData);
        escTmp[2] = 0x03;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 4:
        strcpy((char *)escTmp+3, pcData);
        escTmp[2] = 0x04;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 5:
        strcpy((char *)escTmp+3, pcData);
        escTmp[2] = 0x05;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 6:
        strcpy((char *)escTmp+3, pcData);
        escTmp[2] = 0x06;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    // 第二种格式：带长度发送
    case 65:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x41;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 66:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x42;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 67:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x43;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 68:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x44;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 69:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x45;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 70:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x46;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 71:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x47;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 72:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x48;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 73:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x49;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    default:
        fputs ("invalid argument!\n",stderr);
        return -1;
    }

    return 0; 
    
}


// *********************************************************************
// * 函数名称： MW_GetPrinterStatus
// * 功能描述： 获得打印机纸以及钱箱状态
// * 输入参数： n 为条码高度，单位点【默认值 162】
// * 输出参数： 无
// * 返 回 值： 返回值是纸传感器状态或钱箱状态
// * 其它说明： 返回值是纸传感器状态或钱箱状态
//             【n = 1】：纸传感器状态
//                 位	0/1		十六进制码		十进制码	状态
//                 0/1	0		00				0		    纸将尽传感器，有纸
//                 	1		03				3			纸将尽传感器，纸将尽
//                 2/3 0       00              0           纸尽传感器，有纸
//                     1       0c              12          纸尽传感器，缺纸
//                 4   0       00              0           不用，固定为 0
//                 5/6 -       --              --          未定义
//                 7   0       00              0           不用，固定为 0
//             【n = 2 】：钱箱状态
//                 位	0/1		十六进制码		十进制码	功能
//                 0	0		00				0			有钱箱打开
//                     1       01              1           无钱箱打开
//                 1-3 -       --              --          未定义
//                 4   0       00              0           不用，固定为 0
//                 5/6 -       --              --          未定义
//                 7   0       00              0           不用，固定为 0
//                 
// **********************************************************************
// int CPrint::MW_GetPrinterStatus(unsigned char n)
// {
//     if ((n < 1) || (n > 2))
//     {
//         fputs ("invalid argument!\n",stderr);
//         return -1;
//     }
//     unsigned char escTmp[10];
//     escTmp[0] = 0x1D;
//     escTmp[1] = 0x72;
//     escTmp[2] = n;
//     WriteToPort(escTmp, 3);
//     return 0; 
//     
// }

/**********************************************************************
* 函数名称： MW_SetBarcodeWidth
* 功能描述： 设置条码宽度
* 输入参数： n 为条码宽度
                n   单基本模块宽度(毫米)    双基本模块宽度
                                            窄(毫米)    宽(毫米)
                2   0.25                    0.25        0.625
                3   0.375                   0.375       1.0
                4   0.5                     0.5         1.25
                5   0.625                   0.625       1.625
                6   0.75                    0.75        1.875
            
              单基本模块条码如下：
                UPC-A,UPC-E,JAN13(EAN13),JAN8(EAN8),CODE93,CODE128
              双基本模块条码如下：
                CODE39,ITF,CODABAR 
            默认值：3
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SetBarcodeWidth(unsigned char n)
{
    if ((n < 2) || (n > 6))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    unsigned char escTmp[10];
    escTmp[0] = 0x1D;
    escTmp[1] = 0x77;
    escTmp[2] = n;
    WriteToPort(escTmp, 3);
    return 0; 
    
}

/**********************************************************************
* 函数名称： MW_SetHanZiMode
* 功能描述： 设置汉字的倍宽、倍高与下划线
* 输入参数： n 为要设置的项
                位	0/1		十六进制码		十进制码	状态
                0/1 -       --              --          未定义
                2   0       00              0           取消倍宽
                    1       04              4           选择倍宽
                3   0       00              0           取消倍高
                    1       08              8           选择倍高
                4-6 -       --              -           未定义
                7   0       00              0           取消下划线
                    1       80              128         选择下划线
           
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SetHanZiMode(unsigned char n)
{
    if ((n < 0) || (n > 255))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    unsigned char escTmp[10];
    escTmp[0] = 0x1C;
    escTmp[1] = 0x21;
    escTmp[2] = n;
    WriteToPort(escTmp, 3);
    return 0; 
    
}


/**********************************************************************
* 函数名称： MW_ShowHanZiUndline
* 功能描述： 选择/取消汉字下划线模式
* 输入参数： 根据 n 值，选择或取消汉字的下划线
                n           功能
                0           取消汉字下划线
                1           选择汉字下划线(1点宽)
                2           选择汉字下划线(2点宽)
           
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_ShowHanZiUndline(unsigned char n)
{
    if ((n < 0) || (n > 2))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    unsigned char escTmp[10];
    escTmp[0] = 0x1C;
    escTmp[1] = 0x2D;
    escTmp[2] = n;
    WriteToPort(escTmp, 3);
    return 0; 
    
}

/**********************************************************************
* 函数名称： MW_SetHanZiLeftAndRightSpace
* 功能描述： 设置汉字字左右间距
* 输入参数： 分别将汉字的左间距和右间距设置为 ucLeft,ucRight，单位为 英寸
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 汉字最大间距为 36 mm,默认间距为 0.
***********************************************************************/
int CPrint::MW_SetHanZiLeftAndRightSpace(unsigned char ucLeft, unsigned char ucRight)
{
    
    unsigned char escTmp[10];
    escTmp[0] = 0x1C;
    escTmp[1] = 0x53;
    escTmp[2] = ucLeft;
    escTmp[3] = ucRight;
    WriteToPort(escTmp, 4);
    return 0; 
    
}

/**********************************************************************
* 函数名称： MW_SetHanZiAsTwiceHeightAndWidth
* 功能描述： 选择/取消汉字倍高倍宽
* 输入参数： 当 n 的最低位为 0，取消汉字倍高倍宽模式
             当 n 的最低位为 1，选择汉字倍高倍宽模式
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SetHanZiAsTwiceHeightAndWidth(unsigned char n)
{
    
    unsigned char escTmp[10];
    escTmp[0] = 0x1C;
    escTmp[1] = 0x57;
    escTmp[2] = n;
    WriteToPort(escTmp, 3);
    return 0; 
    
}

/**********************************************************************
* 函数名称： MW_SetPrinterAgruments
* 功能描述： 设置打印机参数，并保存 FLASH 中，下次开机生效
* 输入参数： n 设置项选择，    m 选择设置
                指令    n                   说明
                1B 4E   00                  恢复出厂设置
                1B 4E   02  m               设置串口波特率(默认 m=5, 波特率 115200)
                                                m=1: 波特率 9600    m=2: 波特率 19200
                                                m=3: 波特率 38400   m=4: 波特率 57600
                1B 4E   03  m               设置打印模式(中文，西方)(默认m=1中文)
                                                m=0: 英文模式
                1B 4E   04  m               设置打印浓度级别(默认 m=3, Level3)
                                                m=1: 打印浓度级别 1
                                                m=2: 打印浓度级别 2
                                                m=3: 打印浓度级别 3
                                                m=4: 打印浓度级别 4
                1B 4E   05  m               设置代码页(默认 m=0 代码页 0)
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SetPrinterAgruments(unsigned char n, unsigned char m)
{
    unsigned char escTmp[10];

    escTmp[0] = 0x1B;
    escTmp[1] = 0x4E;
    escTmp[2] = n;
    escTmp[3] = m;

    if ((n < 0) || (n > 5) || (n == 1))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    if ((n < 0) || (n > 4))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }

    switch(n)
    {
    case 0:
        WriteToPort(escTmp, 3);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        WriteToPort(escTmp, 4);
        break;
    default:
        fputs ("invalid argument!\n",stderr);
        return -1;
    }

    return 0; 
    
}