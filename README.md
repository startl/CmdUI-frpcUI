命令行UI包装程序，基于VS2019+QT构建，可以跨平台。  
采用ini文件配置动作，可以作为通用命令行程序的UI。  
当然也可以用来作为frpc的UI界面，这样就不用在桌面上显示命令行窗口，保持桌面清爽。  

特色：  
** 将命令行黑窗口程序包装成托盘图标，将命令行输出重定向到本程序的文本框  
** 采用ini文件定制功能，可以定制是否显示托盘图标，点击关闭按钮是否退出  
** 可以定制动作，启动时,退出时,拖放到界面时，动作同时支持批处理和多条命令  

启动后会在系统托盘显示图标，鼠标放上去会显示ini配置的软件名称。  

目标程序默认名称CmdUI.exe(windows)，可以改成其它名字，启动时会读取和程序同名的ini文件里面的配置执行动作。  

**例如**，要作为frpc的UI包装，则将程序名改成frpcUI.exe, 放到frpc.exe同目录下，在同目录下建立一个frcpUI.ini的文件(UTF8编码)  
ini内容说明如下：  
[base]  
**软件名称，托盘显示的名称和主界面左上角的名称，方便多个运行时识别**  
title=frpc  
**执行器，windows下为cmd,linux下为bash**  
host=cmd  
[func]  
**启动延时，一般用不到**  
start_delay=0  
**启动后是否隐藏，0：不隐藏，1：隐藏**  
start_hide=1  
[action]  
**程序启动时执行的动作，可以为批处理文件(win)或sh脚本(linux)，也可以为一系列命令，采用|隔开，例如：taskkill /f /im frpc.exe|frpc -c frpc.ini**  
on_start=frpc.bat  
**程序退出时执行的动作，规则同上**  
on_exit=taskkill /f /im frpc.exe  

OK，如果没有问题，则程序会在托盘建立图标，frpc启动成功  

**再例如**：要作为PE文件32位还是64位的命令行包装，则更改程序名为Exe64bitDetectorUI.exe，放到Exe64bitDetector.exe命令行程序目录下，在同目录下建一个Exe64bitDetectorUI.ini(UTF8编码)  
ini内容说明如下：  
[base]  
**软件名称，托盘显示的名称和主界面左上角的名称，方便多个运行时识别**  
title=Exe64bitDetector  
**执行器，windows下为cmd,linux下为bash**  
host=cmd  
[func]  
**启动延时，一般用不到**  
start_delay=0  
**启动后是否隐藏，0：不隐藏，1：**  
start_hide=0  
**启动后不显示托盘图标，默认显示**
show_tray=0
**启动后不自动隐藏，默认隐藏**
close_btn_exit=1  
[action]  
**::当拖放文件到界面上时执行的动作，%s代表拖放的文件名**  
on_drop=Exe64bitDetector.exe -f %s  

OK，如果没有问题，则程序启动，拖放pe文件到界面，会显示探测结果  
