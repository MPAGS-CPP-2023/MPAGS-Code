#ifndef MPAGSCIPHER_CIPHERMODE_HPP
#define MPAGSCIPHER_CIPHERMODE_HPP

/// Enumeration to define the modes in which the ciphers can operate
enum class CipherMode {
    Encrypt,    ///< Apply cipher in encrypt mode
    Decrypt     ///< Apply cipher in decrypt mode
};

#endif    // MPAGSCIPHER_CIPHERMODE_HPP