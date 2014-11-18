[![Travis: Build Status](https://travis-ci.org/prozum/midiogre.svg?branch=master)](https://travis-ci.org/prozum/midiogre)
[![Coverity: Analysis Status](https://scan.coverity.com/projects/3450/badge.svg)](https://scan.coverity.com/projects/3450)

midiogre
========
- Connecting ogres
- *Get ogre it*
- *It'll be ogre before you know it.*
- *Let's get this ogre with*
- Build for ogres ahead
- Touch the ogre
- Born to ogre
- Applying ogre
- Play the ogre
- Never ogre
- The way to ogre
- A breath of fresh ogre
- The power of ogre
- The joy of ogre
- Exceed your ogre
- The life you ogre
- Like no ogre
- Life's ogre
- Choose ogre
- On ogre
- Bringing ogre to life
- Pure ogre
- Ogre you can use
- Ogre your way
- The ogre for everyone
- Now presenting the new eyeOgre
- The way it's meant to be ogred

Ogres are not included.

Dependencies
--------------------
- CMake		≥ 2.8
- pkg-config	≥ 0.26
- Doxygen	≥ 1.8
- GTK+		≥ 3.10

Compile Instructions
--------------------
Midiogre uses the CMake build system.

Here is a description of Midiogres CMake arguments:

CMake Argument  | Default Value | Meaning
-------------   | ------------- | -------------
-DMAIN          |       0       | Build Midiogre
-DDEBUG         |       1       | Build in Debug Mode
-DSAMPLES       |       1       | Build Samples
-DDOC           |       1       | Build Documentation

### Compile Example
To install Midiogre to */home/user/midiogre* without samples you could do the following:

```bash
git clone git@github.com:prozum/midiogre.git 
mkdir midiogre-build && cd midiogre-build
cmake ../midiogre -DMAIN=1 -DSAMPLES=0 \
	-DCMAKE_INSTALL_PREFIX=/home/user/midiogre
make install
```
