@set /p version=Please input version:

set pack_directory=%~dp0

cd %pack_directory%PiTool

@REM powercmd bug
rename D:\Tools\PowerCmd\CmdLink32.exe  CmdLink32.exe_

python build.py release && cd %pack_directory% && python pack.py toC %version%

cd %pack_directory%
rename D:\Tools\PowerCmd\CmdLink32.exe_  CmdLink32.exe