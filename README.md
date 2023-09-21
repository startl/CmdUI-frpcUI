一个通用的UI包装器，可以将任何命令行程序包裹一个UI，可以跨平台  
采用ini文件配置动作
作为frpc的UI界面自然也是很轻松，这样就不用在桌面上显示命令行窗口，保持桌面清爽  

特色：  
- 将命令行窗口程序包装成托盘图标，将命令行输出重定向到本程序的文本框  
- 采用ini文件定制功能，可以定制是否显示托盘图标，点击关闭按钮是否退出  
- 可以定制动作，支持三种动作，启动时、退出时、拖放到界面时，动作支持批处理和多条命令 

启动后会在系统托盘显示图标，鼠标放上去会显示ini配置的软件名称(base/title)

目标程序默认名称CmdUI，可以改成其它名字，启动时会读取和程序同名的ini文件执行相应的配置

INI内容说明：

```
[base]
;软件名称，托盘显示的名称和主界面左上角的名称，方便多个本程序同时运行时识别
title=xxx

[func]
;启动延时，秒，一般用不到
start_delay=0

;启动后UI主界面是否隐藏，如果隐藏，则只显示托盘图标，0：不隐藏，1：隐藏 
start_hide=0

;启动后是否显示托盘图标，默认显示
show_tray=1

;点击UI主界面关闭按钮时是否退出？默认 0：不退出 1：退出
close_btn_exit=1

[action]
;启动时要执行的动作，多个命令之间用|分隔开
on_start=Exe64bitDetector.exe -f "C:\Program Files (x86)\Internet Explorer\iexplore.exe"|D:\Green\[Process]\processhacker\x64\ProcessHacker.exe

;当拖放文件到界面上时执行的动作，%s代表拖放的文件名；支持多个命令，中间用|分隔**
on_drop=Exe64bitDetector.exe -f %s

;同on_start，当退出时要执行的动作，多个命令之间用|分隔开**
on_exit=D:\CPP\ecfile\Release\ecfile.exe
```


**例如：**
我现在有一个命令行程序Exe64bitDetector.exe，是探测PE文件(EXE或者DLL)是32位还是64位的
**我想将其做成UI界面的，当将PE文件拖放到UI界面时，在UI界面上显示探测结果**

那么可以如下：
更改程序名为Exe64bitDetectorUI.exe(方便自己识别，当然也可以随便叫别的名字)，放到命令行程序Exe64bitDetector.exe同一个目录下，然后在该目录下建一个Exe64bitDetectorUI.ini文件(UTF8编码)  
  

```
[base]  
title=Exe64bitDetector  

[func]  
start_delay=0  
start_hide=0  
show_tray=0 
close_btn_exit=1  

[action]  
on_drop=Exe64bitDetector.exe -f %s  
```


**OK，如果没有问题，则将程序Exe64bitDetectorUI.exe启动，拖放PE文件到界面，UI界面会显示探测结果**  
![image.png](https://raw.githubusercontent.com/startl/BatchUrlGenerator/master/BlogImg20230921155159.png)


**再例如**，要作为frpc的UI包装，则将程序名改成frpcUI.exe, 放到frpc.exe同目录下，在同目录下建立一个frcpUI.ini的文件(UTF8编码)  


```
[base]  
title=frpc  

[func]  
start_delay=0  
start_hide=1  

[action]  
on_start=frpc.bat  
on_exit=taskkill /f /im frpc.exe  
```


## 编译
因为要做为独立的程序，需要将WxWidgets和asl编译为静态库，如果是windows，还需要设置线程方式为MT