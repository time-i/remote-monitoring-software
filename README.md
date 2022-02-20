# remote-monitoring-software
设计的远程控制软件 客户端和服务端，两个防护软件
程序及源代码—————————————**//各部分的源代码**
    ├─ControlServer————————————**//控制端工程代码**
    │  │   
    │  ···               
    │  └─x64
    │      └─Debug
    │             ControlServer.exe————————**//控制端可执行程序**
    │             ControlServer.pdb
    │            
    │      
    ├─MainContralView————————————**//被控端工程代码**
    │  │  
    │  ···              
    │  └─x64
    │      └─Debug
    │              MainContralView.exe————————**//被控端可执行程序**
    │              MainContralView.pdb
    │
    │              
    ├─moudle——————————————————**//功能实现中用到的录音/拍照/注册表附加模块**
    │      luyin.exe
    │      luyin.py
    │      regedit.exe
    │      snap.exe
    │      snap.py
    │      
    ├─防护\_DLL注入——————————————**//基于dll注入的防护软件**
    │  │  说明文档.txt
    │  │ 
    │  ├─Hook_DLL——————————————**//HOOK主程序工程文件**
    │  │
    │  └─Server_Hook——————————————**//HOOK动态链接库工程文件**
    │
    │ 
    └─防护\_连接监控——————————**//基于连接监控的防护软件**
            network_monitor.exe——————**//连接监控可执行文件**
            network_monitor.py
