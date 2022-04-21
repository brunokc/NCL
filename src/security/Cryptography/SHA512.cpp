#include "SHA512.h"
#include "Util.h"

using namespace WCL::Security::Cryptography;

//SHA512::SHA512(
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

// static
std::shared_ptr<SHA512>
SHA512::Create()
{
    return static_pointer_cast<SHA512>(CreateHash(BCRYPT_SHA512_ALGORITHM));
}
