project('midiogre', 'c', version: '0.0.1')

gnome = import('gnome')

# Options
main_enabled = get_option('main')
debug_enabled = get_option('debug')
samples_enabled = get_option('samples')
doc_enabled = get_option('doc')

# Include dirs
incdir = include_directories('src/lib')

# Dependencies
cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)
glib_dep = dependency('glib-2.0')
gtk_dep = dependency('gtk+-3.0')
gio_dep = dependency('gio-2.0')
sqlite_dep = dependency('sqlite3')

subdir('src')

if samples_enabled
  subdir('samples')
endif

if doc_enabled
  subdir('doc')
endif
