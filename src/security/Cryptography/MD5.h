#pragma once

#include <memory>

#include "HashAlgorithm.h"

namespace WCL::Security::Cryptography {

class MD5 : public HashAlgorithm
{
public:
    virtual ~MD5() { }

    static std::shared_ptr<MD5> Create();
};

}
