#include "SHA256.h"

using namespace WCL::Security::Cryptography;

// static
std::shared_ptr<SHA256> 
SHA256::Create()
{
    return static_pointer_cast<SHA256>(CreateHash(BCRYPT_SHA256_ALGORITHM));
}
