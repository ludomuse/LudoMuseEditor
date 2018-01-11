#! /bin/bash

wget http://www.cocos2d-x.org/filedown/cocos2d-x-3.16.zip
unzip cocos2d-x-3.16.zip
export COCOS=$PWD/cocos2d-x-3.16/tools/cocos2d-console/bin/cocos
mkdir -p ludomuse
cd ludomuse
$COCOS new LudoMuse -l cpp com.IHMTEK.LudoMuse
