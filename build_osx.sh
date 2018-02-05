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
sudo /usr/libexec/locate.updatedb
locate qmake
$QT_PREFIX/bin/qmake --version
$QT_PREFIX/bin/qmake DEPLOY=release $TRAVIS_BUILD_DIR/LudoMuseEditor.pro
make LDFLAGS=$LDFLAGS CPPFLAGS=$CPPFLAGS
