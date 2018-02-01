#export GCC_PATH=$(which g++)
#sudo rm $GCC_PATH
#sudo ln -s $(dirname GCC_PATH)/g++-7 $GCC_PATH
echo "starting build"
echo $TRAVIS_BUILD_DIR
#export TRAVIS_BUILD_DIR=/home/travis/build/ludomuse/LudoMuseEditor
echo "Build will use g++ version :"
export CXX=/usr/bin/g++-7
$CXX --version
cd libs/quazip/linux_build
rm *
/opt/qt58/bin/qmake ../quazip.pro CONFIG-=debug CONFIG+=release
make CXX=/usr/bin/g++-7

mkdir $TRAVIS_BUILD_DIR/../LudoMuseEditor_build
cd $TRAVIS_BUILD_DIR/../LudoMuseEditor_build
/opt/qt58/bin/qmake --version
/opt/qt58/bin/qmake CONFIG+=x86_64 CONFIG-=debug CONFIG+=release -config release $TRAVIS_BUILD_DIR/LudoMuseEditor.pro
make CXX=/usr/bin/g++-7 release
