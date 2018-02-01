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
$QT_PREFIX/bin/qmake --version
$QT_PREFIX/bin/qmake CONFIG+=x86_64 CONFIG-=debug CONFIG+=release -config release $TRAVIS_BUILD_DIR/LudoMuseEditor.pro
make LDFLAGS=$LDFLAGS CPPFLAGS=$CPPFLAGS
