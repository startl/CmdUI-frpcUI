**Welcome to the CmdUI-frpcUI wiki!**

The cross platform command line UI wrapper is based on vs2019 + QT, using INI file configuration action, can be used as a general command line program UI.  
Of course, it can also be used as the UI interface of *frpc*, so that there is no need to display the command line window on the desktop to keep the desktop fresh.

After startup, the icon will be displayed in the system tray, and the software name configured by ini will be displayed when the mouse is put up.
Target program default name CmdUI.exe (Windows), which can be changed to another name. When starting, the configuration execution actions in the INI file with the same name as the program will be read.

**For example**, to install as the UI of FRPC, change the program name to frpcUI.exe , put in frpc.exe In the same directory, create a frcpUI.ini File (utf8 encoding)  
The description of ini is as follows:  
**::title, the name of the software, the name of the tray display and the name of the upper left corner of the main interface to facilitate multiple runtime identification**  
title=frpc  
**::host, executor, CMD under windows, bash under Linux**  
host=cmd  
**::start_ Delay, startup delay, generally not used**  
start_ delay=0  
**::start_ Hide, whether to hide after startup, 0: do not hide, 1: do not hide**  
start_ hide=1  
**::the action executed when the program starts can be a batch file (win) or sh script (Linux), or a series of commands separated by | such as taskkill / F / im frpc.exe |frpc -c  frpc.ini**  
on_ start= frpc.bat  
**::the action executed when the program exits. The rules are the same as above**  
on_ exit=taskkill /f /im  frpc.exe  

OK, if there is no problem, the program will create an icon in the tray, and FRPC starts successfully  

**## Another example**: to wrap a PE file as a 32-bit or 64 bit command line, change the program name to exe64bitdetectorUI.exe and put it in exe64 bitDetector.exe Command line program directory, in the same directory to create an exe64bitdetectorUI.ini (utf8 encoding)  
The description of ini is as follows:  
**::title, the name of the software, the name of the tray display and the name of the upper left corner of the main interface to facilitate multiple runtime identification**  
title=Exe64bitDetector  
**::host, executor, CMD under windows, bash under Linux**  
host=cmd  
**::start_ Delay, startup delay, generally not used****  
start_ delay=0  
**::start_ Hide, whether to hide after startup, 0: do not hide, 1: do not hide**  
start_ hide=0  
**::action performed when dragging and dropping a file onto a node surface,% s represents the name of the drag and drop file**  
on_ drop=Exe64 bitDetector.exe  -f %s  

OK, if there is no problem, the program starts, drag and drop PE file to the interface, and the detection result will be displayed

--------------------------------------------------------------------------------------------------------------------------------

跨平台命令行UI包装程序，基于VS2019+QT构建。
采用ini文件配置动作，可以作为通用命令行程序的UI。
当然也可以用来作为frpc的UI界面，这样就不用在桌面上显示命令行窗口，保持桌面清爽。

启动后会在系统托盘显示图标，鼠标放上去会显示ini配置的软件名称。

目标程序默认名称CmdUI.exe(windows)，可以改成其它名字，启动时会读取和程序同名的ini文件里面的配置执行动作。

**例如**，要作为frpc的UI报装，则将程序名改成frpcUI.exe, 放到frpc.exe同目录下，在同目录下建立一个frcpUI.ini的文件(UTF8编码)  
ini内容说明如下：  
**::title,软件名称，托盘显示的名称和主界面左上角的名称，方便多个运行时识别**  
title=frpc  
**::host,执行器，windows下为cmd,linux下为bash**  
host=cmd  
**::start_delay，启动延时，一般用不到**  
start_delay=0  
**::start_hide，启动后是否隐藏，0：不隐藏，1：隐藏**  
start_hide=1  
**::程序启动时执行的动作，可以为批处理文件(win)或sh脚本(linux)，也可以为一系列命令，采用|隔开，例如：taskkill /f /im frpc.exe|frpc -c frpc.ini**  
on_start=frpc.bat  
**::程序退出时执行的动作，规则同上**  
on_exit=taskkill /f /im frpc.exe  

OK，如果没有问题，则程序会在托盘建立图标，frpc启动成功  

**再例如**：要作为PE文件32位还是64位的命令行包装，则更改程序名为Exe64bitDetectorUI.exe，放到Exe64bitDetector.exe命令行程序目录下，在同目录下建一个Exe64bitDetectorUI.ini(UTF8编码)  
ini内容说明如下：  
**::title,软件名称，托盘显示的名称和主界面左上角的名称，方便多个运行时识别**  
title=Exe64bitDetector  
**::host,执行器，windows下为cmd,linux下为bash**  
host=cmd  
**::start_delay，启动延时，一般用不到**  
start_delay=0  
**::start_hide，启动后是否隐藏，0：不隐藏，1：**  
start_hide=0  
**::当拖放文件到节面上时执行的动作，%s代表拖放的文件名**  
on_drop=Exe64bitDetector.exe -f %s  

OK，如果没有问题，则程序启动，拖放pe文件到界面，会显示探测结果
