echo -e "machine github.com\n login $GITHUB_TOKEN" >> ~/.netrc
#git lfs pull
brew update
brew install qt@5.7
echo $PWD
./configure-cocos-osx.sh $PWD
stat $PWD/../LudoMuse/cocos2d
