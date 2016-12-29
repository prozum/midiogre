import qbs 1.0
import qbs.Probes

CppApplication {
    name : "midiogre"
	type: "application"
    files : "*.c"

	property stringList cFlags: []
	property stringList linkerFlags: []

    Probes.PkgConfigProbe {
        id: glib2
        name: "glib-2.0"
    }

    Probes.PkgConfigProbe {
        id: gtk3
        name: "gtk+-3.0"
    }

    Probes.PkgConfigProbe {
        id: sqlite3
        name: "sqlite3"
    }

    Depends { name: "mid" }
    Depends { name: "analyze" }
    Depends { name: "db" }
    Depends { name: "pop" }
	
	cpp.includePaths: "lib"
    cpp.cFlags: cFlags.concat(gtk3.cflags)
    cpp.linkerFlags: linkerFlags.concat(gtk3.libs, sqlite3.libs)
}
