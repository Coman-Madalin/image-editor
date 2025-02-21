#!/bin/bash
# Copyright 2020 Darius Neatu (neatudarius@gmail.com)

sudo apt-get update

sudo apt-get install                        \
    python3 python3-pip                     \
    perl                                    \
    imagemagick                             \
    rustc cargo                             \
    make gdb valgrind

# all py packages json os pathlib recordclass subprocess sys
sudo pip3 install                           \
    argparse                                \
    pathlib                                 \
    recordclass

cargo build --release
cp target/release/imagecheck image_check