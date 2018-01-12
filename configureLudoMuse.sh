cd LudoMuse/
git init
git remote add origin https://github.com/ludomuse/Ludomuse.git
git fetch origin master
git reset --hard origin/master
# TODO : weird hack with libbullet
./doBulletHack.sh
cocos compile -p linux
./changeExeRPATH.sh
mkdir bin/debug/linux/lib
cp prebuilt/libfmod.so.6 bin/debug/linux/lib
