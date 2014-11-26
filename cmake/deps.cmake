
# Find PkgConfig
find_package( PkgConfig )
if (NOT PKGCONFIG_FOUND)
        message(FATAL_ERROR  "Please install: pkgconfig")
endif()

# Localization support (Not used)
if(LOCALE)
    find_package(GetText)
    if (NOT PKGCONFIG_FOUND)
        message(FATAL_ERROR  "Please install: gettext")
    endif()
endif()

if(SQL OR MAIN)
    pkg_check_modules_fullpath(SQLITE3 sqlite3)
    #pkg_check_modules(SQLITE3 sqlite3)
    if(NOT SQLITE3_FOUND)
        message(FATAL_ERROR  "Please install: sqlite-devel")
    endif()
    
    include_directories(${SQLITE3_INCLUDE_DIRS})
    add_definitions(${SQLITE3_CFLAGS_OTHER})
endif()

if(GUI OR MAIN)
    # Find glib-2
    #pkg_check_modules_fullpath(GLIB2 glib-2.0)
    pkg_check_modules(GLIB2 glib-2.0)
    if (NOT GLIB2_FOUND)
        message(FATAL_ERROR  "Please install: glib2-devel")
    endif()

    include_directories(${GLIB2_INCLUDE_DIR})

    # Find GTK3
    #pkg_check_modules_fullpath(GTK3 gtk+-3.0>=3.10)
    pkg_check_modules(GTK3 gtk+-3.0>=3.10)
    if (NOT GTK3_FOUND)
        message(FATAL_ERROR  "Please install: gtk3-devel")
    endif()

    include_directories(${GTK3_INCLUDE_DIRS})
    add_definitions(${GTK3_CFLAGS_OTHER})

    # Find command: glib-compile-resources
    _pkgconfig_invoke("glib-2.0" GLIB2 PREFIX
                      "" "--variable=prefix")

    find_program(GLIB_COMPILE_RESOURCES
                NAMES glib-compile-resources
                HINTS ${GLIB2_PREFIX})

    if (NOT GLIB_COMPILE_RESOURCES)
        message(FATAL_ERROR "Please install: glib2-devel")
    endif()
endif()

