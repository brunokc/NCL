#include "SHA1.h"

using namespace WCL::Security::Cryptography;

// static
std::shared_ptr<SHA1>
SHA1::Create()
{
    return static_pointer_cast<SHA1>(CreateHash(BCRYPT_SHA1_ALGORITHM));
}
