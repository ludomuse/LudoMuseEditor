#cd libs/quazip/mac_build
#rm *
#/opt/qt58/bin/qmake ../quazip.pro CONFIG-=debug CONFIG+=release
#make
#cd ../../../../
cd ../
mkdir LudoMuseEditor_build
cd LudoMuseEditor_build
echo "starting build with env"
echo $LDFLAGS
echo $CPPFLAGS
/usr/local/opt/qt@5.7/bin/qmake --version
/usr/local/opt/qt@5.7/bin/qmake CONFIG+=x86_64 CONFIG-=debug CONFIG+=release -config release $TRAVIS_BUILD_DIR/LudoMuseEditor.pro
make LDFLAGS=$LDFLAGS CPPFLAGS=$CPPFLAGS release
