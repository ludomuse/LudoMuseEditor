#cd libs/quazip/mac_build
#rm *
#/opt/qt58/bin/qmake ../quazip.pro CONFIG-=debug CONFIG+=release
#make
#cd ../../../../
cd ../
mkdir LudoMuseEditorLinux
cd LudoMuseEditorLinux
qmake --version
qmake CONFIG+=x86_64 CONFIG-=debug CONFIG+=release $TRAVIS_BUILD_DIR/LudoMuseEditor.pro 
make LDFLAGS=$LDFLAGS CPPFLAGS=$CPPFLAGS
