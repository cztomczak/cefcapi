@echo off

if exist .\Release rmdir /q /s .\Release
if exist .\cef_extracted rmdir /q /s .\cef_extracted
if exist .\cef_partially_extracted rmdir /q /s .\cef_partially_extracted

del *.obj
del *.pdb
del *.res
del *.coff