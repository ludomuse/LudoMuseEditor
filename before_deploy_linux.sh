cp -r $TRAVIS_BUILD_DIR/../LudoMuse/bin/release/linux/{LudoMuse.exe,lib,Resources} $TRAVIS_BUILD_DIR/../LudoMuseEditor_build
rm $TRAVIS_BUILD_DIR/../LudoMuseEditor_build/{*.obj,*.o,*.h,*.cpp}
cp -r $TRAVIS_BUILD_DIR/buildFiles/* $TRAVIS_BUILD_DIR/../LudoMuseEditor_build

curl -s -O "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
chmod a+x linuxdeployqt*.AppImage
./linuxdeployqt*.AppImage $TRAVIS_BUILD_DIR/../LudoMuseEditor_build/LudoMuseEditor -bundle-non-qt-libs
./linuxdeployqt*.AppImage $TRAVIS_BUILD_DIR/../LudoMuseEditor_build/LudoMuseEditor -appimage

zip -r $HOME/LudoMuseEditor-linux.zip $TRAVIS_BUILD_DIR/../LudoMuseEditor_build
