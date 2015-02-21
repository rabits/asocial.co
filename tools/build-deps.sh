#!/bin/sh
# Drone.io build script
export PATH=/opt/Qt/5.4/gcc_64/bin:$PATH

# Set c++11 usage
echo 2 | sudo update-alternatives --config gcc

# Checkout submodules
git submodule update --init --recursive

# Download qt5 package & unpack it
curl -Lo ~/qt-online.run 'http://download.qt-project.org/official_releases/online_installers/qt-opensource-linux-x64-online.run'
chmod +x ~/qt-online.run

curl -Lo ~/essentials.7z http://download.qt-project.org/online/qtsdkrepository/linux_x64/desktop/qt5_54/qt.54.gcc_64/5.4.0-1qt5_essentials.7z
curl -Lo ~/addons.7z http://download.qt-project.org/online/qtsdkrepository/linux_x64/desktop/qt5_54/qt.54.gcc_64/5.4.0-1qt5_addons.7z
curl -Lo ~/icu.7z http://download.qt-project.org/online/qtsdkrepository/linux_x64/desktop/qt5_54/qt.54.gcc_64/5.4.0-1icu_53_1_ubuntu_11_10_64.7z

sudo mkdir /opt/Qt
sudo apt-get install -qq p7zip --no-install-recommends
sudo 7zr -o/opt/Qt x ~/essentials.7z > /dev/null
sudo 7zr -o/opt/Qt x ~/addons.7z > /dev/null
sudo 7zr -o/opt/Qt x ~/icu.7z > /dev/null

sudo ~/qt-online.run --runoperation QtPatch linux /opt/Qt/5.4/gcc_64 qt5

# Build binaries
qmake asocial.co.pro
make -j4
