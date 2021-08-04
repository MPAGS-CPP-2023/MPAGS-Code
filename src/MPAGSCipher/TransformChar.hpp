#ifndef MPAGSCIPHER_TRANSFORMCHAR_HPP
#define MPAGSCIPHER_TRANSFORMCHAR_HPP

#include <string>

/**
 * \file TransformChar.hpp
 * \brief Contains the declaration of the function for pre-processing the input text
 */

/**
 * \brief Transliterate char to string
 *
 * - Alphabet characters are made uppercase
 * - Digits are replaced with their English word equivalents, e.g. '0' -> "ZERO"
 * - All other characters (e.g. punctuation, spaces) are discarded
 *
 * \param inputChar the character to process
 * \return the transliterated string
 */
std::string transformChar(const char inputChar);

#endif    // MPAGSCIPHER_TRANSFORMCHAR_HPP