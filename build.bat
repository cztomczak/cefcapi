@if exist "%~dp0Release\cefcapi.exe" (
    @del "%~dp0Release\cefcapi.exe"
)

gcc -Wall -Werror -I. -L./Release main_windows.c -o Release/cefcapi.exe -lcef
cd Release/
call "cefcapi.exe"
cd ../

:end
@echo exit code = %ERRORLEVEL%
