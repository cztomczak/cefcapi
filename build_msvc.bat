@echo off

if not exist .\Release (
    call .\deps.bat
)

if "%VSINSTALLDIR%" equ "" (
    for /f "usebackq delims=" %%i in (`"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -prerelease -latest -property installationPath`) do (
        call "%%i\VC\Auxiliary\Build\vcvars64.bat"
        goto :LOADED
    )
)

:LOADED

set CC=cl
set WARNINGS=/W4 /wd4100
set DBG_FLAGS=/Zi
set CFLAGS=%DBG_FLAGS% %WARNINGS% /I.\cef_extracted /I. /nologo
set LFLAGS=/link /LIBPATH:.\Release /SUBSYSTEM:CONSOLE libcef.lib

set CFILES=.\examples\main_win.c

set RC=rc
set RCFILE=main.rc

%RC% /nologo /fo browser.res %RCFILE%

%CC% %CFLAGS% /Fe.\Release\cef_example.exe browser.res %CFILES% %LFLAGS%
