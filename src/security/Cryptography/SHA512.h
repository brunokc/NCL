#pragma once

#include <memory>
#include <vector>

#include <windows.h>
#include <bcrypt.h>
#include <wil/resource.h>

#include "HashAlgorithm.h"

namespace WCL::Security::Cryptography {

class SHA512 : public HashAlgorithm
{
private:
    //struct private_marker { };

public:
    virtual ~SHA512() { }

    //SHA512(
    //    const private_marker&,
    //    wil::unique_bcrypt_algorithm&& algoHandle,
    //    wil::unique_bcrypt_hash&& hashHandle,
    //    std::vector<uint8_t>&& hashObject,
    //    DWORD hashLengthInBytes
    //    );

    static std::shared_ptr<SHA512> Create();
};

}
