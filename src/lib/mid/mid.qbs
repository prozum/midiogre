import qbs 1.0

Product {
	name: "mid"
	type: "dynamiclibrary"
	files: ["*.c"]

	Depends { name: "list" }

	cpp.includePaths: project.sourceDirectory + "/src/lib"
	Depends { name: 'cpp' }
	Export {
        Depends { name: "cpp" }
	}
}
