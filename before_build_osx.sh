echo -e "machine github.com\n login $GITHUB_TOKEN" >> ~/.netrc
#git lfs pull
brew update
brew install qt@5.7 glfw
echo 'export PATH="/usr/local/opt/qt@5.7/bin:$PATH"' >> ~/.bash_profile
export PATH="/usr/local/opt/qt@5.7/bin:$PATH"
export LDFLAGS="-L/usr/local/opt/qt@5.7/lib"
export CPPFLAGS="-I/usr/local/opt/qt@5.7/include"

echo $PWD
./configure-cocos-osx.sh $PWD
stat $PWD/../LudoMuse/cocos2d
