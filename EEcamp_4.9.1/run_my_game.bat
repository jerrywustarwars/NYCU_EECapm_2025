@echo off
set FILE_NAME=main
powershell -ExecutionPolicy Bypass -File "bin\setup.ps1" -fileName "exe\%FILE_NAME%_static.exe"
pause
exit
