#pragma once

#include "CryptographicException.h"

namespace WCL::Security::Cryptography {

inline
void ThrowIfBCryptFailed(NTSTATUS status, const char* message)
{
    if (!BCRYPT_SUCCESS(status))
    {
        throw CryptographicException(HRESULT_FROM_NT(status), message);
    }
}

//
//template<class THash>
//std::shared_ptr<THash>
//CreateHash(const wchar_t* algorithm)
//{
//    wil::unique_bcrypt_algorithm algo;
//    NTSTATUS status = BCryptOpenAlgorithmProvider(&algo, algorithm, nullptr, 0);
//    ThrowIfBCryptStatus(status, "failed to open BCrypt algorithm provider");
//
//    DWORD hashObjectSize = 0;
//    DWORD resultLength = 0;
//    status = BCryptGetProperty(algo.get(), BCRYPT_OBJECT_LENGTH, (PUCHAR)&hashObjectSize, sizeof(DWORD), &resultLength, 0);
//    ThrowIfBCryptStatus(status, "failed to get object length");
//
//    std::vector<uint8_t> hashObject(hashObjectSize);
//
//    DWORD hashLength = 0;
//    status = BCryptGetProperty(algo.get(), BCRYPT_HASH_LENGTH, (PUCHAR)&hashLength, sizeof(DWORD), &resultLength, 0);
//    ThrowIfBCryptStatus(status, "failed to get hash length");
//
//    wil::unique_bcrypt_hash hash;
//    status = BCryptCreateHash(algo.get(), &hash, hashObject.data(), hashObjectSize, nullptr, 0, 0);
//    ThrowIfBCryptStatus(status, "failed to create hash");
//
//    return std::make_shared<THash>(
//        private_marker{},
//        std::move(algo),
//        std::move(hash),
//        std::move(hashObject),
//        hashLength
//        );
//}

}