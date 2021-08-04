#include "VigenereCipher.hpp"

#include <algorithm>
#include <string>

VigenereCipher::VigenereCipher(const std::string& key)
{
    this->setKey(key);
}

void VigenereCipher::setKey(const std::string& key)
{
    // Store the original key
    key_ = key;

    // Loop through the key

    // - Find the position of the letter in the alphabet

    // - Construct a Caesar cipher object using the position as the key

    // - Store the Caesar cipher object so that it can be retrieved based on the character in the key
}

std::string VigenereCipher::applyCipher(const std::string& inputText,
                                        const CipherMode /*cipherMode*/) const
{
    // Create the output string, initially a copy of the input text
    std::string outputText{inputText};

    // Loop through the text

    // - Determine the corresponding character in the key based on the position
    //   (repeating the key if necessary)

    // - Find the corresponding Caesar cipher

    // - Use it to encrypt/decrypt the character of the text

    // - Put the new character into the output

    // Return the output text
    return outputText;
}