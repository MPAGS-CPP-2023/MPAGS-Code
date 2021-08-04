#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

#include "CipherMode.hpp"

#include <map>
#include <string>

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */
class PlayfairCipher {
  public:
    /**
     * \brief Create a new PlayfairCipher with the given key
     *
     * \param key the key to use in the cipher
     */
    explicit PlayfairCipher(const std::string& key);

    /**
     * \brief Set the key to be used for the encryption/decryption
     *
     * \param key the key to use in the cipher
     */
    void setKey(const std::string& key);

    /**
     * \brief Apply the cipher to the provided text
     *
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
    std::string applyCipher(const std::string& inputText,
                            const CipherMode cipherMode) const;

  private:
    /// The grid size
    const std::string::size_type gridSize_{5};

    /// The key length = grid size^2
    const std::string::size_type keyLength_{gridSize_ * gridSize_};

    /// The cipher key
    std::string key_{""};

    // Lookup tables generated from the key

    /// Type definition for the coordinates in the 5x5 table
    using PlayfairCoords = std::pair<std::size_t, std::size_t>;

    /// Lookup table to go from the character to the coordinates
    std::map<char, PlayfairCoords> charLookup_;

    /// Lookup table to go from the coordinates to the character
    std::map<PlayfairCoords, char> coordLookup_;
};

#endif