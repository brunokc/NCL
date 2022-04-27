
#include <windows.h>
#include <stdio.h>
#include <vector>
#include <string>

#include <IO/File.h>
#include <IO/FileStream.h>

#include <Security/Cryptography/MD5.h>
#include <Security/Cryptography/SHA1.h>
#include <Security/Cryptography/SHA256.h>
#include <Security/Cryptography/SHA512.h>

using namespace WCL::IO;
using namespace WCL::Security::Cryptography;

std::string data = "This string is going be hashed.";

bool CheckHash(const byte* calculatedHash, size_t calculatedHashSize, const byte* expectedHash, size_t expectedHashSize)
{
    bool success = true;
    if (calculatedHashSize != expectedHashSize)
    {
        wprintf(L"Hash size mismatch\n");
        success = false;
    }

    for (size_t i = 0; i < calculatedHashSize; i++)
    {
        if (calculatedHash[i] != expectedHash[i])
        {
            wprintf(L"Hash mismatch at index %I64d.\n", i);
            success = false;
            break;
        }
    }

    return success;
}

template<class THash>
bool CheckFileHash(const std::wstring& fileName, const THash& hashAlgo, const byte* expectedHash, size_t expectedHashSize)
{
    auto file = File::OpenRead(fileName);
    auto calculatedHash = hashAlgo->ComputeHash(file);

    bool success = true;
    if (calculatedHash.size() != expectedHashSize)
    {
        wprintf(L"Hash size mismatch\n");
        success = false;
    }

    for (size_t i = 0; i < calculatedHash.size(); i++)
    {
        if (calculatedHash[i] != expectedHash[i])
        {
            wprintf(L"Hash mismatch at index %I64d.\n", i);
            success = false;
            break;
        }
    }

    return success;
}

void TestMD5()
{
    byte md5_hash[] = { 0x61, 0x03, 0x28, 0x1F, 0x55, 0x22, 0xA1, 0x8D, 0x94, 0x7F, 0x3B, 0x05, 0xB9, 0xDB, 0x82, 0x99 };

    wprintf(L"MD5: hashing string\n");
    auto md5hash = MD5::Create();
    auto hash = md5hash->ComputeHash(reinterpret_cast<BYTE*>(data.data()), static_cast<int>(data.length()));
    if (CheckHash(hash.data(), hash.size(), md5_hash, std::size(md5_hash)))
    {
        wprintf(L"Hashes match!\n");
    }
}

void TestSHA1()
{
    byte sha1_hash[] = { 0xE2, 0x85, 0x67, 0xCF, 0xFC, 0xA8, 0xA6, 0x8F, 0xE6, 0xE4, 0x56, 0x6F, 0x4C, 0xAC,
        0x57, 0xFC, 0x33, 0x06, 0xDA, 0x93 };

    wprintf(L"SHA1: hashing string\n");
    auto sha1hash = SHA1::Create();
    auto hash = sha1hash->ComputeHash(reinterpret_cast<BYTE*>(data.data()), static_cast<int>(data.length()));
    if (CheckHash(hash.data(), hash.size(), sha1_hash, std::size(sha1_hash)))
    {
        wprintf(L"Hashes match!\n");
    }
}

void TestSHA256()
{
    byte sha256_hash[] = { 0x77, 0xB4, 0xD8, 0xDA, 0x2A, 0xD1, 0xED, 0xB2, 0x7D, 0x62, 0x29, 0xBF, 0x4C, 0x83,
        0xC7, 0x4E, 0x48, 0x0B, 0x5B, 0xD1, 0xCA, 0xE1, 0x4D, 0x25, 0x94, 0x44, 0x29, 0x27, 0x93, 0x72, 0xF0, 0x0E };

    wprintf(L"SHA256: hashing string\n");
    auto sha256hash = SHA256::Create();
    auto hash = sha256hash->ComputeHash(reinterpret_cast<BYTE*>(data.data()), static_cast<int>(data.length()));
    if (CheckHash(hash.data(), hash.size(), sha256_hash, std::size(sha256_hash)))
    {
        wprintf(L"Hashes match!\n");
    }
}

void TestSHA512()
{
    byte sha512_hash[] = { 0xE8, 0x24, 0xDE, 0xB8, 0x99, 0x31, 0x48, 0x93, 0x93, 0x59, 0xAE, 0x64, 0x00, 0x24, 0x99, 
        0x0A, 0x56, 0x76, 0x7C, 0x9B, 0xC9, 0x81, 0x82, 0x04, 0x35, 0xBC, 0xDD, 0xFD, 0x21, 0xCF, 0x21, 0xE5, 0xFB, 
        0xAD, 0xC5, 0xA3, 0xD0, 0xCE, 0x46, 0x93, 0x3B, 0x67, 0x5C, 0xD6, 0x3B, 0xC8, 0xF0, 0xD8, 0xA4, 0x15, 0xAC, 
        0xF2, 0xC8, 0xD6, 0x10, 0x88, 0xBC, 0x4A, 0x5D, 0x4F, 0xF8, 0x2F, 0xBD, 0x26 };

    wprintf(L"SHA512: hashing string\n");
    auto sha512hash = SHA512::Create();
    auto hash = sha512hash->ComputeHash(reinterpret_cast<BYTE*>(data.data()), static_cast<int>(data.length()));
    if (CheckHash(hash.data(), hash.size(), sha512_hash, std::size(sha512_hash)))
    {
        wprintf(L"Hashes match!\n");
    }
}

void Test2mbFileWithMD5()
{
    // 0689fbeab69d83d9de56340f05173bb8
    byte md5_hash[] = { 0x06, 0x89, 0xfb, 0xea, 0xb6, 0x9d, 0x83, 0xd9, 0xde, 0x56, 0x34, 0x0f, 0x05, 0x17, 0x3b, 0xb8 };

    wprintf(L"MD5: hashing 2MB file\n");
    bool success = CheckFileHash(L"sample-2mb-text-file.txt", MD5::Create(), md5_hash, std::size(md5_hash));
    if (success)
    {
        wprintf(L"Hashes match!\n");
    }
}

void Test2mbFileWithSHA1()
{
    // 9828990315546737962abed57318ede7fdfe8c09
    byte sha1_hash[] = { 0x98, 0x28, 0x99, 0x03, 0x15, 0x54, 0x67, 0x37, 0x96, 0x2a, 0xbe, 0xd5, 0x73, 0x18, 0xed, 0xe7, 
        0xfd, 0xfe, 0x8c, 0x09 };

    wprintf(L"SHA1: hashing 2MB file\n");
    bool success = CheckFileHash(L"sample-2mb-text-file.txt", SHA1::Create(), sha1_hash, std::size(sha1_hash));
    if (success)
    {
        wprintf(L"Hashes match!\n");
    }
}

void Test2mbFileWithSHA256()
{
    // abf7947acf62a008be06fbdf80c2dab843af11763d7283c6efb258a3da9371ec
    byte sha256_hash[] = { 0xab, 0xf7, 0x94, 0x7a, 0xcf, 0x62, 0xa0, 0x08, 0xbe, 0x06, 0xfb, 0xdf, 0x80, 0xc2, 0xda, 0xb8, 
        0x43, 0xaf, 0x11, 0x76, 0x3d, 0x72, 0x83, 0xc6, 0xef, 0xb2, 0x58, 0xa3, 0xda, 0x93, 0x71, 0xec };

    wprintf(L"SHA256: hashing 2MB file\n");
    bool success = CheckFileHash(L"sample-2mb-text-file.txt", SHA256::Create(), sha256_hash, std::size(sha256_hash));
    if (success)
    {
        wprintf(L"Hashes match!\n");
    }
}

void Test2mbFileWithSHA512()
{
    // cec0c6a0ca830bbffc5c5f66988565535993707359acfb0320820b96f83969021b6474860f3dacf8f2b3405a4faa64fa4e2b4a9efe7fd758f684c1af9e284344
    byte sha512_hash[] = { 0xce, 0xc0, 0xc6, 0xa0, 0xca, 0x83, 0x0b, 0xbf, 0xfc, 0x5c, 0x5f, 0x66, 0x98, 0x85, 0x65, 0x53, 
        0x59, 0x93, 0x70, 0x73, 0x59, 0xac, 0xfb, 0x03, 0x20, 0x82, 0x0b, 0x96, 0xf8, 0x39, 0x69, 0x02, 0x1b, 0x64, 0x74, 
        0x86, 0x0f, 0x3d, 0xac, 0xf8, 0xf2, 0xb3, 0x40, 0x5a, 0x4f, 0xaa, 0x64, 0xfa, 0x4e, 0x2b, 0x4a, 0x9e, 0xfe, 0x7f, 
        0xd7, 0x58, 0xf6, 0x84, 0xc1, 0xaf, 0x9e, 0x28, 0x43, 0x44 };

    wprintf(L"SHA512: hashing 2MB file\n");
    bool success = CheckFileHash(L"sample-2mb-text-file.txt", SHA512::Create(), sha512_hash, std::size(sha512_hash));
    if (success)
    {
        wprintf(L"Hashes match!\n");
    }
}

int __cdecl main(int, wchar_t* [])
{
    TestMD5();
    TestSHA1();
    TestSHA256();
    TestSHA512();

    Test2mbFileWithMD5();
    Test2mbFileWithSHA1();
    Test2mbFileWithSHA256();
    Test2mbFileWithSHA512();
}
