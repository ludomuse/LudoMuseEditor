cd LudoMuse/
git init
git remote add origin https://github.com/ludomuse/Ludomuse.git
git fetch origin master
git reset --hard origin/master
# weird hack with libbullet
# ./doBulletHack.sh
mkdir -p linux-build
cd linux-build
cmake ../ -DUSE_BULLET=0
$1 compile -p linux
./changeExeRPATH.sh
