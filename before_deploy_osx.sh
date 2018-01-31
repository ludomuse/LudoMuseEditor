
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


# Qt fixing
# install_name_tool -id @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore $HOME/LudoMuseEditor.app/Contents/Frameworks/QtCore.framework/Versions/5/QtCore
# install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui $HOME/LudoMuseEditor.app/Contents/Frameworks/QtGui.framework/Versions/5/QtGui
# install_name_tool -id @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets $HOME/LudoMuseEditor.app/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets
# install_name_tool -id @executable_path/../Frameworks/QtOpenGL.framework/Versions/5/QtOpenGL $HOME/LudoMuseEditor.app/Contents/Frameworks/QtOpenGL.framework/Versions/5/QtOpenGL

# QtGui
install_name_tool -change /usr/local/Cellar/qt@5.7/5.7.1/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore $HOME/LudoMuseEditor.app/Contents/Frameworks/QtGui.framework/Versions/5/QtGui

# QtWidgets
install_name_tool -change /usr/local/Cellar/qt@5.7/5.7.1/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui $HOME/LudoMuseEditor.app/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets
install_name_tool -change /usr/local/Cellar/qt@5.7/5.7.1/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore $HOME/LudoMuseEditor.app/Contents/Frameworks/QWidgets.framework/Versions/5/QWidgets

# QtOpenGL
install_name_tool -change /usr/local/Cellar/qt@5.7/5.7.1/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore $HOME/LudoMuseEditor.app/Contents/Frameworks/QtOpenGL.framework/Versions/5/QtOpenGL
install_name_tool -change /usr/local/Cellar/qt@5.7/5.7.1/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui $HOME/LudoMuseEditor.app/Contents/Frameworks/QtOpenGL.framework/Versions/5/QtOpenGL
install_name_tool -change /usr/local/Cellar/qt@5.7/5.7.1/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets $HOME/LudoMuseEditor.app/Contents/Frameworks/QtOpenGL.framework/Versions/5/QtOpenGL


zip -r $HOME/LudoMuseEditor-osx.zip LudoMuseEditor.app
