# Compile Guide for GlPortal

## Prerequisites
Make sure you have the necessary dependencies installed for your operating system.

### Windows
- **Visual Studio 2017**: Required for building with MSVC due to C++14 language features. See the [compile instructions for VS](https://github.com/GlPortal/glportal-vs) for details.
- **CMake**: Make sure it's installed and in your PATH.
- **MinGW**: For using `mingw32-make`.

### Linux
- Ensure `cmake` and `make` are installed using your package manager.
- Install dependencies listed in `CMakeLists.txt`.

### Mac OS X
- Use `brew` to install dependencies: 
  ```bash
  brew install Assimp SDL2 SDL2_mixer TinyXML2 Bullet freeimage

## Downloading source code

In your favorite console, run these commands:

```bash
git clone https://github.com/GlPortal/glPortal.git
cd glPortal
git submodule update --init --recursive
```

## Building the binary

Check the README.md and COMPILE.md files from the release you downloaded for any differences in the compile workflow.

### Setting up the build directory

In the root directory of the downloaded source type:

#### Linux
```bash
cmake ./;
```

#### Windows
```bash
mkdir build && cd build
cmake .. -G "MSYS Makefiles" -DCMAKE_MAKE_PROGRAM="mingw32-make"
```

#### Mac OS X

```bash
brew install Assimp SDL2 SDL2_mixer TinyXML2 Bullet freeimage
cmake ./;
```

If you want to profile the source code pass `-DCMAKE_CXX_FLAGS=-pg` to cmake.

If you encounter an error during this step use the
bugtracker https://github.com/GlPortal/glPortal/issues to report an issue.

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

#### Mac OS X
```bash
make
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


#### Mac OS X
```bash
make run
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
