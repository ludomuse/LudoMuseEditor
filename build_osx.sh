#cd libs/quazip/mac_build
#rm *
#/opt/qt58/bin/qmake ../quazip.pro CONFIG-=debug CONFIG+=release
#make
#cd ../../../../
cd ../
mkdir LudoMuseEditorLinux
cd LudoMuseEditorLinux
qmake --version
qmake ../LudoMuseEditor/LudoMuseEditor.pro
make