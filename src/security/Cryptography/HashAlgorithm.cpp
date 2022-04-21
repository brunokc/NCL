#include "HashAlgorithm.h"
#include "Util.h"

using namespace WCL::Security::Cryptography;

HashAlgorithm::HashAlgorithm(
    const private_marker&,
    wil::unique_bcrypt_algorithm&& algoHandle,
    wil::unique_bcrypt_hash&& hashHandle,
    std::vector<uint8_t>&& hashObject,
    DWORD hashLengthInBytes
    )
{
    _algorithmHandle = std::move(algoHandle);
    _hashHandle = std::move(hashHandle);
    _hashObject = std::move(hashObject);
    _hashSize = hashLengthInBytes;

    _hash.resize(hashLengthInBytes);
}

// static 
std::shared_ptr<HashAlgorithm> HashAlgorithm::Create(
    const std::wstring_view //hashName
    )
{
    return nullptr;
}

// static
//template<class THash>
std::shared_ptr<HashAlgorithm>
HashAlgorithm::CreateHash(const wchar_t* algorithm)
{
    wil::unique_bcrypt_algorithm algo;
    NTSTATUS status = BCryptOpenAlgorithmProvider(&algo, algorithm, nullptr, 0);
    ThrowIfBCryptFailed(status, "failed to open BCrypt algorithm provider");

    DWORD resultLength = 0;
    DWORD hashObjectSize = 0;
    status = BCryptGetProperty(algo.get(), BCRYPT_OBJECT_LENGTH, (PUCHAR)&hashObjectSize, sizeof(DWORD), &resultLength, 0);
    ThrowIfBCryptFailed(status, "failed to get object length");

    std::vector<uint8_t> hashObject(hashObjectSize);

    DWORD hashLength = 0;
    status = BCryptGetProperty(algo.get(), BCRYPT_HASH_LENGTH, (PUCHAR)&hashLength, sizeof(DWORD), &resultLength, 0);
    ThrowIfBCryptFailed(status, "failed to get hash length");

    wil::unique_bcrypt_hash hash;
    status = BCryptCreateHash(algo.get(), &hash, hashObject.data(), hashObjectSize, nullptr, 0, 0);
    ThrowIfBCryptFailed(status, "failed to create hash");

    return std::make_shared<HashAlgorithm>(
        private_marker{},
        std::move(algo),
        std::move(hash),
        std::move(hashObject),
        hashLength
        );
}

const std::vector<uint8_t>& HashAlgorithm::Hash() const
{
    return _hash;
}

int HashAlgorithm::HashSize() const
{
    return _hashSize;
}

int HashAlgorithm::IntputBlockSize() const
{
    return _inputBlockSize;
}

int HashAlgorithm::OutputBlockSize() const
{
    return _outputBlockSize;
}

const std::vector<uint8_t>& HashAlgorithm::ComputeHash(
    const BYTE* bytes,
    int byteCount
    )
{
    HashData(bytes, byteCount);
    FinishHash();
    return _hash;
}

const std::vector<uint8_t>&
HashAlgorithm::ComputeHash(
    const std::shared_ptr<WCL::IO::Stream>& stream
    )
{
    if (!stream->CanRead())
    {
        throw Exception("stream");
    }

    std::vector<uint8_t> buffer(4096);
    int bytesRead = 0;
    while ((bytesRead = stream->Read(buffer)) > 0)
    {
        HashData(buffer.data(), bytesRead);
    }

    FinishHash();

    return _hash;
}

void HashAlgorithm::HashData(
    const BYTE* bytes,
    int byteCount
)
{
    NTSTATUS status = BCryptHashData(
        _hashHandle.get(), 
        static_cast<PUCHAR>(const_cast<PUCHAR>(bytes)), 
        byteCount, 
        0);
    ThrowIfBCryptFailed(status, "failed to compute hash");
}

void HashAlgorithm::HashData(const std::vector<uint8_t>& buffer)
{
    HashData(buffer.data(), static_cast<int>(buffer.size()));
}

void HashAlgorithm::FinishHash()
{
    NTSTATUS status = BCryptFinishHash(_hashHandle.get(), _hash.data(), static_cast<ULONG>(_hash.size()), 0);
    ThrowIfBCryptFailed(status, "failed to finish hash");
}
