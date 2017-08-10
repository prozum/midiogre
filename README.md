[![Travis: Build Status](https://travis-ci.org/prozum/midiogre.svg?branch=master)](https://travis-ci.org/prozum/midiogre)
[![Coverity: Analysis Status](https://scan.coverity.com/projects/3450/badge.svg)](https://scan.coverity.com/projects/3450)

midiogre
========
MIDI music recommendation program written in C/GTK+

Dependencies (Tested with)
--------------------
- Meson		≥ 0.41.0
- pkg-config	≥ 1.3.0
- Doxygen	≥ 1.8
- GTK+		≥ 3.10
- SQLite 3  ≥ 3.19.0

Compile Instructions
--------------------
Midiogre uses Meson as the build system.

Here is a description of Midiogres Meson options:

Meson Option    | Default Value | Meaning
-------------   | ------------- | -------------
main            | true          | Build Midiogre app
samples         | true          | Build Samples
doc             | true          | Build Documentation
test            | true          | Build Test

### Compile Example
Install Midiogre to */home/user/midiogre* without the samples:

```bash
git clone git@github.com:prozum/midiogre.git 
mkdir build
meson -Dsamples=false \
	-Dprefix=/home/$user/midiogre
ninja install
```

CodingStyle
--------------------
Indentation: 4 spaces

Comments: Doxygen compatible

### Example
```c
/** @example.c */

/* include example 
 * 
 * 1. header file
 * 2. project libs
 * 3. 3rd party libs
 * 4. standard libs
 */
#include "example.h"

#include <projectlib/projectlib.h>
#include <projectlib/projectlib-util.h>

#include <3rdlib/3rdlib.h>

#include <stdio.h>
#include <stdint.h>


/** this_is_a_enum - enum example
 *  
 *  Description of enum.
 */
enum this_is_a_enum
{
    ELEMENT_0       0x10, /**< Description of ELEMENT_0 */
    ELEMENT_1       0x20, /**< Description of ELEMENT_1 */
    ELEMENT_2       0x30, /**< Description of ELEMENT_2 */
    ELEMENT_3       0x40, /**< Description of ELEMENT_3 */
}

/**
 * this_is_a_func - a func example 
 * @param var_1: description of var_1
 * @param var_2: description of var_2
 * @param var_3: description of var_3
 *
 * This function is used as an example.
 * More info about function...
 */
int this_is_a_func(int var_1, int var_2, int var_3)
{
    /* Variable naming example */
    int i;
    int this_is_a_var;   /**< Description of this_is_a_var   */
    int yet_another_var; /**< Description of yet_another_var */

    /* If example */
    if (var_1 < ELEMENT_0) {
        this_is_a_var = -1;
    } else if (var_1 > ELEMENT_4) {
        this_is_a_var = 1;
    } else {
        this_is_a_var = 0;
    }
    
    /* For example */
    for (i = 0; i < var_2; i++) {
        call_func(i);
    }

    /* Switch example */
    switch (var_3) {
        case ELEMENT_0:
        case ELEMENT_1:
            yet_another_var = 666;
            break;
        case ELEMENT_2:
            yet_another_var = 616;
            break;
        case ELEMENT_3:
            yet_another_var = 999;
            break;
        default:
            yet_another_var = 919;
    }

    return 0;
}
```

