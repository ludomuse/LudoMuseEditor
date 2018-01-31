
cd $TRAVIS_BUILD_DIR/../LudoMuseEditor_build
make INSTALL_ROOT=$HOME/ -j$(nproc) install 

/usr/local/opt/qt@5.7/bin/macdeployqt $HOME/LudoMuseEditor.app

cd $HOME

curl -s "https://ihmtek-services.com/files/LudoMuse/video.mp4" > $HOME/LudoMuseEditor.app/Contents/MacOS/default/cache/video.mp4


cp $TRAVIS_BUILD_DIR/libs/quazip/mac_build/libquazip.1.dylib $HOME/LudoMuseEditor.app/Contents/Frameworks/
cp /usr/local/lib/libglfw.3.dylib $HOME/LudoMuseEditor.app/Contents/Frameworks/

cp -r $TRAVIS_BUILD_DIR/../LudoMuse/Resources $HOME/LudoMuseEditor.app/Contents/
cp $TRAVIS_BUILD_DIR/../LudoMuse/bin/release/mac/LudoMuse.app/Contents/MacOS/LudoMuse $HOME/LudoMuseEditor.app/Contents/MacOS
cp -r $TRAVIS_BUILD_DIR/deploy/common/* $HOME/LudoMuseEditor.app/Contents/MacOS


install_name_tool -change libquazip.1.dylib @executable_path/../Frameworks/libquazip.1.dylib $HOME/LudoMuseEditor.app/Contents/MacOS/LudoMuseEditor
install_name_tool -change lib/libglfw.3.dylib @executable_path/../Frameworks/libglfw.3.dylib $HOME/LudoMuseEditor.app/Contents/MacOS/LudoMuseEditor


zip -r $HOME/LudoMuseEditor-osx.zip LudoMuseEditor.app
