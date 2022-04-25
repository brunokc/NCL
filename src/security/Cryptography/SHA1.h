#pragma once

#include <memory>

#include "HashAlgorithm.h"

namespace WCL::Security::Cryptography {

class SHA1 : public HashAlgorithm
{
public:
    virtual ~SHA1() { }

    static std::shared_ptr<SHA1> Create();
};

}
