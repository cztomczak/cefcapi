@echo off

call .\clean.bat

for %%i in (.\third_party\*.tar.bz2) do (
    "C:\Program Files\7-Zip\7z.exe" x "%%i" -o.\cef_partially_extracted
    goto :BRK
)
:BRK

"C:\Program Files\7-Zip\7z.exe" x "cef_partially_extracted\*" -o.\cef_extracted
rmdir /q /s cef_partially_extracted

for /d %%i in (cef_extracted\*) do (
    rename "%%i" tmp_cef_extracted
)

move /y cef_extracted\tmp_cef_extracted\* cef_extracted
for /d %%i in (cef_extracted\tmp_cef_extracted\*) do (
    move /y "%%i" cef_extracted
)
rmdir /q /s cef_extracted\tmp_cef_extracted

mkdir .\Release

robocopy .\cef_extracted\Debug .\Release /e
robocopy .\cef_extracted\Resources .\Release /e