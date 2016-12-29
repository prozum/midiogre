import qbs 1.0

CppApplication {
    name : "info"
	type: "application"
    files : "*.c"

    Depends { name: "mid" }
	
	cpp.includePaths: project.sourceDirectory + "/src/lib"
}
