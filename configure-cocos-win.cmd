set PREF=%1/../
cd %PREF%
echo "downloading cocos 3.16 ..."
curl -s -O http://cdn.cocos2d-x.org/cocos2d-x-3.16.zip
echo "unzipping cocos 3.16"
unzip -q %PREF%/cocos2d-x-3.16.zip -d %PREF%
set COCOS="%PREF%/cocos2d-x-3.16/tools/cocos2d-console/bin/cocos.py"
rem %PREF%/LudoMuseEditor/install_glfw.cmd
echo "creating LudoMuse cocos project"
echo N | python %COCOS% new LudoMuse -l cpp -p com.IHMTEK.LudoMuse
echo "configuring LudoMuse as editor dependency"
call "%PREF%/LudoMuseEditor/configure-LudoMuse-win.cmd" %COCOS%
cd "%PREF%/LudoMuseEditor"
