# NCL - Native Class Library

NCL (pronounced "Nickel") is a C++ library aimed at making day-to-day tasks easier 
to do. The API borrows the design from the .NET Base Class Library (BCL) where it 
makes sense, while maintaining some well-known C++ standards, like the use of STL.
For example, std::wstring are used instead of a new String class (although more
helpers functions are provided); std::vector<> is used instead of raw arrays 
and std::shared_ptr<> is used to allow sharing of object instances.

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
