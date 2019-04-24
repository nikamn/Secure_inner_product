#!/usr/bin/env bash
set -e
if test $(id -u) != 0 ; then
    SUDO=sudo
fi

if [ ! -d deps ]; then
    mkdir deps
fi

cd deps

if [ ! -d gmp-6.1.2 ]; then
    if [ ! -f gmp-6.1.2.tar.lz ]; then
	wget https://gmplib.org/download/gmp/gmp-6.1.2.tar.lz
	sudo apt install lzip
	tar --lzip -xf gmp-6.1.2.tar.lz
    fi
fi

cd gmp-6.1.2
sudo apt-get install m4
./configure && make && sudo make install && make check

if [ ! -d ntl-10.5.0 ]; then
    if [ ! -f ntl-10.5.0.tar.gz ]; then
	wget http://www.shoup.net/ntl/ntl-10.5.0.tar.gz
	tar -xf ntl-10.5.0.tar.gz
    fi
fi

cd ntl-10.5.0/src
./configure && make && make check && sudo make install

if [ ! $? -eq 0 ];
then
    echo "Unable to configure NTL library" > /dev/stderr
    exit 1
fi

cd -


