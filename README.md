midiogre
========
- Connecting ogres
- *Get ogre it*
- *It'll be ogre before you know it.*
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

Ogres are not included.

Compile Instructions
--------------------
Midiogre uses CMake as build system.

Here is a description of Midiogres CMake arguments

CMake Argument  | Default Value | Meaning
-------------   | ------------- | -------------
-DMAIN          |       0       | Build Midiogre
-DSAMPLES       |       1       | Build Samples
-DDOC           |       0       | Build Documentation

So to install Midiogre to /home/user/midiogre 

without samples and documentation you could do the following:

```bash
[user midiogre-git]$ mkdir build && cd build
[user midiogre-git/build]$ cmake .. -DMAIN=1 -DSAMPLES=0 -DCMAKE_INSTALL_PREFIX=/home/user/midiogre
[user midiogre-git/build]$ make install
```
