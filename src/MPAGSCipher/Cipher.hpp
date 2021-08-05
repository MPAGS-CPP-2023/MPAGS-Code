#ifndef MPAGSCIPHER_CIPHER_HPP
#define MPAGSCIPHER_CIPHER_HPP

#include "CipherMode.hpp"
#include "CipherType.hpp"

#include <stdexcept>
#include <string>

/**
 * \file Cipher.hpp
 * \brief Contains the declaration of the purely abstract Cipher base class
 */

/**
 * \class Cipher
 * \brief Defines the interface for a cipher
 *
 * A purely abstract base class that defines the interface for a cipher
 *
 * It can be used as follows:
 * \code{.cpp}
 * class MyCipher : public Cipher {...};
 * \endcode
 */
class Cipher {
  public:
    /**
     * \brief Apply the cipher to the provided text
     *
     * This version copies the input string
     *
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
    virtual std::string applyCipher(const std::string& inputText,
                                    const CipherMode cipherMode) const = 0;

    /**
     * \brief Apply the cipher to the provided text
     *
     * This version moves the input string
     *
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
    virtual std::string applyCipher(std::string&& inputText,
                                    const CipherMode cipherMode) const = 0;

    /**
     * \brief Determine the type of cipher algorithm
     *
     * \return the cipher type
     */
    virtual CipherType type() const = 0;

    /// Default constructor
    Cipher() = default;
    /// Default copy constructor
    Cipher(const Cipher& rhs) = default;
    /// Default move constructor
    Cipher(Cipher&& rhs) = default;
    /// Default copy assignment operator
    Cipher& operator=(const Cipher& rhs) = default;
    /// Default move assignment operator
    Cipher& operator=(Cipher&& rhs) = default;
    /// Make the default destructor virtual
    virtual ~Cipher() = default;
};

/**
 * \class InvalidKey
 * \brief Exception object indicating an invalid key
 *
 * Exception object to indicate that the key provided to the given cipher is invalid
 */
class InvalidKey : public std::invalid_argument {
  public:
    /**
     * \brief Construct an InvalidKey exception
     * \param what the diagnostic message
     */
    InvalidKey(const std::string& what) : std::invalid_argument(what) {}
};

#endif
