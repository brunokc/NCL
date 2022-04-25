#include "SHA512.h"

using namespace WCL::Security::Cryptography;

// static
std::shared_ptr<SHA512>
SHA512::Create()
{
    return static_pointer_cast<SHA512>(CreateHash(BCRYPT_SHA512_ALGORITHM));
}
