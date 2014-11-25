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
Midiogre uses CMake as build system.

Here is a description of Midiogres CMake arguments:

CMake Argument  | Default Value | Meaning
-------------   | ------------- | -------------
-DMAIN          |       0       | Build Midiogre
-DDEBUG         |       1       | Build in Debug Mode
-DSAMPLES       |       1       | Build Samples
-DDOC           |       1       | Build Documentation

### Compile Example
So to install Midiogre to */home/user/midiogre* without samples you could do the following:

```bash
git clone git@github.com:prozum/midiogre.git 
mkdir midiogre-build && cd midiogre-build
cmake ../midiogre -DMAIN=1 -DSAMPLES=0 \
	-DCMAKE_INSTALL_PREFIX=/home/user/midiogre
make install
```

CodingStyle
--------------------
### Indentation: 
4 spaces

### Example
```c
/**
 * this_is_a_func - a func example 
 * @var_1: description of var_1
 * @var_2: description of var_2
 * @var_3: description of var_3
 *
 * This function is used as an example.
 * More info about function...
 */
int this_is_a_func(int var_1, int var_2, int var_3)
{
    /* variable naming example */
    int i,this_is_a_var,yet_another_var;

    /* if example */
    if (var_1 == SYMB_CONST) {
        this_is_a_var = 0;
    } else {
        this_is_a_var = -1;
    }
    
    /* for example */
    for (i = 0; i < var_2; i++) {
        call_func(i);
    }

    /* switch example */
    switch (var_3) {
        case SYMB_CONST_1:
            yet_another_var = 0x10;
            break;
        case SYMB_CONST_2:
            yet_another_var = 0x20;
            break;
        default:
            yet_another_var = 0x01;
    }

    return 0;
}
```

