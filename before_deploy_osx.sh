
cd $TRAVIS_BUILD_DIR/../LudoMuseEditor_build
make INSTALL_ROOT=$HOME/ -j$(nproc) install 

/usr/local/opt/qt@5.7/bin/macdeployqt $HOME/LudoMuseEditor.app

cd $HOME

curl -s "https://ihmtek-services.com/files/LudoMuse/video.mp4" > $HOME/LudoMuseEditor.app/Contents/MacOS/default/cache/video.mp4


cp $TRAVIS_BUILD_DIR/libs/quazip/mac_build/libquazip.1.dylib $HOME/LudoMuseEditor.app/Contents/Frameworks/

install_name_tool -change libquazip.1.dylib @executable_path/../Frameworks/libquazip.1.dylib $HOME/LudoMuseEditor.app/Contents/MacOS/LudoMuseEditor

#cp -r $TRAVIS_BUILD_DIR/deploy/common/* 


zip -r $HOME/LudoMuseEditor-osx.zip LudoMuseEditor.app
