mkdir $HOME/LudoMuseEditorLinux
cd $TRAVIS_BUILD_DIR/../LudoMuseEditor_build
make INSTALL_ROOT=$HOME/LudoMuseEditorLinux -j$(nproc) install ; find $HOME/LudoMuseEditorLinux/

# cp -r $TRAVIS_BUILD_DIR/../LudoMuse/bin/release/linux/{LudoMuse.exe,lib,Resources} $HOME/LudoMuseEditorLinux
# cp -r $TRAVIS_BUILD_DIR/{ludomuse.png,LudoMuseEditor.desktop} $HOME/LudoMuseEditorLinux
# rm $TRAVIS_BUILD_DIR/../LudoMuseEditor_build/{*.o,*.h,*.cpp,Makefile}
# cp -r $TRAVIS_BUILD_DIR/buildFiles/* $HOME/LudoMuseEditorLinux

curl -s "https://github-production-release-asset-2e65be.s3.amazonaws.com/67432158/4a731c68-0506-11e8-86a2-9c89c14f0941?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAIWNJYAX4CSVEH53A%2F20180130%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20180130T095903Z&X-Amz-Expires=300&X-Amz-Signature=d8aed99e265db81eb8e45306ef82528b9007a689bad8495a217a9d0632214dbd&X-Amz-SignedHeaders=host&actor_id=2983006&response-content-disposition=attachment%3B%20filename%3Dlinuxdeployqt-continuous-x86_64.AppImage&response-content-type=application%2Foctet-stream" > linuxdeployqt.AppImage
chmod a+x linuxdeployqt.AppImage

cd $HOME

unset QTDIR; unset QT_PLUGIN_PATH ; unset LD_LIBRARY_PATH
export VERSION=$TRAVIS_BUILD_NUMBER # linuxdeployqt uses this for naming the file

./linuxdeployqt.AppImage $HOME/LudoMuseEditorLinux/*.desktop -bundle-non-qt-libs
./linuxdeployqt.AppImage $HOME/LudoMuseEditorLinux/*.desktop -appimage

curl -s "https://ihmtek-services.com/files/LudoMuse/video.mp4" > $HOME/LudoMuseEditorLinux/default/cache/video.mp4


# get libs dependencies
export CPY="libQt5OpenGL.so.5 libQt5Widgets.so.5 libQt5Gui.so.5 libQt5Core.so.5 libpng12.so.0 libGLEW.so.1.10 libglfw.so.3"

export LIBS=$(ldd $HOME/LudoMuseEditorLinux/LudoMuseEditor | grep " => /" | cut -d " " -f 3-3 | sort | uniq)

for VPATH in $LIBS; do
    for VNAME in $CPY; do
	if [[ $VPATH = *$VNAME* ]]; then
	    cp $VPATH $HOME/LudoMuseEditorLinux/lib/
	fi
    done;
done;


zip -r $HOME/LudoMuseEditor-linux.zip LudoMuseEditorLinux/*
