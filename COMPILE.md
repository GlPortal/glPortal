# Compile Guide for GlPortal

## Dependencies
**NOTE:** Dependencies will change all the time so watch the cmake output carefuly for missing libraries.
Make sure to check CMakeLists.txt if it is something missing.

## Upgrade your OS
You will need very recent versions of all the dependencies. So please upgrade your OS before proceeding.

* Debian: [DebianUpgrade](https://wiki.debian.org/DebianUpgrade)
* Ubuntu: [Upgrade Ubuntu](http://www.ubuntu.com/download/desktop/upgrade)
* Linux Mint: [How to upgrade to a newer release](https://community.linuxmint.com/tutorial/view/2)
* Arch Linux: `pacman -Syu`, which you probably already do frequently

## List of dependencies
- assimp: 3D model asset loading
- epoxy: OpenGL initialization
- sdl2: Input, OpenGL context & window
- sdl2-mixer: Sound
- freeimage: Texture loading
- bullet (v 2.82+): Physics simulation
- tinyxml2: Loading maps and data
- libgl (binaries provided by Mesa or your proprietary driver): 3D rendering
- cmake *(make)*: Meta-build system
- make *(make)*: Build system
- gcc, clang++ or MSVC *(make)*: C++14-able compiler
- pkg-config *(make)*: Libraries' paths auto-confog
- git: Download source code
- libunittest++ *(tests)*: Testing framework

## Installing dependencies
Remember, you have to be root, so you might want to prefix `sudo`.

### Windows
[Install msys2](http://msys2.github.io/).

#### on 32-bit
Launch the *Mingw-w64 Win32 shell* from the start menu. In it, type:
```bash
pacman -S git pkg-config mingw-w64-i686-{cmake,make,gcc,pkg-config,assimp,libepoxy,SDL2{,_mixer},bullet,tinyxml2,freeimage}
```

#### on 64-bit
Launch the *Mingw-w64 Win64 shell* from the start menu. In it, type:
```bash
pacman -S git pkg-config mingw-w64-x86_64-{cmake,make,gcc,pkg-config,assimp,libepoxy,SDL2{,_mixer},bullet,tinyxml2,freeimage}
```

### Arch Linux
```bash
pacman -S cmake make gcc pkg-config assimp libepoxy sdl2{,_mixer} bullet mesa tinyxml2 freeimage
```

### Debian 8 / Ubuntu 14.04 / Linux Mint 17.x
```bash
apt-get install cmake make gcc pkg-config lib{assimp,epoxy,sdl2{,-mixer},bullet,tinyxml2,gl1-mesa,unittest++,freeimage}-dev
```

### Ubuntu 17.04
```bash
apt-get install cmake make gcc pkg-config lib{assimp,epoxy,sdl2{,-mixer},bullet,tinyxml2,gl1-mesa,unittest++,freeimageplus}-dev
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
git submodule update --init --recursive
```

## Building the binary

Make sure to check the README.md and COMPILE.md from the release you downloaded in case
there are differences in the compile workflow for it.

### Setting up the build directory
In the root directory of the downloaded source type:
#### Linux
```bash
mkdir build && cd build
cmake ..
```

#### Windows
```bash
mkdir build && cd build
cmake .. -G "MSYS Makefiles" -DCMAKE_MAKE_PROGRAM="mingw32-make"
```


If you want to profile the source code pass `-DCMAKE_CXX_FLAGS=-pg` to cmake.
Now if this throws an error you have to fix something first. If you don't manage to fix the error, use the
bugtracker https://github.com/GlPortal/glPortal/issues or http://bugs.glportal.de to report what you did, and what error you got.

If the command did not produce an error, you can build the binary by typing in:

### Building
#### Linux
```bash
make
```

#### Windows
```bash
mingw32-make
```

### Running
If this produces no error you have built the binary and should be able to start GlPortal by typing in:
#### Linux
```bash
make run
```

#### Windows
Before we can start the game it is necessary to copy some dlls into our executable directory.

##### on 32-bit
```bash
cp /mingw32/bin/{libLinearMath,SDL2{,_mixer},libtinyxml2,libgcc_s_dw2-1,libstdc++-6,libmodplug-1,libvorbisfile-3,libvorbis-0,libogg-0,libassimp,libBulletCollision,libBulletDynamics,libepoxy-0,libwinpthread-1,libfluidsynth-1,libminizip-1,zlib1,libFLAC-8,libmad-0,libbz2-1,libglib-2.0-0,libportaudio-2,libsndfile-1,libintl-8,libspeex-1,libvorbisenc-2,libiconv-2}.dll source
```

##### on 64-bit
```bash
cp /mingw64/bin/{libLinearMath,SDL2{,_mixer},libtinyxml2,libgcc_s_seh-1,libstdc++-6,libmodplug-1,libvorbisfile-3,libvorbis-0,libogg-0,libassimp,libBulletCollision,libBulletDynamics,libepoxy-0,libwinpthread-1,libfluidsynth-1,libminizip-1,zlib1,libFLAC-8,libmad-0,libbz2-1,libglib-2.0-0,libportaudio-2,libsndfile-1,libintl-8,libspeex-1,libvorbisenc-2,libiconv-2,libpcre-1}.dll source
```

##### Run
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

## Build with Visual Studio
Building is only possible in Visual Studio 2017, as earlier versions do not support the requied C++14 language features.

First follow the section: "Downloading source code". Then download the latest project files from:
https://bintray.com/glportal/generic/GlPortal-VS2017-Solution

After downloading extract the contents into your GlPortal directory. This is the same directory that contains the README.
The GlPortal VS17 solution can be found in the new folder also called GlPortal.

When first building GlPortal be sure to build the whole solution and select Release + x86 mode at the top.

The game can then be run by pressing Ctrl + F5 or by going to Debug -> Start Without Debugging.

## Keep us posted
Not working? No worries! Help us help you figure out what we missed to make this work on
your system by opening an issue.