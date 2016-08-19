// SerialPort.cpp: implementation of the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SerialPort.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialPort::CSerialPort()  //构造函数中初始化
{
	m_hComm = NULL;
	m_port = 1;
	m_baud = 9600;
}
CSerialPort::~CSerialPort()//析构中释放
{
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
}
/**********************************************************************
* 函数名称： setPortName
* 功能描述： 设置串口号
* 输入参数： port 要设置的串口号
* 输出参数： 无
* 返 回 值： 无
* 其它说明： 
***********************************************************************/
void CSerialPort::setPortName( const int port)//设置串口号
{
    m_port = port;
}
/**********************************************************************
* 函数名称： setBaud
* 功能描述： 设置串口波特率
* 输入参数： baud 要设备的波特率
* 输出参数： 无
* 返 回 值： 无
* 其它说明： 
***********************************************************************/
void CSerialPort::setBaud( UINT baud )//设置波特率
{
	m_baud = baud;
}

/**********************************************************************
* 函数名称： ClosePort
* 功能描述： 关闭串口
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 无
* 其它说明： 
***********************************************************************/
void CSerialPort::ClosePort()//关闭串口
{
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComm);
	}
	m_hComm = INVALID_HANDLE_VALUE;
}
/**********************************************************************
* 函数名称： InitPort
* 功能描述： 初始化串口
* 输入参数： portnr 串口号，baud 波特率，parity 极性，databits 数据位，
                stopsbits 停止位
* 输出参数： 无
* 返 回 值： 成功返回 TRUE，失败返回 FALSE
* 其它说明： 
***********************************************************************/
BOOL CSerialPort::InitPort( UINT portnr, UINT baud, char parity, UINT databits, UINT stopsbits )//打开串口
{
	//assert(portnr > 0 && portnr < 50);
	char *szPort = new char[50]; // 保存串口号
	char *szBaud = new char[50]; // 保存串口设置参数
	baud = m_baud;
	portnr = m_port;
	sprintf(szPort, "COM%d", portnr);
	sprintf(szBaud, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
	m_hComm = CreateFile(szPort,	// communication port string (COMX)
		GENERIC_READ | GENERIC_WRITE,	// read/write types
		NULL,	// comm devices must be opened with exclusive access
		NULL,	// no security attributes
		OPEN_EXISTING,	// comm devices must use OPEN_EXISTING
		FILE_ATTRIBUTE_NORMAL,	// Async I/O
		NULL);	// template must be 0 for comm devices
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// port not found
		delete [] szPort;
		delete [] szBaud;
		return FALSE;
	}
	// set the timeout values
	memset(&m_CommTimeouts, 0, sizeof(m_CommTimeouts));
	memset(&m_dcb, 0, sizeof(m_dcb));
	m_CommTimeouts.ReadIntervalTimeout = 1000;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.ReadTotalTimeoutConstant = 1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.WriteTotalTimeoutConstant = 1000;
	// configure
	// 设置串口
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{	  
		if (GetCommState(m_hComm, &m_dcb))
		{
			if (BuildCommDCB(szBaud, &m_dcb))
			{
				if (SetCommState(m_hComm, &m_dcb))
					; // normal operation... continue
				else
					return FALSE;//ProcessErrorMessage("SetCommState()");
			}
			else
				return FALSE;//ProcessErrorMessage("BuildCommDCB()");
		}
		else
			return FALSE;//ProcessErrorMessage("GetCommState()");
	}
	else
		return FALSE;//ProcessErrorMessage("SetCommTimeouts()");

	delete [] szPort;
	delete [] szBaud;
	// flush the port
	// 清串口缓存
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	return TRUE;
}

/**********************************************************************
* 函数名称： WriteToPort
* 功能描述： 向串口写数据
* 输入参数： *writeData 指向要发送的数据，WriteSize 为要发送的数据大小
* 输出参数： 无
* 返 回 值： 成功返回发送的字节数，失败返回 -1
* 其它说明： 
***********************************************************************/
int	CSerialPort::WriteToPort( unsigned  char *writeData, int WriteSize )//发送数据
{
	BOOL bResult = TRUE;
	DWORD BytesSent = 0;
	// Clear buffer
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	bResult = WriteFile(m_hComm, writeData, WriteSize, &BytesSent, NULL);
	if (!bResult)  
	{
		DWORD dwError = GetLastError();
		switch (dwError)
		{
		case ERROR_IO_PENDING:
			{
				// continue to GetOverlappedResults()
				BytesSent = 0;
				//bWrite = FALSE;
				return -1;
				break;
			}
		default:
			{
				// all other error codes
				//port->ProcessErrorMessage("WriteFile()");
				//return -1;
			}
		}
	} 
	// end if(bWrite)
	//Verify that the data size send equals what we tried to send
	if (BytesSent != (DWORD)WriteSize)	// Length of message to send)
	{
		//TRACE("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", BytesSent, strlen((char*)port->m_szWriteBuffer));
		return -1;
	}
	return BytesSent;
}
//----------------
/**********************************************************************
* 函数名称： ReceiveChar
* 功能描述： 从串口接收数据
* 输入参数： *readBuff 指向接收数据缓存，ReadSize 为要接收的字节数
* 输出参数： 无
* 返 回 值： 成功返回接收的字节数，失败返回 -1
* 其它说明： 
***********************************************************************/
int	CSerialPort::ReceiveChar( char *readBuff, int ReadSize )//接收数据
{
	BOOL  bRead = TRUE; 
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	bResult = ReadFile(m_hComm, readBuff, ReadSize, &BytesRead, NULL);
	if (!bResult)  
	{ 
		switch (dwError = GetLastError()) 
		{ 
		case ERROR_IO_PENDING:
			{ 
				// asynchronous i/o is still in progress 
				// Proceed on to GetOverlappedResults();
				//bRead = FALSE;
				return -1;
				break;
			}
		default:
			{
				// Another error has occured.  Process this error.
				//port->ProcessErrorMessage("ReadFile()");
				break;
			} 
		}
	}
	if (BytesRead != (DWORD)ReadSize)
	{
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
		return -1;
	}
	return BytesRead;
}
