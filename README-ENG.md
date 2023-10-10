A universal UI wrapper that can wrap any command line program into a UI and can be cross-platform
Configure actions using ini files
The UI interface of frpc is naturally very easy, so you don’t have to display the command line window on the desktop and keep the desktop clean.

feature:  
- Wrap the command line window program into a tray icon and redirect the command line output to the program's text box
- Using the ini file customization function, you can customize whether to display the tray icon and whether to exit by clicking the close button.
- Actions can be customized and support three actions: when starting, when exiting, and when dragging and dropping into the interface. Actions support batch processing and multiple commands.

After startup, the icon will be displayed in the system tray. When the mouse is placed on it, the software name (base/title) configured in the ini will be displayed.

The default name of the target program is CmdUI, which can be changed to other names. When starting, the ini file with the same name as the program will be read to execute the corresponding configuration.

INI content description:

```
[base]
;Software name, the name displayed on the tray and the name in the upper left corner of the main interface to facilitate identification when multiple programs are running at the same time
title=xxx

[func]
;Start delay, seconds, generally not used
start_delay=0

;Whether the UI main interface is hidden after startup, if it is hidden, only the tray icon will be displayed, 0: not hidden, 1: hidden
start_hide=0

;Whether to display the tray icon after startup, it is displayed by default
show_tray=1

;Exit when clicking the close button of the UI main interface? Default 0: Do not exit 1: Exit
close_btn_exit=1

[action]
;The action to be performed at startup, separate multiple commands with |
on_start=Exe64bitDetector.exe -f "C:\Program Files (x86)\Internet Explorer\iexplore.exe"|D:\Green\[Process]\processhacker\x64\ProcessHacker.exe

;The action performed when dragging and dropping a file onto the interface, %s represents the name of the dragged and dropped file; supports multiple commands, separated by | in the middle **
on_drop=Exe64bitDetector.exe -f %s

;Same as on_start, the action to be executed when exiting, multiple commands are separated by |**
on_exit=D:\CPP\ecfile\Release\ecfile.exe
```


**For example:**
I currently have a command line program Exe64bitDetector.exe, which detects whether a PE file (EXE or DLL) is 32-bit or 64-bit.
**I want to make it into a UI interface. When the PE file is dragged and dropped into the UI interface, the detection results will be displayed on the UI interface**

Then it can be as follows:
Change the name of the program to Exe64bitDetectorUI.exe (to facilitate your own identification, of course, you can also call it another name), put it in the same directory as the command line program Exe64bitDetector.exe, and then create an Exe64bitDetectorUI.ini file (UTF8 encoding) in the directory.
  

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


**OK, if there is no problem, start the program Exe64bitDetectorUI.exe, drag and drop the PE file to the interface, and the UI interface will display the detection results**
![image.png](https://raw.githubusercontent.com/startl/BatchUrlGenerator/master/BlogImg20230921155159.png)


**Another example**, if you want to use it as a UI package for frpc, change the program name to frpcUI.exe, put it in the same directory as frpc.exe, and create a frcpUI.ini file (UTF8 encoding) in the same directory.


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


## Compile
Because to be used as independent programs, WxWidgets and asl need to be compiled into static libraries. If it is windows, the thread mode also needs to be set to MT.
