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
* 函数名称： MW_LF
* 功能描述： 查询打印机实时状态 
* 输入参数： n: 指定所要传送的打印机状态    (1<= n <=4 ) 
				n = 1: 传送打印机状态
				n = 2: 传送脱机状态
				n = 3: 传送错误状态
				n = 4: 传送纸传感器状态 
* 输出参数： // 对输出参数的说明。
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
	unsigned char cWriteBuf[3];
	char cReadBuf[1];
	if((n > 4)||(n < 1))
		return -1;
	
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
***********************************************************************/
int CPrint::MW_PrintBitmap(char *pcPicAddr)
{
 
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
	unsigned char escBmp[] = { 0x1B, 0x2A, 0x00, 0x00, 0x00 };


	/* 不同模式宽度限制：每行固定像素点为 384
			0x00: 8 点单密度，192 宽的位图
			0x01: 8 点双密度，384 宽的位图 
			0x32: 24 点单密度，192 宽的位图 
			0x33: 24 点双密度，384 宽的位图 
	*/
	escBmp[2] = 0x21;
	
	//nL, nH
	escBmp[3] = (unsigned char)(ptBitmapH->biWidth % 256);
	escBmp[4] = (unsigned char)(ptBitmapH->biWidth / 256);
	
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
// 		WriteToPort(escBmp, 5);
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
		WriteToPort(escBmp, 5);
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
* 输入参数： n 表示右边距为 [ n * 横向移动单位或纵向移动单位]英寸
* 输出参数： 无
* 返 回 值： 无
* 其它说明：
***********************************************************************/
int CPrint::MW_SetCharRightSpace(unsigned char n)
{
	if (n <= 0 || n >=255 )
		return -1;

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
    //fputs ("Reading error",stderr); 

	int iByteSent;
    char tmp[100];
    //string sErr;
    unsigned char err = MW_RealTimeStatus(4);

    
    sprintf(tmp, "Printer error Status 0x%X\n", err);
    if (0x12 != err)
    {
        fputs (tmp, stderr); 
        return -1;
    }
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
    if (ucMode < 0 || ucMode > 255)
        return -1;
    
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
* 输入参数： iPost 当前位置距离行首距离
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SetAbsPrintPosition(int iPost)
{
    unsigned char nL,nH;
    nL = iPost % 255;
    nH = iPost / 255;

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
                    0,48            取消下划线模式
                    1,49            选择下划线模式(1 点宽)
                    2,50            选择下划线模式(2 点宽)
* 输出参数： 无
* 返 回 值： 成功返回 0 ，失败返回 -1
* 其它说明： 
***********************************************************************/
int CPrint::MW_SetUndlineMode(unsigned char ucMode)
{
    if (ucMode < 0 || ucMode > 255)
        return -1;
    
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x2D;
    cWriteBuf[2] = ucMode;
    WriteToPort(cWriteBuf, 3);
	return 0;
}
