mkdir $HOME/LudoMuseEditorLinux
cd $TRAVIS_BUILD_DIR/../LudoMuseEditor_build
make INSTALL_ROOT=$HOME/LudoMuseEditorLinux -j$(nproc) install ; find $HOME/LudoMuseEditorLinux/

# cp -r $TRAVIS_BUILD_DIR/../LudoMuse/bin/release/linux/{LudoMuse.exe,lib,Resources} $HOME/LudoMuseEditorLinux
# cp -r $TRAVIS_BUILD_DIR/{ludomuse.png,LudoMuseEditor.desktop} $HOME/LudoMuseEditorLinux
# rm $TRAVIS_BUILD_DIR/../LudoMuseEditor_build/{*.o,*.h,*.cpp,Makefile}
# cp -r $TRAVIS_BUILD_DIR/buildFiles/* $HOME/LudoMuseEditorLinux


cd $HOME


curl -s "https://ihmtek-services.com/files/LudoMuse/video.mp4" > $HOME/LudoMuseEditorLinux/default/cache/video.mp4

# echo "locating xcb"
# sudo updatedb
# locate libqxcb

# mkdir -p $HOME/LudoMuseEditorLinux/plugins/{platforms,xcbglintegrations}

# cp /opt/qt58/plugins/platforms/libqxcb.so $HOME/LudoMuseEditorLinux/plugins/platforms/
# cp /opt/qt58/plugins/xcbglintegrations/libqxcb-glx-integration.so $HOME/LudoMuseEditorLinux/plugins/xcbglintegrations/


# get libs dependencies
# export CPY="libQt5OpenGL.so.5 libQt5Widgets.so.5 libQt5Gui.so.5 libQt5Core.so.5 libQt5XcbQpa.so.5 libQt5DBus.so.5 libpng12.so.0 libGLEW.so.1.10 libglfw.so.3 libicui18n.so.52 libicuuc.so.52 libicudata.so.52"
export CPY="libpng12.so.0 libGLEW.so.1.10 libglfw.so.3"

export LIBS=$(ldd $HOME/LudoMuseEditorLinux/LudoMuseEditor | grep " => /" | cut -d " " -f 3-3 | sort | uniq)

for VPATH in $LIBS; do
    for VNAME in $CPY; do
	if [[ $VPATH = *$VNAME* ]]; then
	    cp $VPATH $HOME/LudoMuseEditorLinux/lib/
	fi
    done;
done;


wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"

chmod a+x linuxdeployqt-continuous-x86_64.AppImage


unset QTDIR; unset QT_PLUGIN_PATH ; unset LD_LIBRARY_PATH
# export VERSION=$TRAVIS_BUILD_NUMBER # linuxdeployqt uses this for naming the file

cd $HOME/LudoMuseEditorLinux

../linuxdeployqt-continuous-x86_64.AppImage LudoMuseEditor -qmake=/opt/qt58/bin/qmake #-bundle-non-qt-libs

zip -r $HOME/LudoMuseEditor-linux.zip ./*

mkdir -p $TRAVIS_BUILD_DIR/deploy/Installer/packages/com.ihmtek.ludomuseeditor/data
cp -r ./* $TRAVIS_BUILD_DIR/deploy/Installer/packages/com.ihmtek.ludomuseeditor/data/


echo "downloading binary creator to create linux installer"
curl -s  http://ihmtek-services.com/files/LudoMuse/binarycreator > $HOME/binarycreator
chmod +x $HOME/binarycreator
cd $TRAVIS_BUILD_DIR/deploy/Installer/
$HOME/binarycreator -c config/config.xml -p packages $HOME/LudoMuseEditorLinux_Setup

curl -u $UPLOAD_USER:$UPLOAD_PWD -T $HOME/LudoMuseEditorLinux_Setup $UPLOAD_SERVER/LudoMuseEditor/LudoMuseEditor-$TRAVIS_OS_NAME-$TRAVIS_BUILD_NUMBER-installer.zip

# cd $HOME
# $HOME/linuxdeployqt-continuous-x86_64.AppImage $HOME/LudoMuseEditorLinux/*.desktop -appimage -qmake=/opt/qt58/bin/qmake
