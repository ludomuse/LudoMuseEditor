cd LudoMuse/
git init
git remote add origin https://github.com/ludomuse/Ludomuse.git
git fetch origin master
git reset --hard origin/master
rem %1 compile -p win32 -m release
rem mkdir win-build
rem cd win-build
rem cmake -G"NMake Makefiles" -DUSE_BULLET=0 -DUSE_RECAST=0 -DUSE_CHIPMUNK=0 -DCMAKE_CXX_FLAGS="-I %cd%/../prebuilt/win/include" ../
rem nmake
msbuild proj.win32/LudoMuse.sln /t:Build /p:Platform="Win32" /p:Configuration="Release" /m
rem type C:\projects\LudoMuse\proj.win32\UpgradeLog2.htm
