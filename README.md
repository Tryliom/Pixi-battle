# Install WSL 2

Install wsl2 from Windows store (Ubuntu 22.04)

# Install libs

## Setup

```
sudo apt-get update && sudo apt-get upgrade
```

Reboot

```
sudo apt-get install -y libopus0 g++ cmake pkg-config p7zip-full zlib1g zlib1g-dev
```

## Setup CLion

In wsl, install gdb and gcc

```
sudo apt install -y gcc gdb
```

Go in CLion -> settings -> Build, execution, .. -> toolchains, add a new one.

Select WSL.

## Install OpenSSL 3

Check the latest version with this command:

```
openssl version -a
```

If you have a version < 3.0.2, run these commands:

```
sudo apt update && sudo apt upgrade -y
```

Reboot WSL.

```
sudo apt install build-essential checkinstall zlib1g-dev -y
sudo apt autoremove -y
cd /usr/local/src/
sudo wget https://www.openssl.org/source/openssl-3.0.2.tar.gz
sudo tar -xvf openssl-3.0.2.tar.gz
cd openssl-3.0.2
sudo ./config --prefix=/usr/local/ssl --openssldir=/usr/local/ssl shared zlib
sudo make
sudo make test
sudo make install
cd /etc/ld.so.conf.d/
sudo vi openssl-3.0.2.conf
```

Add `/usr/local/ssl/lib64` to the file, use ctrl + c and :wq to saveExistingUsers and quit.

```
sudo ldconfig -v
sudo mv /usr/bin/c_rehash /usr/bin/c_rehash.BEKUP
sudo mv /usr/bin/openssl /usr/bin/openssl.BEKUP
sudo vi /etc/environment
```

Add `/usr/local/ssl/bin` at the end of the path, use ctrl + c and :wq to saveExistingUsers and quit.

```
source /etc/environment
echo $PATH
```

## Install DPP

```
cd ~
wget -O libdpp.deb https://dl.dpp.dev/latest
sudo dpkg -i libdpp.deb
```

## Install GraphicsMagick

Need to install jxl before

```
sudo apt install -y clang clang-12 libbrotli-dev libgflags-dev libgif-dev libjpeg-dev libopenexr-dev libpng-dev libwebp-dev ghostscript libfreetype-dev
cd ~
git clone https://github.com/libjxl/libjxl.git --recursive --shallow-submodules
cd ./libjxl && mkdir build && cd ./build
export CC=clang-12 CXX=clang++-12
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=/usr/local ..
cmake --build . -- -j$(nproc)
sudo make install
```

Now, GraphicsMagick

```
cd ~
wget -O graphicsmagick.7z https://downloads.sourceforge.net/project/graphicsmagick/graphicsmagick/1.3.38/GraphicsMagick-1.3.38-windows.7z?ts=gAAAAABirFamMJ_jb5WdzVvOOmzG7xHPN8rQAQqBe2jr665wA0CfkkaaURGNlmg1HbUMVfihocjfWVGpJzVf8j7PfFj-mVZFAQ%3D%3D&r=https%3A%2F%2Fsourceforge.net%2Fprojects%2Fgraphicsmagick%2Ffiles%2Flatest%2Fdownload
7za x graphicsmagick.7z && cd GraphicsMagick-1.3.38

./configure --enable-shared=yes --enable-static=no
make && sudo make install
sudo ldconfig
```

## Error fix

### libcuda.so.1 is not a symbolic link on WSL2

Open powershell as admin and run `cmd` or cmd in admin mod

```
cd \Windows\System32\lxss\lib
del libcuda.so
del libcuda.so.1
```

Work on Cmder but not powershell
```
mklink libcuda.so libcuda.so.1.1
mklink libcuda.so.1 libcuda.so.1.1
```

Rerun the command that give this error to finish it