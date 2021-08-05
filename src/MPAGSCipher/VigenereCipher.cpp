#include "VigenereCipher.hpp"
#include "Alphabet.hpp"

#include <algorithm>
#include <iostream>
#include <string>

VigenereCipher::VigenereCipher(const std::string& key)
{
    this->setKey(key);
}

void VigenereCipher::setKey(const std::string& key)
{
    // Store the original key
    key_ = key;

    // Make sure the key is upper case
    std::transform(std::begin(key_), std::end(key_), std::begin(key_),
                   ::toupper);

    // Remove non-alphabet characters
    key_.erase(std::remove_if(std::begin(key_), std::end(key_),
                              [](char c) { return !std::isalpha(c); }),
               std::end(key_));

    // Check that the key is not now empty
    if (key_.empty()) {
        // If it is we would ideally throw an exception to indicate a problem (see Day 6)
        // For now we'll just set it to be some default value and print a warning
        key_ = "VIGENEREEXAMPLE";
        std::cerr
            << "[warning] key provide to VigenereCipher is empty, setting it to the default: "
            << key_ << std::endl;
    }

    // Loop through the key
    charLookup_.clear();
    for (const char letter : key_) {
        // Check if we've already seen this letter before
        // NB C++20 will allow us to be clearer:
        // if (charLookup_.contains(letter))
        if (charLookup_.find(letter) != charLookup_.end()) {
            continue;
        }

        // Find the position of the letter in the alphabet
        const std::size_t index{Alphabet::alphabet.find(letter)};

        // Construct a Caesar cipher object in-place using the position as the key,
        // storing it so that it can be retrieved based on the character in the key
        charLookup_.emplace(std::piecewise_construct, std::make_tuple(letter),
                            std::make_tuple(index));
    }
}

std::string VigenereCipher::applyCipher(const std::string& inputText,
                                        const CipherMode cipherMode) const
{
    // Store the size of the input text and of the key
    const std::size_t inputSize{inputText.size()};
    const std::size_t keySize{key_.size()};

    // Create the output string, reserving space for
    // as many characters as are in the input text
    std::string outputText;
    outputText.reserve(inputSize);

    // Loop through the text
    for (std::size_t i{0}; i < inputSize; ++i) {
        // Determine the corresponding character in the key based on the position
        // (repeating the key if necessary)
        const char keyChar{key_[i % keySize]};

        // Find the corresponding Caesar cipher
        const CaesarCipher& cipher{charLookup_.at(keyChar)};

        // Use it to encrypt/decrypt the character of the text
        // and put the new character into the output
        const std::string oldChar{inputText[i]};
        outputText += cipher.applyCipher(oldChar, cipherMode);
    }

    // Return the output text
    return outputText;
}