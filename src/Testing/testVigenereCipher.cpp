//! Unit Tests for MPAGSCipher CaesarCipher Class
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "VigenereCipher.hpp"

// Need to wrap the construction in a lambda to use it in REQUIRE_THROWS_AS and REQUIRE_NOTHROW
auto makeVigenereCipher = [](const std::string& key) {
    VigenereCipher cc{key};
};

TEST_CASE("Vigenere Cipher encryption/decryption", "[vigenere]")
{
    const std::string key{"hello"};
    const std::string plainText{
        "THISISQUITEALONGMESSAGESOTHEKEYWILLNEEDTOREPEATAFEWTIMES"};
    const std::string cipherText{
        "ALTDWZUFTHLEWZBNQPDGHKPDCALPVSFATWZUIPOHVVPASHXLQSDXTXSZ"};

    REQUIRE_NOTHROW(makeVigenereCipher(key));
    VigenereCipher cc{key};

    SECTION("encryption")
    {
        REQUIRE(cc.applyCipher(plainText, CipherMode::Encrypt) == cipherText);
    }
    SECTION("decryption")
    {
        REQUIRE(cc.applyCipher(cipherText, CipherMode::Decrypt) == plainText);
    }
}

TEST_CASE("Vigenere Cipher bad key", "[vigenere]")
{
    REQUIRE_THROWS_AS(makeVigenereCipher(""), InvalidKey);
    REQUIRE_THROWS_AS(makeVigenereCipher("!"), InvalidKey);
    REQUIRE_THROWS_AS(makeVigenereCipher("1"), InvalidKey);
}