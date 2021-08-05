#ifndef MPAGSCIPHER_VIGENERECIPHER_HPP
#define MPAGSCIPHER_VIGENERECIPHER_HPP

#include "CaesarCipher.hpp"
#include "Cipher.hpp"
#include "CipherMode.hpp"
#include "CipherType.hpp"

#include <map>
#include <string>

/**
 * \file VigenereCipher.hpp
 * \brief Contains the declaration of the VigenereCipher class
 */

/**
 * \class VigenereCipher
 * \brief Encrypt or decrypt text using the Vigenere cipher with the given key
 */
class VigenereCipher : public Cipher {
  public:
    /**
     * \brief Create a new VigenereCipher with the given key
     *
     * \param key the key to use in the cipher
     *
     * \exception InvalidKey will be emitted if the supplied string is empty
     *                       after removing all non-alphabet characters
     */
    explicit VigenereCipher(const std::string& key);

    /**
     * \brief Set the key to be used for the encryption/decryption
     *
     * \param key the key to use in the cipher
     *
     * \exception InvalidKey will be emitted if the supplied string is empty
     *                       after removing all non-alphabet characters
     */
    void setKey(const std::string& key);

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
     * \brief Determine the type of cipher algorithm
     *
     * \return the cipher type
     */
    CipherType type() const override { return CipherType::Vigenere; }

  private:
    /**
     * \brief The common implementation for the applyCipher overloads
     *
     * \param [in,out] inputText the text to encrypt or decrypt
     * \param [in] cipherMode whether to encrypt or decrypt the input text
     */
    void applyCipherImpl(std::string& inputText,
                         const CipherMode cipherMode) const;

    /// The cipher key
    std::string key_{""};

    /// Lookup table to go from the character to the corresponding Caesar cipher
    std::map<char, CaesarCipher> charLookup_;
};

#endif
