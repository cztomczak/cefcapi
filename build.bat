@if exist "%~dp0Release\cefcapi.exe" (
    @del "%~dp0Release\cefcapi.exe"
)

@call gcc --version
@call gcc -Wall -Werror -I. -I.. -L./Release examples/main_win.c -o Release/cefcapi.exe -lcef
@echo exit code = %ERRORLEVEL%
@cd Release/
call "cefcapi.exe"
@echo exit code = %ERRORLEVEL%
@cd ../
