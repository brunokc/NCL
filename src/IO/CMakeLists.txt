add_library(wcl_io OBJECT 
	Directory.cpp Directory.h 
	File.cpp File.h 
	FileStream.cpp FileStream.h
	Path.cpp Path.h 
	Stream.h
	IOException.h 
	TextReader.h
	StreamReader.h
	StreamReader.cpp
	IOHelper.h
	IOHelper.cpp
)

# Add to lib dependencies:
# Pathcch.lib
