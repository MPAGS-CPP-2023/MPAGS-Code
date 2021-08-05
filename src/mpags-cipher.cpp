#include "CipherFactory.hpp"
#include "CipherMode.hpp"
#include "CipherType.hpp"
#include "ProcessCommandLine.hpp"
#include "TransformChar.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <future>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    // Convert the command-line arguments into a more easily usable form
    const std::vector<std::string> cmdLineArgs{argv, argv + argc};

    // Process command line arguments
    ProgramSettings settings;
    try {
        settings = processCommandLine(cmdLineArgs);

    } catch (const MissingArgument& e) {
        std::cerr << "[error] Missing argument: " << e.what() << std::endl;
        return 1;

    } catch (const UnknownArgument& e) {
        std::cerr << "[error] Unknown argument: " << e.what() << std::endl;
        return 1;
    } catch (const InvalidArgument& e) {
        std::cerr << "[error] Invalid argument: " << e.what() << std::endl;
        return 1;
    } catch (const InconsistentArguments& e) {
        std::cerr << "[error] Inconsistent arguments: " << e.what()
                  << std::endl;
        return 1;
    }

    // Handle help, if requested
    if (settings.helpRequested) {
        // Line splitting for readability
        std::cout
            << "Usage: mpags-cipher [-i/--infile <file>] [-o/--outfile <file>] [--multi-cipher <N_ciphers>] [-c/--cipher <cipher>] [-k/--key <key>] [--encrypt/--decrypt]\n\n"
            << "Encrypts/Decrypts input alphanumeric text using classical ciphers\n\n"
            << "Available options:\n\n"
            << "  -h|--help\n"
            << "                      Print this help message and exit\n\n"
            << "  -v|--version\n"
            << "                      Print version information\n\n"
            << "  -i|--infile FILE\n"
            << "                      Read text to be processed from FILE\n"
            << "                      Stdin will be used if not supplied\n\n"
            << "  -o|--outfile FILE\n"
            << "                      Write processed text to FILE\n"
            << "                      Stdout will be used if not supplied\n\n"
            << "  --multi-cipher N\n"
            << "                      Specify the number of ciphers to be used in sequence\n"
            << "                      N should be a positive integer - defaults to 1\n\n"
            << "  -c|--cipher CIPHER\n"
            << "                      Specify the cipher to be used to perform the encryption/decryption\n"
            << "                      CIPHER can be caesar, playfair or vigenere - caesar is the default\n\n"
            << "  -k|--key KEY\n"
            << "                      Specify the cipher KEY\n"
            << "                      A null key, i.e. no encryption, is used if not supplied\n\n"
            << "  --encrypt\n"
            << "                      Will use the cipher to encrypt the input text (default behaviour)\n\n"
            << "  --decrypt\n"
            << "                      Will use the cipher to decrypt the input text\n\n"
            << std::endl;
        // Help requires no further action, so return from main
        // with 0 used to indicate success
        return 0;
    }

    // Handle version, if requested
    // Like help, requires no further action,
    // so return from main with zero to indicate success
    if (settings.versionRequested) {
        std::cout << "0.5.0" << std::endl;
        return 0;
    }

    // Initialise variables
    char inputChar{'x'};
    std::string cipherText;

    // Read in user input from stdin/file
    if (!settings.inputFile.empty()) {
        // Open the file and check that we can read from it
        std::ifstream inputStream{settings.inputFile};
        if (!inputStream.good()) {
            std::cerr << "[error] failed to create istream on file '"
                      << settings.inputFile << "'" << std::endl;
            return 1;
        }

        // Loop over each character from the file
        while (inputStream >> inputChar) {
            cipherText += transformChar(inputChar);
        }

    } else {
        // Loop over each character from user input
        // (until Return then CTRL-D (EOF) pressed)
        while (std::cin >> inputChar) {
            cipherText += transformChar(inputChar);
        }
    }

    // Request construction of the appropriate cipher(s)
    std::vector<std::unique_ptr<Cipher>> ciphers;
    std::size_t nCiphers{settings.cipherType.size()};
    ciphers.reserve(nCiphers);
    for (std::size_t iCipher{0}; iCipher < nCiphers; ++iCipher) {
        try {
            ciphers.push_back(CipherFactory::makeCipher(
                settings.cipherType[iCipher], settings.cipherKey[iCipher]));
        } catch (const InvalidKey& e) {
            std::cerr << "[error] Invalid key: " << e.what() << std::endl;
            return 1;
        }

        // Check that the cipher was constructed successfully
        if (!ciphers.back()) {
            std::cerr << "[error] problem constructing requested cipher"
                      << std::endl;
            return 1;
        }
    }

    // If we are decrypting, we need to reverse the order of application of the ciphers
    if (settings.cipherMode == CipherMode::Decrypt) {
        std::reverse(ciphers.begin(), ciphers.end());
    }

    // Run the cipher(s) on the input text, specifying whether to encrypt/decrypt
    for (const auto& cipher : ciphers) {
        // Multi-threading of the CaesarCipher encryption
        if (cipher->type() == CipherType::Caesar) {
            // Placeholder for the output text
            std::string outputText{""};

            // TODO: the number of threads should be configurable at the command line!
            constexpr std::size_t numThreads{4};
            std::vector<std::string> threadText;
            threadText.resize(numThreads);
            std::vector<std::future<std::string>> futures;
            futures.reserve(numThreads);

            // Make sure the chunk size * number of threads will cover the whole input text
            // If the input is very small then we'll over cover, hence the try-catch around substr below
            const std::size_t inputSize{cipherText.size()};
            const std::size_t chunkSize{(inputSize % numThreads)
                                            ? (1 + inputSize / numThreads)
                                            : (inputSize / numThreads)};

            // Create the threads
            for (std::size_t i{0}; i < numThreads; i++) {
                // Get the chunk of text for this thread
                try {
                    threadText[i] = cipherText.substr(i * chunkSize, chunkSize);
                } catch (std::out_of_range& e) {
                    // We've already covered the whole string, so can break out
                    break;
                }

                // Start the thread with the given lambda - need to capture the index by value
                // as otherwise it will have changed in the next loop iteration
                futures.push_back(std::async(
                    std::launch::async, [&threadText, i, &cipher, &settings]() {
                        return cipher->applyCipher(std::move(threadText[i]),
                                                   settings.cipherMode);
                    }));
            }

            // Now wait for each thread to finish
            bool complete{false};
            do {
                // Set the flag to completed and set it back if we find incomplete threads
                complete = true;
                for (auto& future : futures) {
                    auto status = future.wait_for(std::chrono::seconds(10));
                    if (status != std::future_status::ready) {
                        complete = false;
                    }
                }
            } while (!complete);

            // Concatenate all the output text
            for (auto& future : futures) {
                outputText += future.get();
            }

            cipherText.swap(outputText);

        } else {
            // For the other ciphers, run synchronously
            // Run the cipher on the input text, specifying whether to encrypt/decrypt
            cipherText = cipher->applyCipher(std::move(cipherText), settings.cipherMode);
        }
    }

    // Output the encrypted/decrypted text to stdout/file
    if (!settings.outputFile.empty()) {
        // Open the file and check that we can write to it
        std::ofstream outputStream{settings.outputFile};
        if (!outputStream.good()) {
            std::cerr << "[error] failed to create ostream on file '"
                      << settings.outputFile << "'" << std::endl;
            return 1;
        }

        // Print the encrypted/decrypted text to the file
        outputStream << cipherText << std::endl;

    } else {
        // Print the encrypted/decrypted text to the screen
        std::cout << cipherText << std::endl;
    }

    // No requirement to return from main, but we do so for clarity
    // and for consistency with other functions
    return 0;
}
