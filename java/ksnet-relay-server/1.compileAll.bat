@echo off

SETLOCAL

SET BAT_CLI=1-1.compileCli.bat
SET BAT_SVR=1-2.compileSvr.bat

@echo %BAT_CLI% ������ ����.

@echo off
call %BAT_CLI%

@echo.

@echo %BAT_SVR% ������ ����.

@echo off
call %BAT_SVR%

ENDLOCAL