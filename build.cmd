set PROGNAME=mimikatz
set REL=Release
set REFL=ReflectiveDLL
set SIMPLEDLL=Simple_DLL
set X32=Win32
set X64=x64
REM Run MSVC 2013 vcvars
call "%VS120COMNTOOLS%\..\..\VC\bin\vcvars32.bat"
msbuild.exe %PROGNAME%.sln /t:%PROGNAME% /p:Configuration="%REL%" /p:Platform="%X32%" /p:BuildProjectReferences=true
msbuild.exe %PROGNAME%.sln /t:%PROGNAME% /p:Configuration="%REL%" /p:Platform="%X64%" /p:BuildProjectReferences=true
msbuild.exe %PROGNAME%.sln /t:%PROGNAME% /p:Configuration="%REFL%" /p:Platform="%X32%"
msbuild.exe %PROGNAME%.sln /t:%PROGNAME% /p:Configuration="%REFL%" /p:Platform="%X64%"
msbuild.exe %PROGNAME%.sln /t:%PROGNAME% /p:Configuration="%SIMPLEDLL%" /p:Platform="%X32%"
msbuild.exe %PROGNAME%.sln /t:%PROGNAME% /p:Configuration="%SIMPLEDLL%" /p:Platform="%X64%"
