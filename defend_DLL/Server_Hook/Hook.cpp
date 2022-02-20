
#include <windows.h>
#include <stdio.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <stdlib.h>
#include <string>
#include <iostream>

/*
通过进程名获得PID
*/
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}


	/*
	通过PID获得进程的句柄
	*/
	HANDLE GetProcessHandle(int nID)
	{
		return OpenProcess(PROCESS_ALL_ACCESS, FALSE, nID);
	}

	//通过进程名（带后缀.exe）获取进程句柄
	HANDLE GetProcessHandle(LPCWSTR lpName)
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapshot)
		{
			return NULL;
		}
		PROCESSENTRY32 pe = { sizeof(pe) };
		BOOL fOk;
		for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe))
		{
			if (!_tcsicmp(pe.szExeFile, lpName)) // 不区分大小写 但必须用unicode编码
			{
				CloseHandle(hSnapshot);
				return GetProcessHandle(pe.th32ProcessID);
			}
		}
		return NULL;
	}

/*
主函数
*/
int main()
{
	//对被控Server端进行hook操作
	std::string myString = "ControlServer.exe";
	std::cout<<"跟踪进程名:" << myString<< "跟踪进程pid:";
	std::wstring stemp = s2ws(myString);
	LPCWSTR ProcName = stemp.c_str();

	DWORD Process_ID = GetProcessId(GetProcessHandle(ProcName));
	std::cout << Process_ID << std::endl;

	
	//打开进程
	HANDLE hPro = OpenProcess(PROCESS_CREATE_THREAD | //允许远程创建线程
		PROCESS_VM_OPERATION | //允许远程VM操作
		PROCESS_VM_WRITE,//允许远程VM写
		FALSE, Process_ID);
	if (hPro == NULL) {
		MessageBoxA(NULL, "被控端进程不存在！", "错误", 0);
	}

	// 在远程进程中为路径名称分配空间
	char str[256] = { 0 };
	strcpy_s(str, "C:\\Users\\NOIKH\\OneDrive\\桌面\\Server_Hook\\DLL.dll");	// 注入所用的dll文件路径
	int dwSize = (lstrlenA(str) + 1);	//路径大小+1是因为\0

	//开辟空间
	LPVOID pszLibFileRemote = (PWSTR)VirtualAllocEx(hPro, NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (pszLibFileRemote == NULL) {
		MessageBoxA(NULL, "准备hook.dll失败！", "错误", 0);
		return -1;
	}

	//写入数据
	SIZE_T  dwWritten;	//写入字节数
	DWORD n = WriteProcessMemory(hPro, (LPTHREAD_START_ROUTINE)pszLibFileRemote, str, dwSize, &dwWritten);//写入到内存
	if (n == NULL) {
		MessageBoxA(NULL, "写入hook.dll失败！", "错误", 0);
		return -1;
	}

	//Kernel32模块已经被包含在依赖库里自动加载进来了，所以我们可以直接获取
	PTHREAD_START_ROUTINE pfnThreadRtn = 
		(PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryA");
	if (pfnThreadRtn == NULL) {
		MessageBoxA(NULL, "获取dll库函数失败！", "错误", 0);
		return -1;
	}

	//创建远线程
	DWORD dwID = NULL;
	HANDLE hThread = CreateRemoteThread(hPro, 0, 0, pfnThreadRtn, pszLibFileRemote, 0, &dwID);
	if (hThread == NULL) {
		MessageBoxA(NULL, "创建远线程失败！", "错误", 0);
		printf("远程线程错误 %d", GetLastError());

	}
	printf("remote success\n");

	//等待线程结束
	WaitForSingleObject(hThread, INFINITE);
	return 0;

}