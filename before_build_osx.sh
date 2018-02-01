echo -e "machine github.com\n login $GITHUB_TOKEN" >> ~/.netrc
#git lfs pull
brew update
brew install qt glfw
echo 'export PATH="$QT_PREFIX/bin:$PATH"' >> ~/.bash_profile
export PATH="$QT_PREFIX/bin:$PATH"
export LDFLAGS="-L$QT_PREFIX/lib"
export CPPFLAGS="-I$QT_PREFIX/include"

echo $PWD
./configure-cocos-osx.sh $PWD
stat $PWD/../LudoMuse/cocos2d
