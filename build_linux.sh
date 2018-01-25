#export GCC_PATH=$(which g++)
#sudo rm $GCC_PATH
#sudo ln -s $(dirname GCC_PATH)/g++-7 $GCC_PATH
echo "Build will use g++ version :"
/usr/bin/g++7 --version
cd libs/quazip/linux_build
rm *
/opt/qt58/bin/qmake ../quazip.pro CONFIG-=debug CONFIG+=release
make CXX=/usr/bin/g++7
cd ../../../../
mkdir LudoMuseEditorLinux
cd LudoMuseEditorLinux
/opt/qt58/bin/qmake --version
/opt/qt58/bin/qmake ../LudoMuseEditor/LudoMuseEditor.pro
make CXX=/usr/bin/g++7