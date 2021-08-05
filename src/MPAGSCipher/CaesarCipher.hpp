#ifndef MPAGSCIPHER_CAESARCIPHER_HPP
#define MPAGSCIPHER_CAESARCIPHER_HPP

#include "Cipher.hpp"
#include "CipherMode.hpp"
#include "CipherType.hpp"

#include <cstddef>
#include <string>
#include <vector>

/**
 * \file CaesarCipher.hpp
 * \brief Contains the declaration of the CaesarCipher class
 */

/**
 * \class CaesarCipher
 * \brief Encrypt or decrypt text using the Caesar cipher with the given key
 */
class CaesarCipher : public Cipher {
  public:
    /**
     * \brief Create a new CaesarCipher with the given key
     *
     * \param key the key to use in the cipher
     */
    explicit CaesarCipher(const std::size_t key);

    /**
     * \brief Create a new CaesarCipher, converting the given string into the key
     *
     * \param key the string to convert into the key to be used in the cipher
     *
     * \exception InvalidKey will be emitted if the supplied string cannot be
     *                       successfully converted to a positive long integer
     */
    explicit CaesarCipher(const std::string& key);

    /**
     * \brief Apply the cipher to the provided text
     *
     * This version copies the input string
     *
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
    std::string applyCipher(const std::string& inputText,
                            const CipherMode cipherMode) const override;

    /**
     * \brief Apply the cipher to the provided text
     *
     * This version moves the input string
     *
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
    std::string applyCipher(std::string&& inputText,
                            const CipherMode cipherMode) const override;

    /**
     * \brief Apply the cipher to the provided single character
     *
     * \param inputChar the character to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input
     * \return the result of applying the cipher to the input
     */
    char applyCipher(const char inputChar, const CipherMode cipherMode) const;

    /**
     * \brief Determine the type of cipher algorithm
     *
     * \return the cipher type
     */
    CipherType type() const override { return CipherType::Caesar; }

  private:
    /**
     * \brief The common implementation for the std::string applyCipher overloads
     *
     * \param [in,out] inputText the text to encrypt or decrypt
     * \param [in] cipherMode whether to encrypt or decrypt the input text
     */
    void applyCipherImpl(std::string& inputText,
                         const CipherMode cipherMode) const;

    /**
     * \brief The common implementation for all applyCipher overloads
     *
     * \param [in] inputChar the character to encrypt or decrypt
     * \param [in] shift the shift to apply to encrypt/decrypt the character
     */
    char shiftedChar(const char inputChar, const std::size_t shift) const;

    /// The cipher key, essentially a constant shift to be applied
    std::size_t key_{0};
};

#endif
