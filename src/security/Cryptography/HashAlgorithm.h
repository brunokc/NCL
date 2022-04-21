#pragma once

#include <memory>
#include <vector>
#include <string>

#include <windows.h>
#include <wil/resource.h>

#include "CryptographicException.h"
#include "io/Stream.h"

namespace WCL::Security::Cryptography {

class HashAlgorithm
{
private:
    struct private_marker { };

public:
    HashAlgorithm(
        const private_marker&,
        wil::unique_bcrypt_algorithm&& algoHandle,
        wil::unique_bcrypt_hash&& hashHandle,
        std::vector<uint8_t>&& hashObject,
        DWORD hashLengthInBytes
        );

    virtual ~HashAlgorithm() { }

    virtual const std::vector<uint8_t>& Hash() const;
    virtual int HashSize() const;
    virtual int IntputBlockSize() const;
    virtual int OutputBlockSize() const;

    virtual const std::vector<uint8_t>& ComputeHash(
        const BYTE* bytes,
        int byteCount
        );

    virtual const std::vector<uint8_t>& ComputeHash(
        const std::shared_ptr<WCL::IO::Stream>& stream
        );

    static std::shared_ptr<HashAlgorithm> Create(
        const std::wstring_view hashName
        );

protected:
    //template<class THash>
    static std::shared_ptr<HashAlgorithm> CreateHash(const wchar_t* algorithm);

    void HashData(
        const BYTE* bytes,
        int byteCount
        );

    void HashData(const std::vector<uint8_t>& data);

    void FinishHash();

protected:
    wil::unique_bcrypt_algorithm _algorithmHandle;
    wil::unique_bcrypt_hash _hashHandle;
    std::vector<uint8_t> _hashObject;

    std::vector<uint8_t> _hash;
    int _hashSize = 0;
    int _inputBlockSize = 0;
    int _outputBlockSize = 0;
};

}
