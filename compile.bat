:call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\Tools\vsvars32.bat"

cl /Iinclude %* lib\v8_base.lib lib\v8_snapshot.lib lib\pthreadVC2.lib "C:\Program Files (x86)\MinGW\lib\libws2_32.a" "C:\Program Files (x86)\MinGW\lib\libwsock32.a" "C:\Program Files (x86)\MinGW\lib\libwinmm.a" -lpthread