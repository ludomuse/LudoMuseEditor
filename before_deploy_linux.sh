cp -r $TRAVIS_BUILD_DIR/../LudoMuse/bin/release/linux/{LudoMuse.exe,lib} $TRAVIS_BUILD_DIR/../LudoMuseEditor
rm $TRAVIS_BUILD_DIR/../LudoMuseEditor/{*.obj,*.o,*.h,*.cpp}
zip -r $HOME/LudoMuseEditor-linux.zip $TRAVIS_BUILD_DIR/../LudoMuseEditor/*
