
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

TEST_CASE("Read a small file using memory mapping", "[io]") {
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
    int bytesRead = accessor->ReadArray<BYTE>(0, stringBytes, 36);
    REQUIRE(bytesRead == stringData.size());

    auto readString = Encoding::UTF8->GetString(stringBytes, bytesRead);
    REQUIRE(readString == stringData);
}
