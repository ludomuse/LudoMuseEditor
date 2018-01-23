cd LudoMuse/
git init
git remote add origin https://github.com/ludomuse/Ludomuse.git
git fetch origin master
git reset --hard origin/master
rem %1 compile -p win32 -m release
mkdir win-build
cd win-build
cmake -G"NMake Makefiles" -DUSE_BULLET=0 -DUSE_RECAST=0 -DUSE_CHIPMUNK=0 -DCMAKE_CXX_FLAGS="-I %cd%/../prebuilt/win/include" ../
nmake
