cd %CD%

if exist C:\Keil\ARM\BIN40\fromelf.exe (C:\Keil\ARM\BIN40\fromelf.exe --bin -o  Boot.bin   .\obj\Boot.axf) 

rem dir *.bin /TW

if exist  D:\Work2013Bin (copy /Y Boot.bin   D:\Work2013Bin) else (copy /Y Boot.bin   c:\)

rem pause