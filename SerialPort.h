// SerialPort.h: interface for the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALPORT_H__B7189E10_6AF6_4C42_9F29_DBB34CB825AD__INCLUDED_)
#define AFX_SERIALPORT_H__B7189E10_6AF6_4C42_9F29_DBB34CB825AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSerialPort  
{
public:
	CSerialPort();
	virtual ~CSerialPort();
public:
	//设置串口号
	void	setPortName(const int port);
	//设置波特率
	void	setBaud( UINT baud );
	//打开串口
	BOOL	InitPort( UINT portnr = 2, UINT baud = 1152, char parity = 'N', UINT databits = 8, UINT stopsbits = 1 );
	//发送数据
	int	WriteToPort(unsigned  char *writeData,int WriteSize );
	//读取数据
	int	ReceiveChar( char *readBuff, int ReadSize );
	//关闭串口
	void	ClosePort();
private:
	HANDLE	m_hComm;   //串口句柄
	COMMTIMEOUTS	m_CommTimeouts;//超时时间
	DCB	m_dcb;//设置波特率 停止位 串口号码.....，用于串口设备的参数设置
	int	m_port;  //串口号
UINT	m_baud;  //波特率

};

#endif // !defined(AFX_SERIALPORT_H__B7189E10_6AF6_4C42_9F29_DBB34CB825AD__INCLUDED_)
