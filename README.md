# WCL

A C++ library aimed at making day-to-day tasks easier to do. The API is based
the .NET Base Class Library (BCL) where it makes sense, while maintaining some
well-known C++ standards, like the use of STL. For example, we use std::wstring
instead of creating a new String class; we use std::vector<> instead of raw 
arrays and use std::shared_ptr<> to allow sharing of object instances.

## Example 1

```cpp
#include <io/File.h>
#include <io/FileStream.h>

auto file = File::CreateText(L"test.txt");
file->WriteLine(L"This is a test file.");
```

## Example 2

```cpp
#include <io/Directory.h>
#include <io/File.h>
#include <io/Path.h>

for (const auto& dir : Directory::EnumerateDirectories(L"."))
{
    if (File::Exists(Path::Combine(dir->FullPath(), L"README.md")))
    {
        wprintf(L"Found README.md in directory %s\n", dir->Name());
    }
}
```
