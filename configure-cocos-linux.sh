#! /bin/bash

export PREF=$1/../
mkdir -p $PREF
cd $PREF
echo "downloading cocos 3.16 ..."
wget --quiet http://cdn.cocos2d-x.org/cocos2d-x-3.16.zip
echo "unzipping cocos 3.16"
unzip -q $PREF/cocos2d-x-3.16.zip -d $PREF
export COCOS=$PREF/cocos2d-x-3.16/tools/cocos2d-console/bin/cocos
$PREF/LudoMuseEditor/install_glfw.sh
echo "creating LudoMuse cocos project"
stat $COCOS
yes N | $COCOS new LudoMuse -l cpp -p com.IHMTEK.LudoMuse
$PREF/LudoMuseEditor/configure-LudoMuse-linux.sh $COCOS
cd $PREF/LudoMuseEditor
