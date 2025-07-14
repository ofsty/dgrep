#!/bin/bash

if ! command -v g++ &> /dev/null
then
    echo "g++ not found. Trying to install..."

    if [ -f /etc/debian_version ]; then
        sudo apt update && sudo apt install -y g++
    elif [ -f /etc/redhat-release ]; then
        sudo yum install -y gcc-c++
    else
        echo "Unsupported Linux distro. Please install g++ manually."
        exit 1
    fi
fi

g++ -std=c++17 -pthread src/bmh.cpp src/dgrep.cpp -o dgrep
mv dgrep /usr/local/bin/

if [ $? -eq 0 ]; then
    echo "Build successful! Executable: dgrep"
else
    echo "Build failed."
fi
