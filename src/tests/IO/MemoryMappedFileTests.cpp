
#include <windows.h>
#include <stdio.h>
#include <vector>
#include <string>

// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <IO/File.h>
#include <IO/MemoryMappedFiles/MemoryMappedFile.h>
#include <Text/Encoding.h>

using namespace WCL::IO;
using namespace WCL::IO::MemoryMappedFiles;
using namespace WCL::Text;

TEST_CASE("Read a small file using memory mapping", "[io]") 
{
    const auto stringData = std::wstring{ L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" };

    auto memoryMappedFile = MemoryMappedFile::CreateFromFile(
        L"test_memory_map_file.bin", 
        FileMode::Open, 
        { },
        0, 
        MemoryMappedFileAccess::Read
        );
    auto accessor = memoryMappedFile->CreateViewAccessor();
    REQUIRE(accessor != nullptr);

    BYTE stringBytes[256] = { };
    int bytesRead = accessor->ReadArray<BYTE>(0, stringBytes, 10);
    REQUIRE(bytesRead == 10);

    auto numbers = Encoding::UTF8->GetString(stringBytes, bytesRead);
    REQUIRE(numbers == stringData.substr(0, 10));

    ZeroMemory(stringBytes, std::size(stringBytes));
    bytesRead = accessor->ReadArray<BYTE>(10, stringBytes, 26);
    REQUIRE(bytesRead == 26);

    auto letters = Encoding::UTF8->GetString(stringBytes, bytesRead);
    REQUIRE(letters == stringData.substr(10, 26));

    auto readString = numbers + letters;
    REQUIRE(readString == stringData);
}
