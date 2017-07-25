doxygen = find_program('doxygen')

cdata = configuration_data()
cdata.set('PROJECT_NAME', meson.project_name())
cdata.set('PROJECT_VERSION', meson.project_version())
cdata.set('PROJECT_SOURCE_DIR', meson.source_root())
cdata.set('PROJECT_README', join_paths(meson.source_root(), 'README.md'))

doxyfile = configure_file(input: 'Doxyfile.in',
                          output: 'Doxyfile',
                          configuration: cdata,
                          install: false)

datadir = join_paths(get_option('datadir'), 'doc', 'midiogre')

html_target = custom_target('doc',
                            input: doxyfile,
                            output: 'html',
                            command: [doxygen, doxyfile],
                            install: true,
                            install_dir: datadir)
