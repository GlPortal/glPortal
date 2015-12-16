# Quick note if you are compiling for the first time
Well done. You made the right decision. It's not even that hard.

# Dependencies
**Note:** Dependencies will change all the time so watch the cmake output carefuly for missing libraries.
Make sure to check CmakeList.txt if is something missing.

## Upgrade your os
You will need very recent versions of all the dependencies. So please upgrade your os before proceeding.
[upgrade ubuntu](http://www.ubuntu.com/download/desktop/upgrade)

## List of dependencies
- assimp
- epoxy
- sdl2
- sdl2-mixer
- bullet (v 2.82+)
- tinyxml2
- libgl
- cmake *(make)*
- make *(make)*
- gcc *(make)*
- pkg-config *(make)*
- git *(download source code)*

## Installing dependencies
Remember, you have to be root, so you might want to prefix `sudo`.

### Arch Linux
**Note:** Package `tinyxml2` can be found in AUR.
```bash
pacman -S cmake make gcc pkg-config assimp libepoxy sdl2 sdl2-mixer bullet mesa
```

### Debian 8 / Ubuntu 14.04 / Linux Mint 17.x
```bash
apt-get install cmake make gcc pkg-config libassimp-dev libepoxy-dev libsdl2-dev libsdl2-mixer-dev libbullet-dev libtinyxml2-dev libgl1-mesa-dev
```

### Others
Now how will you satisfy these dependencies? No problem. You might notice that when you
are looking for them in your package manager that you get overwhelmed by a list of possible
installation candidates, how do you know which one is the right one?

An easy rule that applies to many libraries is that a library has the lib prefix and then, 
since you want to install the files for development there is a dev in the name. 
Pretty much like libfoo-dev for most libraries.

# Downloading source code
```bash
git clone https://github.com/GlPortal/glPortal.git
cd glPortal
git submodule init
git submodule update
```

# Building the binary
Make sure to check the README.org and COMPILE.org from the release you downloaded in case
there are differences in the compile workflow for it.

In the root directory of the downloaded source type:
```bash
mkdir build && cd build
cmake ../
```
If you want to profile the source code pass *-DCMAKE_CXX_FLAGS=-pg* to cmake.   
Now if this throws an error you have to fix something first. If you don't manage to fix the error, use the
bugtracker https://github.com/GlPortal/glPortal/issues or http://bugs.glportal.de to report what you did, and what error you got.

If the command did not produce an error, you can build the binary by typing in:
```bash
make
```

If this produces no error you have built the binary and should be able to start GlPortal by typing in:
```bash
make run
```

If you get errors try to build GlPortal again. If you don't manage to fix the error, use the
bugtracker https://github.com/GlPortal/glPortal/issues or http://bugs.glportal.de to report what you did, and what error you got.

# Keep us posted
Not working? No worries! Help us help you figure out what we missed to make this work on 
your system by opening an issue.
