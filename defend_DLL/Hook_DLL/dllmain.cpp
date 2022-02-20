#include "pch.h"
#include "windows.h"
#include "stdio.h"
HMODULE hDll = NULL; //模块句柄
typedef int(*FunPtr)(int a, int b);//入口地址
FunPtr func = NULL;	//指向API地址
char Ptr[] = { 0x9E,0x00,0x00,0x00,0x00 };	//跳转指令
char ps[1024] = { 0 };
DWORD dwOldProtect;	//用于存储之前的权限

DWORD dwNumberOfBytesRead;	//用于存储之前的权限
char pc[5] = { 0 }; //保存原数据
FARPROC pfOldAdd;  //指向函数的远指针
BYTE myFuncPtr[5];	//存储我们自己API的地址
DWORD dwTemp = 0;	//保留

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//第一步检索目标模块句柄
		
		MessageBoxA(NULL, "成功", "注入结果", 0);
		
		/*
		检测kernel32.dll中的Process32Next API，通过该API,程序能够生成进程表
		*/
		hDll = GetModuleHandle(L"kernel32.dll");
		if (hDll == NULL) {
			//MessageBoxA(NULL, "进程未加载kernel32.dll模块", "提示", 0);
			//return FALSE;
		}
		func = (FunPtr)GetProcAddress(hDll, "Process32Next");
		if (func == NULL) {
			//MessageBoxA(NULL, "进程未加载Process32Next API函数", "提示", 0);
			//return FALSE;
		}
		if (MessageBoxA(NULL, "[Process32Next(API)]:进程能获取所有进程信息，是否要停止该进程？", "提示", MB_YESNO) == 6) {
			printf("pid: %d 进程结束\n", GetProcessId(GetCurrentProcess()));
			TerminateProcess(GetCurrentProcess(), 1);
			return FALSE;
		}

		/*
		检测Gdi32.dll中的CreateBitmap API，通过该API,程序能够录制当前屏幕
		*/
		hDll = GetModuleHandle(L"Gdi32.dll");
		if (hDll == NULL) {
			//MessageBoxA(NULL, "进程未加载Gdi32.dll模块", "提示", 0);
			//return FALSE;
		}
		func = (FunPtr)GetProcAddress(hDll, "CreateBitmap");
		if (func == NULL) {
			//MessageBoxA(NULL, "进程未加载CreateBitmap API函数", "提示", 0);
			//return FALSE;
		}
		if (MessageBoxA(NULL, "[CreateBitmap(API)]:进程能够获取当前屏幕信息，是否要停止该进程？", "提示", MB_YESNO) == 6) {
			printf("pid: %d 进程结束\n", GetProcessId(GetCurrentProcess()));
			TerminateProcess(GetCurrentProcess(), 1);
			return FALSE;
		}

		/*
		检测avicap32.dll，通过该dll,程序能够获得麦克风/摄像头权限
		*/
		hDll = GetModuleHandle(L"avicap32.dll");
		if (hDll == NULL) {
			//printf("%d", GetLastError());
			//MessageBoxA(NULL, "进程未加载avicap32.dll模块", "提示", 0);
			//return FALSE;
		}
		if (MessageBoxA(NULL, "[avicap32(dll)]:进程能够进行录音、拍照，是否要停止该进程？", "提示", MB_YESNO) == 6) {
			printf("pid: %d 进程结束\n", GetProcessId(GetCurrentProcess()));
			TerminateProcess(GetCurrentProcess(), 1);
			return FALSE;
		}

		MessageBoxA(NULL, "dll检测结束", "提示", 0);

		FreeLibrary(hDll);

		// api劫持部分

		//将函数首地址转换成远指针
		//pfOldAdd = (FARPROC)func;
		////第三步获取原API里的内存数据
		//VirtualProtectEx(GetCurrentProcess(), pfOldAdd, 5, PAGE_READWRITE, &dwOldProtect);
		//ReadProcessMemory(GetCurrentProcess(), pfOldAdd, pc, 5, NULL);
		//_asm
		//{
		//	lea eax, Add //获取我们的Add函数地址
		//	mov ebx, func  //原系统API函数地址
		//	sub eax, ebx			 //int nAddr= UserFunAddr – SysFunAddr
		//	sub eax, 5			 //nAddr=nAddr-5
		//	mov dword ptr[myFuncPtr + 1], eax //将算出的地址nAddr保存到NewCode后面4个字节
		//								  //注：一个函数地址占4个字节
		//}
		////写入
		//myFuncPtr[0] = 0xe9;
		//WriteProcessMemory(GetCurrentProcess(), pfOldAdd, &myFuncPtr, 5, &dwNumberOfBytesRead);
		////权限恢复
		//VirtualProtectEx(GetCurrentProcess(), pfOldAdd, 5, dwOldProtect, &dwTemp);
		//MessageBoxA(NULL, "API拦截成功", "ERROR", 0);

		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
