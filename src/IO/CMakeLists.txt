add_library(wcl_io OBJECT 
	Directory.cpp 
	Directory.h 
	DirectoryInfo.cpp
	DirectoryInfo.h
	File.cpp 
	File.h 
	FileInfo.cpp
	FileInfo.h
	FileStream.cpp 
	FileStream.h
	FileSystemInfo.cpp
	FileSystemInfo.h
	IOException.h 
	IOHelper.cpp
	IOHelper.h
	Path.cpp 
	Path.h 
	Stream.h
	StreamReader.cpp
	StreamReader.h
	StreamWriter.cpp
	StreamWriter.h
	TextReader.h
	TextWriter.h
 )

target_link_libraries(wcl_io PRIVATE WIL::WIL)
