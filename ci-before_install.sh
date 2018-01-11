#! /bin/bash

echo "Before install path : $PWD"
cd ../
echo "downloading cocos 3.16 ..."
travis_wait wget --quiet http://cdn.cocos2d-x.org/cocos2d-x-3.16.zip
echo "unzipping cocos 3.16"
travis_wait unzip -q cocos2d-x-3.16.zip
export COCOS=$PWD/cocos2d-x-3.16/tools/cocos2d-console/bin/cocos
echo "creating LudoMuse cocos project"
$COCOS new LudoMuse -l cpp com.IHMTEK.LudoMuse
cd LudoMuseEditor

