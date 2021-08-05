#include "CaesarCipher.hpp"
#include "Alphabet.hpp"

#include <string>

CaesarCipher::CaesarCipher(const std::size_t key) : key_{key % Alphabet::size}
{
}

CaesarCipher::CaesarCipher(const std::string& key) : key_{0}
{
    // We have the key as a string, but the Caesar cipher needs an unsigned long, so we first need to convert it
    // We default to having a key of 0, i.e. no encryption, if no (valid) key was provided on the command line
    if (!key.empty()) {
        // First, explicitly check for negative numbers - these will convert successfully but will not lead to expected results
        if (key.front() == '-') {
            throw InvalidKey(
                "Caesar cipher requires a positive long integer key, the supplied key (" +
                key + ") could not be successfully converted");
        }
        // The conversion function will throw one of two possible exceptions
        // if the string does not represent a valid unsigned long integer
        try {
            key_ = std::stoul(key) % Alphabet::size;
        } catch (const std::invalid_argument&) {
            throw InvalidKey(
                "Caesar cipher requires a positive long integer key, the supplied key (" +
                key + ") could not be successfully converted");
        } catch (const std::out_of_range&) {
            throw InvalidKey(
                "Caesar cipher requires a positive long integer key, the supplied key (" +
                key + ") could not be successfully converted");
        }
    }
}

std::string CaesarCipher::applyCipher(const std::string& inputText,
                                      const CipherMode cipherMode) const
{
    std::string outputText{inputText};

    this->applyCipherImpl(outputText, cipherMode);

    return outputText;
}

std::string CaesarCipher::applyCipher(std::string&& inputText,
                                      const CipherMode cipherMode) const
{
    std::string outputText{std::move(inputText)};

    this->applyCipherImpl(outputText, cipherMode);

    return outputText;
}

char CaesarCipher::applyCipher(const char inputChar,
                               const CipherMode cipherMode) const
{
    // Determine the appropriate shift
    // (depending on whether we're encrypting or decrypting)
    const std::size_t shift{
        (cipherMode == CipherMode::Encrypt) ? key_ : Alphabet::size - key_};

    return this->shiftedChar(inputChar, shift);
}

void CaesarCipher::applyCipherImpl(std::string& inputText,
                                   const CipherMode cipherMode) const
{
    // Determine the appropriate shift
    // (depending on whether we're encrypting or decrypting)
    const std::size_t shift{
        (cipherMode == CipherMode::Encrypt) ? key_ : Alphabet::size - key_};

    // Loop over the input text
    for (auto& currentChar : inputText) {
        // Determine the new character and update in-place
        currentChar = this->shiftedChar(currentChar, shift);
    }
}

char CaesarCipher::shiftedChar(const char inputChar,
                               const std::size_t shift) const
{
    // Find the corresponding position in the alphabet container
    const std::size_t index{Alphabet::alphabet.find(inputChar)};

    // Determine the new character and return it
    return Alphabet::alphabet[(index + shift) % Alphabet::size];
}
