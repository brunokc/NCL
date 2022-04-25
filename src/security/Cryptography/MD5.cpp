#include "MD5.h"

using namespace WCL::Security::Cryptography;

// static
std::shared_ptr<MD5>
MD5::Create()
{
    return static_pointer_cast<MD5>(CreateHash(BCRYPT_MD5_ALGORITHM));
}
