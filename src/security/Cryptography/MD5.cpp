#include "MD5.h"
#include "Util.h"

using namespace WCL::Security::Cryptography;

//MD5::MD5(
//    const private_marker&,
//    wil::unique_bcrypt_algorithm&& algoHandle,
//    wil::unique_bcrypt_hash&& hashHandle,
//    std::vector<uint8_t>&& hashObject,
//    DWORD hashLengthInBytes
//    )
//{
//    _algorithmHandle = std::move(algoHandle);
//    _hashHandle = std::move(hashHandle);
//    _hashObject = std::move(hashObject);
//    _hashSize = hashLengthInBytes;
//
//    _hash.resize(hashLengthInBytes);
//}

std::shared_ptr<MD5>
MD5::Create()
{
    return static_pointer_cast<MD5>(CreateHash(BCRYPT_MD5_ALGORITHM));
}
