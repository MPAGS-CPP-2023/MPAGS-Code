//! Unit Tests for MPAGSCipher Cipher Classes
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <map>
#include <string>

#include "CaesarCipher.hpp"
#include "Cipher.hpp"
#include "CipherMode.hpp"
#include "CipherType.hpp"
#include "PlayfairCipher.hpp"
#include "VigenereCipher.hpp"

std::map<CipherType, std::string> plainText{
    {CipherType::Caesar, "HELLOWORLD"},
    {CipherType::Playfair,
     "BOBISXSOMESORTOFIUNIORCOMPLEXQXENOPHONEONEZEROTHINGZ"},
    {CipherType::Vigenere,
     "THISISQUITEALONGMESSAGESOTHEKEYWILLNEEDTOREPEATAFEWTIMES"}};

std::map<CipherType, std::string> cipherText{
    {CipherType::Caesar, "ROVVYGYBVN"},
    {CipherType::Playfair,
     "FHIQXLTLKLTLSUFNPQPKETFENIOLVSWLTFIAFTLAKOWATEQOKPPA"},
    {CipherType::Vigenere,
     "ALTDWZUFTHLEWZBNQPDGHKPDCALPVSFATWZUIPOHVVPASHXLQSDXTXSZ"}};

bool testCipher(const Cipher& cipher, const CipherMode mode,
                const std::string& inputText, const std::string& outputText)
{
    return cipher.applyCipher(inputText, mode) == outputText;
}

TEST_CASE("Cipher encryption", "[ciphers]")
{
    CaesarCipher cc{10};
    PlayfairCipher pc{"hello"};
    VigenereCipher vc{"hello"};

    REQUIRE(testCipher(cc, CipherMode::Encrypt, plainText[CipherType::Caesar],
                       cipherText[CipherType::Caesar]));
    REQUIRE(testCipher(pc, CipherMode::Encrypt, plainText[CipherType::Playfair],
                       cipherText[CipherType::Playfair]));
    REQUIRE(testCipher(vc, CipherMode::Encrypt, plainText[CipherType::Vigenere],
                       cipherText[CipherType::Vigenere]));
}

TEST_CASE("Cipher decryption", "[ciphers]")
{
    CaesarCipher cc{10};
    PlayfairCipher pc{"hello"};
    VigenereCipher vc{"hello"};

    REQUIRE(testCipher(cc, CipherMode::Decrypt, cipherText[CipherType::Caesar],
                       plainText[CipherType::Caesar]));
    REQUIRE(testCipher(pc, CipherMode::Decrypt,
                       cipherText[CipherType::Playfair],
                       plainText[CipherType::Playfair]));
    REQUIRE(testCipher(vc, CipherMode::Decrypt,
                       cipherText[CipherType::Vigenere],
                       plainText[CipherType::Vigenere]));
}