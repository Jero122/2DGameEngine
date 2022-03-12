call premake5.exe vs2019
if not exist "bin" mkdir "bin\Debug-windows-x86_64\AlmondNut"
if not exist "bin\Release-windows-x86_64" mkdir "bin\Release-windows-x86_64\AlmondNut"

copy SDL2.dll bin\Debug-windows-x86_64\AlmondNut
copy SDL2.dll bin\Release-windows-x86_64\AlmondNut
PAUSE