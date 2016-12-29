import qbs 1.0

CppApplication {
	name: "list"
	type: "dynamiclibrary"
	files: ["*.c"]

	Depends { name: 'cpp' }
	Export {
        Depends { name: "cpp" }
	}
}
