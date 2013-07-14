Hobo
====

Hobo is a clone of an old flash game called Nanaca Crash. (bing it!) It uses the Nanaka game engine and is developed mostly for the purpose of developing Nanaka in some sort of context.

Fetch
-----

By git: (need a github account)

    git clone --recursive git@github.com:mathall/hobo.git

By https:

    git clone https://github.com/mathall/hobo
    
    cd hobo
    rm -rf nanaka # submodules over https seem to not work so well..
    git clone https://github.com/mathall/nanaka

Build and run (linux)
---------------------

First make sure your setup meets the prerequisites for building Nanaka.

    cd platforms/linux/build
    make all
    cd ../../../out_linux_x86/Debug
    ./chuckachicken

Legal
-----

See LICENSE.
