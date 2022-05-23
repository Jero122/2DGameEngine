call premake5.exe vs2022
if not exist "bin" mkdir "bin\Debug-windows-x86_64\AlmondNut"
if not exist "bin\Release-windows-x86_64" mkdir "bin\Release-windows-x86_64\AlmondNut"

copy SDL2.dll bin\Debug-windows-x86_64\AlmondNut
copy SDL2.dll bin\Release-windows-x86_64\AlmondNut
xcopy AlmondNut\resources bin\Release-windows-x86_64\AlmondNut\resources /E /i
xcopy AlmondNut\resources bin\Debug-windows-x86_64\AlmondNut\resources /E /i
copy AlmondNut\imgui.ini bin\Release-windows-x86_64\AlmondNut\
copy AlmondNut\imgui.ini bin\Debug-windows-x86_64\AlmondNut\
PAUSE