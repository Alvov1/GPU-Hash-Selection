#ifndef HASHSELECTION_HOSTHASH_H
#define HASHSELECTION_HOSTHASH_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <array>
#include <openssl/evp.h>

#include "OssException.h"

template<typename Char>
class HostSHA256 final {
    unsigned char bytes[EVP_MAX_MD_SIZE] {};
    unsigned bytesLength = 0;
public:
    HostSHA256() = delete;
    HostSHA256(const Char* data, size_t length);

    std::basic_string<Char> to_string() const;

    HostSHA256(const HostSHA256 &copy) = default;
    HostSHA256 &operator=(const HostSHA256 &assign) = default;
    HostSHA256(HostSHA256 &&move) noexcept = default;
    HostSHA256& operator=(HostSHA256 &&moveAssign) noexcept = default;
};

template<typename Char>
HostSHA256<Char>::HostSHA256(const Char *data, size_t length) {
    const size_t byteLength = length * (sizeof(Char) / sizeof(char));

    EVP_MD_CTX *context = EVP_MD_CTX_new();
    if (context == nullptr)
        throw OssException("EVP_MD_CTX_new");

    if (0 == EVP_DigestInit_ex(context, EVP_sha256(), nullptr))
        throw OssException("EVP_DigestInit_ex");

    if (0 == EVP_DigestUpdate(context, data, byteLength))
        throw OssException("EVP_DigestUpdate");

    if (0 == EVP_DigestFinal_ex(context, bytes, &bytesLength))
        throw OssException("EVP_DigestFinal_ex");

    EVP_MD_CTX_free(context);
}

template<typename Char>
std::basic_string<Char> HostSHA256<Char>::to_string() const {
    std::basic_stringstream<Char> ss;
    for (unsigned i = 0; i < bytesLength; ++i)
        ss << std::hex << std::setw(2) << std::setfill(Char('0')) << static_cast<int>(bytes[i]);
    return ss.str();
}

#endif //HASHSELECTION_HOSTHASH_H