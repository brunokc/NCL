#pragma once

#include <memory>

#include "HashAlgorithm.h"

namespace WCL::Security::Cryptography {

class SHA512 : public HashAlgorithm
{
public:
    virtual ~SHA512() { }

    static std::shared_ptr<SHA512> Create();
};

}
