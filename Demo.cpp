// Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SerialPort.h"
#include "Print.h"

CPrint * g_clsPrint = new CPrint();


int main(int argc, char* argv[])
{

	g_clsPrint->setPortName(2);
	g_clsPrint->setBaud(115200);
	g_clsPrint->InitPort();
	//一下是打印机的功能
 	//char cWriteBuf[10];
// 	cWriteBuf[0] = 0x0A;
// 	for(int i = 0; i < 2; i++)//循环发送  打印空的5行
// 	{
// 		//g_clsPrint->WriteToPort("Hello!", 6);
// 		g_clsPrint->MW_LF(); //发送打印换行
// 	}

	//err = g_clsPrint->MW_RealTimeStatus(4);


	g_clsPrint->MW_PrintBitmap((unsigned char*)"Hello");
	
	for( int i = 0; i < 400000; i++);

	g_clsPrint->ClosePort();



	return 0;
}
