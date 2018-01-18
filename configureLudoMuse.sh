cd LudoMuse/
git init
git remote add origin https://github.com/ludomuse/Ludomuse.git
git fetch origin master
git reset --hard origin/master
# TODO : weird hack with libbullet
./doBulletHack.sh
$1 compile -p linux
./changeExeRPATH.sh
