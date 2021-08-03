#ifndef MPAGSCIPHER_CAESARCIPHER_HPP
#define MPAGSCIPHER_CAESARCIPHER_HPP

#include <cstddef>
#include <string>

/// Encrypt or decrypt text using the Caesar cipher with the given key
class CaesarCipher {
  public:
    /// Create a new CaesarCipher, with the given key
    explicit CaesarCipher(const std::size_t key);

    /// Create a new CaesarCipher, converting the given string into the key
    explicit CaesarCipher(const std::string& key);

    std::size_t key_{0};
};

#endif