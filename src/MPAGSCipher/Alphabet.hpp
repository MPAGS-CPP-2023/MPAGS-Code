#ifndef MPAGSCIPHER_ALPHABET_HPP
#define MPAGSCIPHER_ALPHABET_HPP

#include <string>

/**
 * \file Alphabet.hpp
 * \brief Contains the declaration of the Alphabet namespace, which holds various commonly used constants
 */

/**
 * \namespace Alphabet
 * \brief Namespace to hold alphabet-related constants
 */
namespace Alphabet {
    /// The alphabet
    const std::string alphabet{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};

    /// The size of the alphabet
    const std::size_t size{alphabet.size()};
}    // namespace Alphabet

#endif    // MPAGSCIPHER_ALPHABET_HPP