ginfo_res = gnome.compile_resources('ginfo-res', 'ginfo.gresource.xml')

ginfo_exe = executable('ginfo', 'ginfo-main.c', 'ginfo-app.c', 'ginfo-window.c', 'ginfo-mid.c', ginfo_res, 
                       link_with: [list_lib, mid_lib], 
                       dependencies: [gtk_dep], 
                       include_directories: incdir)
