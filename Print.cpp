// Print.cpp: implementation of the CPrint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Print.h"




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
* 输出参数： // 对输出参数的说明。
* 返 回 值： 0 正常，出错时返回出错码, -1 表示参数不正确
* 其它说明： // 其它说明
***********************************************************************/
int CPrint::MW_RealTimeStatus(int n)
{
	unsigned char cWriteBuf[10];
	if((n > 4)||(n < 1))
		return -1;

	cWriteBuf[0] = 0x10;
	cWriteBuf[1] = 0x04;
	cWriteBuf[2] = n;
	WriteToPort(cWriteBuf, 3);
	return 0;
}
// typedef struct tagBITMAPFILEHEADER 
// {
//     WORD bfType; 
//     DWORD bfSize; 
//     WORD bfReserved1; 
//     WORD bfReserved2; 
//     DWORD bfOffBits; 
// } BITMAPFILEHEADER; 

// typedef struct tagBITMAPINFOHEADER{ 
//     DWORD biSize; 
//     LONG biWidth; 
//     LONG biHeight; 
//     WORD biPlanes; 
//     WORD biBitCount DWORD biCompression; 
//     DWORD biSizeImage; 
//     LONG biXPelsPerMeter; 
//     LONG biYPelsPerMeter; 
//     DWORD biClrUsed; 
//     DWORD biClrImportant; 
// } BITMAPINFOHEADER; 
void CPrint::MW_PrintBitmap(const unsigned char *pcPicAddr)
{
 
	FILE * pFile;  
    long lSize;  
    unsigned char * pucBuffer;  
    size_t result;  
      
    /* 若要一个char不漏地读入整个文件，只能采用二进制方式打开 */   
    pFile = fopen ("D:\\VC_Project\\Demo\\Bitmap.bmp", "rb" );  
    if (pFile==NULL)  
    {  
        fputs ("File error",stderr);  
        //exit (1);  
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
        //exit (2);  
    }  
	
    /* 将文件拷贝到buffer中 */  
    result = fread (pucBuffer,1,lSize,pFile);  
    if (result != (size_t)lSize)  
    {  
        fputs ("Reading error",stderr);  
        //exit (3);  
    }  
    /* 现在整个文件已经在buffer中，可由标准输出打印内容 */  
// 	for(int i = 0; i < 100; i++)
// 	{
// 		if (i % 10 == 0)
// 		{
// 			printf("\n");
// 		}
// 		printf("%X ", pucBuffer[i]); 
// 	}
// 	printf("\n");
//  	printf("lSize = %d  0x%X\n", lSize, lSize);
	
	/////////////////////////////////////////////////////////////////////////
	BITMAPFILEHEADER *ptBitmap = (BITMAPFILEHEADER *)pucBuffer;
	BITMAPINFOHEADER *ptBitmapH = (BITMAPINFOHEADER *)&pucBuffer[14];







	//设置字符行间距为 0 点行
//     unsigned char data[] = { 0x1B, 0x33, 0x00 };
// 	WriteToPort(data, sizeof(data));

	//ESC * m nL nH d1…dk   选择位图模式
	// ESC * m nL nH
	unsigned char escBmp[] = { 0x1B, 0x2A, 0x00, 0x00, 0x00 };

	////////////////////////
	// for test
	unsigned char ucaBitData[10];
	/*
	// 8 点单密度, 最多可打 48 个，即192 列，按示例图算，一行应该有 384 
	escBmp[2] = 0x00;
	escBmp[3] = 0xC0;
	escBmp[4] = 0x00;

	WriteToPort(escBmp, 5);
	ucaBitData[0] = 0x81;
	ucaBitData[1] = 0x42;
	ucaBitData[2] = 0x24;
	ucaBitData[3] = 0x18;
	
	for(int i = 0; i < 48; i++)
	{
		WriteToPort(ucaBitData, 4);
	}
	MW_LF();
	*/

	/*
	// 8 点双密度，可打印 38 个，限制应该也是 384
	escBmp[2] = 0x01;
	escBmp[3] = 0x80;
	escBmp[4] = 0x01;
	WriteToPort(escBmp, 5);
	ucaBitData[0] = 0x80;
	ucaBitData[1] = 0x40;
	ucaBitData[2] = 0x20;
	ucaBitData[3] = 0x10;
	ucaBitData[4] = 0x08;
	ucaBitData[5] = 0x04;
	ucaBitData[6] = 0x02;
	ucaBitData[7] = 0x01;
	ucaBitData[8] = 0x18;
	ucaBitData[9] = 0x24;
	for(int i = 0; i < 38; i++)
	{
		WriteToPort(ucaBitData, 10);
	}
	MW_LF();
	*/
	
	
	// 24 点双密度
	////////////////////////
	escBmp[2] = 0x21; // 0x21
	escBmp[3] = 0x02;
	escBmp[4] = 0x00;
	WriteToPort(escBmp, 5);
	ucaBitData[0] = 0x00;
	ucaBitData[1] = 0xFF;
	ucaBitData[2] = 0x00;

// 	ucaBitData[1] = 0x00;
// 	ucaBitData[4] = 0x00;
// 	ucaBitData[7] = 0x00;
// 
// 	ucaBitData[2] = 0x00;
// 	ucaBitData[5] = 0x00;
// 	ucaBitData[8] = 0x00;

	for(int i = 0; i < 2; i++)
	{
		WriteToPort(ucaBitData, 3);
	}
	MW_LF();
	



	/* 不同模式宽度限制：每行固定像素点为 384
			0x00: 8 点单密度，192 宽的位图
			0x01: 8 点双密度，384 宽的位图 
			0x32: 24 点单密度，192 宽的位图 
			0x33: 24 点双密度，384 宽的位图 
	*/
// 	escBmp[2] = 0x33;
// 	
// 	//nL, nH
// 	escBmp[3] = (unsigned char)(ptBitmapH->biWidth % 256);
// 	escBmp[4] = (unsigned char)(ptBitmapH->biWidth / 256);
// 	
// 
// 
// 
// 	int iWidth;
// 	int iHeight;
// 	int iBMPBpp;
// 	int iLineWidthAlign;
// 	int iLineWidthReal;
// 	int iCurPiexl;
// 	int iCurBlue;
// 
// 	unsigned char ucaData[10] = {0};
// 	unsigned char *pucSrc; // 指向图片数据
// 
// 	iWidth = ptBitmapH->biWidth;
// 	iHeight = ptBitmapH->biHeight;
// 	iBMPBpp = ptBitmapH->biBitCount;
// 	
// 	iLineWidthReal = iWidth * iBMPBpp / 8;
// 	iLineWidthAlign = (iLineWidthReal + 3) & ~0x3;   // 向 4 取整 
// 	
// 	
// 
// 	pucSrc = pucBuffer + ptBitmap->bfOffBits;
// 	//pucSrc = pucSrc + (iHeight - 1) * iLineWidthAlign;  // 指向最后一行 
// 	for ( i = 0; i < 16; i++)
// 	{
// 		printf("%02d ", i); 
// 	}
// 	for(i = 0; i < 96; i++)
// 	{
// 		if (i % 16 == 0)
// 		{
// 			printf("\n");
// 		}
// 		printf("%02X ", pucSrc[i]); 
// 	}
// 	printf("\n");

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

// 	for (int y = 0; y < (iHeight / 24 + 1); y++)// 循环组，每 8 行一组 + 1 处理最后一组
// 	{	
// 		WriteToPort(escBmp, 5);
// 		for (int x = 0; x < iWidth; x++)// 循环列，每列一字节
// 		{
// 			ucaData[0] = 0x00;
// 			ucaData[1] = 0x00;
// 			ucaData[2] = 0x00;
// 			for (int z = 0; z < 24; z++) // 循环每列字节位
// 			{
// 				//当前像素点标号(从 0 开始) = 第几位 * 每行像素数 + 本行第向个像素 + 第几组 * 组行数 * 每行像素数 =(z * iWidth + x + y * 8 * iWidth);
// 				iCurPiexl = (z * iWidth + x + y * 24 * iWidth);
// 
// 				// 判断是还在当前位图中 
// 				if (iCurPiexl < iWidth * iHeight)
// 				{
// 					// iCurPiexl / iWidth = 所在行 
// 					// iCurPiexl % iWidth = 所在列 = x
// 					// 进行 4 字节对齐后的像素位置 = 所在行 * 对齐后的列宽 + 所在列 * 3（24 位深）
// 					iCurBlue = iCurPiexl / iWidth * iLineWidthAlign + ( (iWidth - x - 1) * 3); // (iWidth - x - 1) 行数据颠倒
// 					if (pucSrc[iCurBlue] != 0xFF ) // 0x00 0x00 0x00 表示黑色，0xFF 0xFF 0xFF 表示白色
// 					{
// 						if ( z < 8)
// 						{
// 							ucaData[0] |= (1 << (7-z));
// 						}
// 						else
// 						if ( (z >= 8) && (z < 16) )
// 						{
// 							ucaData[1] |= (1 << (7-z));
// 						}
// 						else
// 						if((z >= 16) && (z < 24))
// 						{
// 							ucaData[2] |= (1 << (7-z));
// 						}
// 						
// 					}
// 				}	
// 			}
// 			WriteToPort(ucaData, 3);
// 			printf("0x%X ", ucaData[0]);
// 			printf("0x%X ", ucaData[1]);
// 			printf("0x%X ", ucaData[2]);
// 			printf("\n");
// 		}
// 		MW_LF();
// 	}


	/////////////////////////////////////////////////////////////////////////








    /* 结束演示，关闭文件并释放内存 */  
    fclose (pFile);  
    free (pucBuffer);  
   // return 0; 
	

}
