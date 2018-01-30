echo -e "machine github.com\n login $GITHUB_TOKEN" >> ~/.netrc
#git lfs pull
brew update
echo $PWD
./configure-cocos-osx.sh $PWD
stat $PWD/../LudoMuse/cocos2d
