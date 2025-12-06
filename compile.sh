i686-w64-mingw32-g++ -shared -o gen_oli.dll main.cpp  -I./include -municode  -static-libgcc -static-libstdc++
cp ./gen_oli.dll "/home/olivierd/winamp32/drive_c/Program Files/Winamp/Plugins/"

