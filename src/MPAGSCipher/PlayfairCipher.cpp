#include "PlayfairCipher.hpp"
#include "Alphabet.hpp"

#include <algorithm>
#include <string>

PlayfairCipher::PlayfairCipher(const std::string& key)
{
    this->setKey(key);
}

void PlayfairCipher::setKey(const std::string& key)
{
    // Store the original key
    key_ = key;

    // Append the alphabet to the key
    key_ += Alphabet::alphabet;

    // Make sure the key is upper case
    std::transform(std::begin(key_), std::end(key_), std::begin(key_),
                   ::toupper);

    // Remove non-alphabet characters
    key_.erase(std::remove_if(std::begin(key_), std::end(key_),
                              [](char c) { return !std::isalpha(c); }),
               std::end(key_));

    // Change J -> I
    std::transform(std::begin(key_), std::end(key_), std::begin(key_),
                   [](char c) { return (c == 'J') ? 'I' : c; });

    // Remove duplicated letters
    std::string lettersFound{""};
    auto detectDuplicates = [&](char c) {
        if (lettersFound.find(c) == std::string::npos) {
            lettersFound += c;
            return false;
        } else {
            return true;
        }
    };
    key_.erase(
        std::remove_if(std::begin(key_), std::end(key_), detectDuplicates),
        std::end(key_));

    // Store the coordinates of each letter
    // (at this point the key length must be equal to the square of the grid dimension)
    for (std::size_t i{0}; i < keyLength_; ++i) {
        std::size_t row{i / gridSize_};
        std::size_t column{i % gridSize_};

        auto coords = std::make_pair(row, column);

        charLookup_[key_[i]] = coords;
        coordLookup_[coords] = key_[i];
    }
}

std::string PlayfairCipher::applyCipher(const std::string& inputText,
                                        const CipherMode cipherMode) const
{
    std::string outputText{inputText};

    this->applyCipherImpl(outputText, cipherMode);

    return outputText;
}

std::string PlayfairCipher::applyCipher(std::string&& inputText,
                                        const CipherMode cipherMode) const
{
    std::string outputText{std::move(inputText)};

    this->applyCipherImpl(outputText, cipherMode);

    return outputText;
}

void PlayfairCipher::applyCipherImpl(std::string& inputText,
                                     const CipherMode cipherMode) const
{
    // Change J -> I
    std::transform(std::begin(inputText), std::end(inputText),
                   std::begin(inputText),
                   [](char c) { return (c == 'J') ? 'I' : c; });

    // Find repeated characters (but only when they occur within a bigram)
    // and add an X (or a Q for repeated X's) between them
    std::string outputText{""};
    // Reserve space to hold the size of the input text plus a bit of headroom
    outputText.reserve(inputText.size() * 1.1);
    for (std::size_t i{0}; i < inputText.size(); i += 2) {
        // Always add the first of the bigram
        outputText += inputText[i];
        if (i + 1 == inputText.size()) {
            // If this was the last character then we've ended up with an odd-length input
            // so add Z to the end (or X if the last character is a Z)
            outputText += (inputText[i] == 'Z') ? 'X' : 'Z';
            // then explicitly break out of the loop since we've finished
            break;
        } else if (inputText[i] != inputText[i + 1]) {
            // If the two characters in the bigram are different,
            // simply add the second one as well
            outputText += inputText[i + 1];
        } else {
            // Otherwise, if two characters in the bigram are the same,
            // we instead add an X (or a Q if the first was an X)
            outputText += (inputText[i] == 'X') ? 'Q' : 'X';
            // Need to decrement i since the second character in this bigram now
            // becomes the first character in the next one
            --i;
        }
    }

    // Depending on encryption/decryption mode, set whether to increment or
    // decrement the column/row index (modulo the grid dimension)
    const std::size_t shift{
        (cipherMode == CipherMode::Encrypt) ? 1u : gridSize_ - 1u};

    // Loop over the input digraphs
    for (std::size_t i{0}; i < outputText.size(); i += 2) {
        // Find the coordinates in the grid for each digraph
        PlayfairCoords pointOne{charLookup_.at(outputText[i])};
        PlayfairCoords pointTwo{charLookup_.at(outputText[i + 1])};
        auto& [rowOne, columnOne]{pointOne};
        auto& [rowTwo, columnTwo]{pointTwo};

        // Find whether the two points are on a row, a column or form a rectangle/square
        // Then apply the appropriate rule to these coords to get new coords
        if (rowOne == rowTwo) {
            // Row - so increment/decrement the column indices (modulo the grid dimension)
            columnOne = (columnOne + shift) % gridSize_;
            columnTwo = (columnTwo + shift) % gridSize_;

        } else if (columnOne == columnTwo) {
            // Column - so increment/decrement the row indices (modulo the grid dimension)
            rowOne = (rowOne + shift) % gridSize_;
            rowTwo = (rowTwo + shift) % gridSize_;

        } else {
            // Rectangle/Square - so keep the rows the same and swap the columns
            // (NB the operation is actually the same regardless of encrypt/decrypt
            // since applying the same operation twice gets you back to where you were)
            std::swap(columnOne, columnTwo);
        }

        // Find the letters associated with the new coords and make the replacements
        outputText[i] = coordLookup_.at(pointOne);
        outputText[i + 1] = coordLookup_.at(pointTwo);
    }

    // Return the output text
    inputText.swap(outputText);
}
