@echo off
set SCRPATH=%~dp0
set PROGNAME=mimikatz
set REL=ReleaseEXE
set REFL=ReflectiveDLL
set X32=Win32
set X64=x64

set TARGETPATH=%SCRPATH%bin\%REL%-%X32%\%PROGNAME%-%X32%.exe
copy /y %TARGETPATH% %SCRPATH%testbuild

set TARGETPATH=%SCRPATH%bin\%REL%-%X64%\%PROGNAME%-%X64%.exe
copy /y %TARGETPATH% %SCRPATH%testbuild

set TARGETPATH=%SCRPATH%bin\%REFL%-%X32%\%PROGNAME%-%REFL%-%X32%.dll
copy /y %TARGETPATH% %SCRPATH%testbuild

set TARGETPATH=%SCRPATH%bin\%REFL%-%X64%\%PROGNAME%-%REFL%-%X64%.dll
copy /y %TARGETPATH% %SCRPATH%testbuild
