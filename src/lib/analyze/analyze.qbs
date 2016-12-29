import qbs 1.0

Product {
	name: "analyze"
	type: "library"
	files: ["*.c"]

	Depends { name: 'cpp' }
	Export {
        Depends { name: "cpp" }
	}
}
