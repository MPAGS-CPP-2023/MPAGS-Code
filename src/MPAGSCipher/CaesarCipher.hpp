#ifndef MPAGSCIPHER_CAESARCIPHER_HPP
#define MPAGSCIPHER_CAESARCIPHER_HPP

#include <cstddef>

/// Encrypt or decrypt text using the Caesar cipher with the given key
class CaesarCipher {
  public:
    CaesarCipher(const std::size_t key);

    std::size_t key_{0};
};

#endif