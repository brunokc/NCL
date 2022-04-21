#include "SHA1.h"
#include "Util.h"

using namespace WCL::Security::Cryptography;

//SHA1::SHA1(
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

std::shared_ptr<SHA1>
SHA1::Create()
{
    return static_pointer_cast<SHA1>(CreateHash(BCRYPT_SHA1_ALGORITHM));
}
