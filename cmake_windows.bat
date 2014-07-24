set lib_path=C:\vs_libs
set glew_version=1.10.0
set sdl2_version=2.0.3

cmake^
 -DCMAKE_INCLUDE_PATH=^
%lib_path%\glew-%glew_version%\include;^
%lib_path%\SDL2-%sdl2_version%\include^
 -DCMAKE_LIBRARY_PATH=^
%lib_path%\glew-%glew_version%\lib\Release\x64;^
%lib_path%\glew-%glew_version%\lib\Release\Win32;^
%lib_path%\SDL2-%sdl2_version%\lib\x64;^
%lib_path%\SDL2-%sdl2_version%\lib\x86^
 %~dp0
