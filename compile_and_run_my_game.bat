@echo off
set FILE_NAME=main
g++ %FILE_NAME%.cpp -DLOCAL -static -o exe\%FILE_NAME%_static.exe
powershell -ExecutionPolicy Bypass -File "bin\setup.ps1" -fileName "exe\%FILE_NAME%_static.exe"
pause
exit
