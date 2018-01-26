echo -e "machine github.com\n login $GITHUB_TOKEN" >> ~/.netrc
#git lfs pull
sudo add-apt-repository --yes ppa:ubuntu-sdk-team/ppa
sudo add-apt-repository ppa:beineri/opt-qt58-trusty -y
#- sudo add-apt-repository --yes ppa:pyglfw/pyglfw
sudo apt-get update -qq
#- sudo apt-get install qtbase5-dev qtdeclarative5-dev libqt5webkit5-dev libsqlite3-dev
sudo apt-get install qt58base qt58svg qt58charts-no-lgpl qt58xmlpatterns
source /opt/qt58/bin/qt58-env.sh
sudo apt-get install -y libxrandr-dev libxi-dev libxxf86vm-dev libgtk-3-dev
echo $PWD
./configure-cocos-linux.sh $PWD
stat $PWD/../LudoMuse/cocos2d