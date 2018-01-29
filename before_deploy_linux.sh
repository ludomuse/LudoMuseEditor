cp -r $TRAVIS_BUILD_DIR/../LudoMuse/bin/release/linux/{LudoMuse.exe,lib,Resources} $HOME/LudoMuseEditor_build
rm $HOME/LudoMuseEditor_build/{*.obj,*.o,*.h,*.cpp}
cp -r $TRAVIS_BUILD_DIR/buildFiles/* $HOME/LudoMuseEditor_build

curl -s -O "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
chmod a+x linuxdeployqt*.AppImage
./linuxdeployqt*.AppImage $HOME/LudoMuseEditor_build/LudoMuseEditor -bundle-non-qt-libs
./linuxdeployqt*.AppImage $HOME/LudoMuseEditor_build/LudoMuseEditor -appimage

zip -r $HOME/LudoMuseEditor-linux.zip $HOME/LudoMuseEditor_build
