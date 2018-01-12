cocos new LudoMuse -l cpp -p com.IHMTEK.LudoMuse
cd LudoMuse/
git init
git remote add origin https://github.com/ludomuse/Ludomuse.git
git fetch origin master
git reset --hard origin/master
# TODO : weird hack with libbullet
cocos compile -p linux
bash changeExeRPATH.sh
mkdir bin/debug/linux/lib
mv bin/debug/linux/libfmod.so.6 bin/debug/linux/lib
