# Compile Guide for GlPortal

## Dependencies
**NOTE:** Dependencies will change all the time so watch the cmake output carefuly for missing libraries.
Make sure to check CMakeLists.txt if it is something missing.

## Upgrade your OS
You will need very recent versions of all the dependencies. So please upgrade your OS before proceeding.
[Upgrade Ubuntu](http://www.ubuntu.com/download/desktop/upgrade)

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
- libunittest++

## Installing dependencies
Remember, you have to be root, so you might want to prefix `sudo`.

### Windows
[Install msys2](http://msys2.github.io/).

#### on 32-bit
Launch the *Mingw-w64 Win32 shell* from the start menu. In it, type:
```bash
pacman -S pkg-config mingw-w64-i686-cmake mingw-w64-i686-make mingw-w64-i686-gcc mingw-w64-i686-pkg-config mingw-w64-i686-assimp mingw-w64-i686-libepoxy mingw-w64-i686-SDL2 mingw-w64-i686-SDL2_mixer mingw-w64-i686-bullet mingw-w64-i686-tinyxml2
```

#### on 64-bit
Launch the *Mingw-w64 Win64 shell* from the start menu. In it, type:
```bash
pacman -S pkg-config mingw-w64-x86_64-cmake mingw-w64-x86_64-make mingw-w64-x86_64-gcc mingw-w64-x86_64-pkg-config mingw-w64-x86_64-assimp mingw-w64-x86_64-libepoxy mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-bullet mingw-w64-x86_64-tinyxml2
```

If you do not already have `git` installed, please also install it with:

```bash
pacman -S git
```

### Arch Linux
```bash
pacman -S cmake make gcc pkg-config assimp libepoxy sdl2 sdl2_mixer bullet mesa tinyxml2
```

### Debian 8 / Ubuntu 14.04 / Linux Mint 17.x
```bash
apt-get install cmake make gcc pkg-config libassimp-dev libepoxy-dev libsdl2-dev libsdl2-mixer-dev libbullet-dev libtinyxml2-dev libgl1-mesa-dev libunittest++-dev
```

### Others
Now how will you satisfy these dependencies? No problem. You might notice that when you
are looking for them in your package manager that you get overwhelmed by a list of possible
installation candidates, how do you know which one is the right one?

An easy rule that applies to many libraries is that a library has the lib prefix and then,
since you want to install the files for development there is a dev in the name.
Pretty much like libfoo-dev for most libraries.

## Downloading source code

In your favorite console, run these commands:

```bash
git clone https://github.com/GlPortal/glPortal.git
cd glPortal
git submodule init
git submodule update
```

## Building the binary

Make sure to check the README.md and COMPILE.md from the release you downloaded in case
there are differences in the compile workflow for it.

In the root directory of the downloaded source type:
### Linux
```bash
mkdir build && cd build
cmake ..
```

### Windows
```bash
mkdir build && cd build
cmake .. -G "MSYS Makefiles" -DCMAKE_MAKE_PROGRAM="mingw32-make"
```


If you want to profile the source code pass *-DCMAKE_CXX_FLAGS=-pg* to cmake.
Now if this throws an error you have to fix something first. If you don't manage to fix the error, use the
bugtracker https://github.com/GlPortal/glPortal/issues or http://bugs.glportal.de to report what you did, and what error you got.

If the command did not produce an error, you can build the binary by typing in:
### Linux
```bash
make
```

### Windows
```bash
mingw32-make
```

If this produces no error you have built the binary and should be able to start GlPortal by typing in:
### Linux
```bash
make run
```

### Windows
Before we can start the game it is neccesery to copy some dlls into our executable directory.

#### on 32-bit
```bash
cp /mingw32/bin/libLinearMath.dll /mingw32/bin/SDL2.dll /mingw32/bin/SDL2_mixer.dll /mingw32/bin/libtinyxml2.dll /mingw32/bin/libgcc_s_dw2-1.dll /mingw32/bin/libstdc++-6.dll /mingw32/bin/libmodplug-1.dll /mingw32/bin/libvorbisfile-3.dll /mingw32/bin/libvorbis-0.dll /mingw32/bin/libogg-0.dll /mingw32/bin/libassimp.dll /mingw32/bin/libBulletCollision.dll /mingw32/bin/libBulletDynamics.dll /mingw32/bin/libepoxy-0.dll /mingw32/bin/libwinpthread-1.dll /mingw32/bin/libfluidsynth-1.dll /mingw32/bin/libminizip-1.dll /mingw32/bin/zlib1.dll /mingw32/bin/libFLAC-8.dll /mingw32/bin/libmad-0.dll /mingw32/bin/libbz2-1.dll /mingw32/bin/libglib-2.0-0.dll /mingw32/bin/libportaudio-2.dll /mingw32/bin/libsndfile-1.dll /mingw32/bin/libintl-8.dll /mingw32/bin/libspeex-1.dll /mingw32/bin/libvorbisenc-2.dll /mingw32/bin/libiconv-2.dll source
```

#### on 64-bit
```bash
cp /mingw64/bin/libLinearMath.dll /mingw64/bin/SDL2.dll /mingw64/bin/SDL2_mixer.dll /mingw64/bin/libtinyxml2.dll /mingw64/bin/libgcc_s_seh-1.dll /mingw64/bin/libstdc++-6.dll /mingw64/bin/libmodplug-1.dll /mingw64/bin/libvorbisfile-3.dll /mingw64/bin/libvorbis-0.dll /mingw64/bin/libogg-0.dll /mingw64/bin/libassimp.dll /mingw64/bin/libBulletCollision.dll /mingw64/bin/libBulletDynamics.dll /mingw64/bin/libepoxy-0.dll /mingw64/bin/libwinpthread-1.dll /mingw64/bin/libfluidsynth-1.dll /mingw64/bin/libminizip-1.dll /mingw64/bin/zlib1.dll /mingw64/bin/libFLAC-8.dll /mingw64/bin/libmad-0.dll /mingw64/bin/libbz2-1.dll /mingw64/bin/libglib-2.0-0.dll /mingw64/bin/libportaudio-2.dll /mingw64/bin/libsndfile-1.dll /mingw64/bin/libintl-8.dll /mingw64/bin/libspeex-1.dll /mingw64/bin/libvorbisenc-2.dll /mingw64/bin/libiconv-2.dll /mingw64/bin/libpcre-1.dll source
```

```bash
mingw32-make run
```

If you get errors, try to build GlPortal again. If you don't manage to fix the error, use the
bugtracker https://github.com/GlPortal/glPortal/issues or http://bugs.glportal.de to report what you did, and what error you got.

## Build with docker
Building with docker is still in early testing and not feature complete. It will make compiling easier in the future.
```bash
docker run -it --rm -w /data -v $(pwd):/data glportal/gcc bash -c "cmake ./; make"
```

To run the game type:
```bash
source/glportal --datadir ./data
```

## Keep us posted
Not working? No worries! Help us help you figure out what we missed to make this work on
your system by opening an issue.
