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
	MemoryMappedFiles/MemoryMappedFile.cpp
	MemoryMappedFiles/MemoryMappedFile.h
	MemoryMappedFiles/MemoryMappedViewAccessor.cpp
	MemoryMappedFiles/MemoryMappedViewAccessor.h
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
