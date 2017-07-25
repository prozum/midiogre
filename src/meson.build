subdir('lib')

if main_enabled
  midiogre_res = gnome.compile_resources('midiogre-res', 'midiogre.gresource.xml', 
                                         source_dir: '..')
  midiogre_exe = executable('midiogre', 
                            'midiogre-main.c', 'midiogre-app.c', 'midiogre-import.c', 
                            'midiogre-song.c', 'midiogre-search.c', midiogre_res, 
                            link_with: [mid_lib, analyze_lib, db_lib, pop_lib], 
                            dependencies: [m_dep, gtk_dep, sqlite_dep], 
                            include_directories: incdir)
endif
