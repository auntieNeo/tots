mkdir %~dp0\vs_libs
set lib_path=%~dp0\vs_libs
set boost_version=1_55_0
set glew_version=1.10.0
set sdl2_version=2.0.3

set SDL2DIR=%lib_path%\SDL2-%sdl2_version%

cmake^
 -DCMAKE_INCLUDE_PATH=^
%lib_path%\glew-%glew_version%\include;^
%lib_path%\SDL2-%sdl2_version%\include^
 -DCMAKE_LIBRARY_PATH=^
%lib_path%\glew-%glew_version%\lib\Release\Win32;^
%lib_path%\SDL2-%sdl2_version%\lib^
 -DBOOST_INCLUDEDIR=^
%lib_path%\boost_%boost_version%\include^
 -DBOOST_LIBRARYDIR=^
%lib_path%\boost_%boost_version%\stage\lib^
 %~dp0
