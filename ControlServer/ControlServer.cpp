// ControlServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束
#include <iostream>
#include"Server.h"
using namespace std;
void RemoteUI();

int main()
{
	int res = MessageBox(NULL,TEXT("正在使用远端控制软件，是否继续？"),TEXT("提示"),MB_YESNO);
	if (res == IDNO) {
		return -1;
	}
	RemoteUI();
	while (TRUE)
	{
		Server tcpServer;
	
		int nPort = 0;
		char szIpAddress[20];
		cout << "输入连接IP地址: ";
		//这里设置被控端端口号
		cin >> szIpAddress;
		cout << "输入连接端口号: ";
		cin >> nPort;
		BOOL nRet=tcpServer.CreateServer(szIpAddress, nPort);

	
		//BOOL nRet = tcpServer.CreateServer((char*)"127.0.0.1", 9527);
		if (nRet )
		{
			//启动服务器
			tcpServer.RunServer();
		}
		cout << "IP地址或端口号输入错误或未打开主控端" << endl;

	}


	return 0;
}

void RemoteUI()
{
	cout << "远程控制软件\n";
}
