#pragma once

#include <memory>

#include "HashAlgorithm.h"

namespace WCL::Security::Cryptography {

class SHA256 : public HashAlgorithm
{
public:
    virtual ~SHA256() { }

    static std::shared_ptr<SHA256> Create();
};

}
