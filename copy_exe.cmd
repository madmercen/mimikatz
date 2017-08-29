@echo off
set SCRPATH=%~dp0
set PROGNAME=mimikatz
set REL=ReleaseEXE
set REFL=ReflectiveDLL
set SIMPLE=Simple_DLL
set X32=Win32
set X64=x64

set TARGETPATH=%SCRPATH%%X32%\%PROGNAME%-%X32%.exe
copy /y %TARGETPATH% %SCRPATH%testbuild

set TARGETPATH=%SCRPATH%%X64%\%PROGNAME%-%X64%.exe
copy /y %TARGETPATH% %SCRPATH%testbuild

set TARGETPATH=%SCRPATH%%X32%\%PROGNAME%-%REFL%-%X32%.dll
copy /y %TARGETPATH% %SCRPATH%testbuild

set TARGETPATH=%SCRPATH%%X64%\%PROGNAME%-%REFL%-%X64%.dll
copy /y %TARGETPATH% %SCRPATH%testbuild

set TARGETPATH=%SCRPATH%%X32%\%PROGNAME%-%SIMPLE%-%X32%.dll
copy /y %TARGETPATH% %SCRPATH%testbuild

set TARGETPATH=%SCRPATH%%X64%\%PROGNAME%-%SIMPLE%-%X64%.dll
copy /y %TARGETPATH% %SCRPATH%testbuild
