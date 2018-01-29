#export GCC_PATH=$(which g++)
#sudo rm $GCC_PATH
#sudo ln -s $(dirname GCC_PATH)/g++-7 $GCC_PATH
echo "startig build"
echo $TRAVIS_BUILD_DIR
#export TRAVIS_BUILD_DIR=/home/travis/build/ludomuse/LudoMuseEditor
echo "Build will use g++ version :"
export CXX=/usr/bin/g++-7
$CXX --version
cd libs/quazip/linux_build
rm *
/opt/qt58/bin/qmake ../quazip.pro CONFIG-=debug CONFIG+=release
make CXX=/usr/bin/g++-7
cd ../../../../
mkdir $HOME/LudoMuseEditor_build
cd $HOME/LudoMuseEditor_build
/opt/qt58/bin/qmake --version
/opt/qt58/bin/qmake CONFIG+=x86_64 CONFIG-=debug CONFIG+=release "LUDOMUSE_PATH=${TRAVIS_BUILD_DIR}/../LudoMuse/" $TRAVIS_BUILD_DIR/LudoMuseEditor.pro
make CXX=/usr/bin/g++-7
