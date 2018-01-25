echo -e "machine github.com\n login $GITHUB_TOKEN" >> ~/.netrc
#git lfs pull
brew update
echo $PWD
travis_wait ./configure-cocos-linux.sh $PWD
stat $PWD/../LudoMuse/cocos2d