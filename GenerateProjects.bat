call vendor\premake\premake5.exe vs2022
if not exist "bin" mkdir "bin\Debug-windows-x86_64\AlmondNut"
if not exist "bin\Release-windows-x86_64" mkdir "bin\Release-windows-x86_64\AlmondNut"

copy SDL2.dll bin\Debug-windows-x86_64\AlmondNut
copy SDL2.dll bin\Release-windows-x86_64\AlmondNut

copy assimp-vc143-mt.dll bin\Debug-windows-x86_64\AlmondNut
copy assimp-vc143-mt.dll bin\Release-windows-x86_64\AlmondNut

xcopy AlmondNut\assets bin\Release-windows-x86_64\AlmondNut\assets /E /i
xcopy AlmondNut\assets bin\Debug-windows-x86_64\AlmondNut\assets /E /i
copy AlmondNut\imgui.ini bin\Release-windows-x86_64\AlmondNut\
copy AlmondNut\imgui.ini bin\Debug-windows-x86_64\AlmondNut\
PAUSE