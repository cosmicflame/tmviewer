@echo off
"D:\Program Files\NSIS\makensis.exe" /V3 setup.nsi
if errorlevel 1 goto wait
goto quit

:wait
pause

:quit
