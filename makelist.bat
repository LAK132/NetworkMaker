set CPPVER=c++latest

set APP=app.exe

set OUTDIR=out
set BINDIR=bin

set LIBDIR=lib
set LIBS=SDL2main.lib SDL2.lib

set COMPOPT=/nologo /EHa /MD /bigobj
set LINKOPT=/nologo

set DBGCOMPOPT=/Zi
set DBGLINKOPT=/DEBUG

set RELCOMPOPT=/DNDEBUG
set RELLINKOPT=

set SOURCES=lak lib lib_imgui lib_imgui_stl src 

set src_SRC=src
set src_OBJ=source.cpp nodegraph.cpp
set src_INC=include include/SDL lib/imgui

set lib_SRC=lib
set lib_OBJ=gl3w.c imgui_impl_sdl_gl3.cpp 
set lib_INC=include include/SDL lib/imgui

set lak_SRC=include/lak
set lak_OBJ=queue.cpp graphics.cpp json.cpp stream_util.cpp stride_vector.cpp
set lak_INC=include include/lak

set lib_imgui_stl_SRC=lib/imgui/misc/stl
set lib_imgui_stl_OBJ=imgui_stl.cpp
set lib_imgui_stl_INC=lib/imgui

set lib_imgui_SRC=lib/imgui
set lib_imgui_OBJ=imgui.cpp imgui_demo.cpp imgui_draw.cpp
set lib_imgui_INC=

if "%1"=="clean" goto :eof
call vcvarsall.bat %1