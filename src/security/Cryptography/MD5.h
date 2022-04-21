#pragma once

#include <memory>
#include <vector>

#include <windows.h>
#include <bcrypt.h>
#include <wil/resource.h>

#include "HashAlgorithm.h"

namespace WCL::Security::Cryptography {

class MD5 : public HashAlgorithm
{
//private:
//    struct private_marker { };
//
public:
    virtual ~MD5() { }

    //MD5(
    //    const private_marker&,
    //    wil::unique_bcrypt_algorithm&& algoHandle,
    //    wil::unique_bcrypt_hash&& hashHandle,
    //    std::vector<uint8_t>&& hashObject,
    //    DWORD hashLengthInBytes
    //    );

    static std::shared_ptr<MD5> Create();
};

}
