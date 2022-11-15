#include "ProcessCommandLine.hpp"

#include <iostream>
#include <string>
#include <vector>

bool processCommandLine(const std::vector<std::string>& cmdLineArgs,
                        ProgramSettings& settings)
{
    // Status flag to indicate whether or not the parsing was successful
    bool processStatus{true};

    // Default to expecting information about one cipher
    // (modified by the --multi-cipher argument)
    std::size_t nExpectedCiphers{1};
    settings.cipherType.reserve(nExpectedCiphers);
    settings.cipherKey.reserve(nExpectedCiphers);

    // Process the arguments - ignore zeroth element, as we know this to be
    // the program name and don't need to worry about it
    const std::size_t nCmdLineArgs{cmdLineArgs.size()};
    for (std::size_t i{1}; i < nCmdLineArgs; ++i) {
        if (cmdLineArgs[i] == "-h" || cmdLineArgs[i] == "--help") {
            // Set the indicator and terminate the loop
            settings.helpRequested = true;
            break;
        } else if (cmdLineArgs[i] == "--version") {
            // Set the indicator and terminate the loop
            settings.versionRequested = true;
            break;
        } else if (cmdLineArgs[i] == "--multi-cipher") {
            // Handle multi-cipher option
            // Next element should be the number of ciphers
            if (i == nCmdLineArgs - 1) {
                std::cerr
                    << "[error] --multi-cipher requires a positive integer argument"
                    << std::endl;
                // Set the flag to indicate the error and terminate the loop
                processStatus = false;
                break;
            } else {
                // Before doing the conversion we should check that the string contains a
                // valid positive integer.
                // Here we do that by looping through each character and checking that it
                // is a digit. What is rather hard to check is whether the number is too
                // large to be represented by an unsigned long, so we've omitted that for
                // the time being.
                // (Since the conversion function std::stoul will throw an exception if the
                // string does not represent a valid unsigned long, we could check for and
                // handle that instead but we only cover exceptions very briefly on the
                // final day of this course - they are a very complex area of C++ that
                // could take an entire course on their own!)
                const std::string& arg{cmdLineArgs[i + 1]};
                for (const auto& elem : arg) {
                    if (!std::isdigit(elem)) {
                        std::cerr
                            << "[error] --multi-cipher requires a positive integer argument,\n"
                            << "        the supplied string (" << arg
                            << ") could not be successfully converted"
                            << std::endl;
                        return false;
                    }
                }
                nExpectedCiphers = std::stoul(arg);
                settings.cipherType.reserve(nExpectedCiphers);
                settings.cipherKey.reserve(nExpectedCiphers);
                ++i;
            }
        } else if (cmdLineArgs[i] == "-i") {
            // Handle input file option
            // Next element is filename unless "-i" is the last argument
            if (i == nCmdLineArgs - 1) {
                std::cerr << "[error] -i requires a filename argument"
                          << std::endl;
                // Set the flag to indicate the error and terminate the loop
                processStatus = false;
                break;
            } else {
                // Got filename, so assign value and advance past it
                settings.inputFile = cmdLineArgs[i + 1];
                ++i;
            }
        } else if (cmdLineArgs[i] == "-o") {
            // Handle output file option
            // Next element is filename unless "-o" is the last argument
            if (i == nCmdLineArgs - 1) {
                std::cerr << "[error] -o requires a filename argument"
                          << std::endl;
                // Set the flag to indicate the error and terminate the loop
                processStatus = false;
                break;
            } else {
                // Got filename, so assign value and advance past it
                settings.outputFile = cmdLineArgs[i + 1];
                ++i;
            }
        } else if (cmdLineArgs[i] == "-k") {
            // Handle cipher key option
            // Next element is the key unless -k is the last argument
            if (i == nCmdLineArgs - 1) {
                std::cerr << "[error] -k requires a positive integer argument"
                          << std::endl;
                // Set the flag to indicate the error and terminate the loop
                processStatus = false;
                break;
            } else {
                // Got the key, so assign the value and advance past it
                settings.cipherKey.push_back(cmdLineArgs[i + 1]);
                ++i;
            }
        } else if (cmdLineArgs[i] == "--encrypt") {
            settings.cipherMode = CipherMode::Encrypt;
        } else if (cmdLineArgs[i] == "--decrypt") {
            settings.cipherMode = CipherMode::Decrypt;
        } else if (cmdLineArgs[i] == "-c") {
            // Handle cipher type option
            // Next element is the name of the cipher, unless -c is the last argument
            if (i == nCmdLineArgs - 1) {
                std::cerr << "[error] -c requires a string argument"
                          << std::endl;
                // Set the flag to indicate the error and terminate the loop
                processStatus = false;
                break;
            } else {
                // Got the cipher name, so assign the value and advance past it
                if (cmdLineArgs[i + 1] == "caesar") {
                    settings.cipherType.push_back(CipherType::Caesar);
                } else if (cmdLineArgs[i + 1] == "playfair") {
                    settings.cipherType.push_back(CipherType::Playfair);
                } else if (cmdLineArgs[i + 1] == "vigenere") {
                    settings.cipherType.push_back(CipherType::Vigenere);
                } else {
                    std::cerr << "[error] unknown cipher '"
                              << cmdLineArgs[i + 1] << "'\n";
                    processStatus = false;
                    break;
                }
                ++i;
            }
        } else {
            // Have encoutered an unknown flag, output an error message,
            // set the flag to indicate the error and terminate the loop
            std::cerr << "[error] unknown argument '" << cmdLineArgs[i]
                      << "'\n";
            processStatus = false;
            break;
        }
    }

    // For backward compatibility we allow (for a single cipher) nothing to be
    // specified and default to using Caesar cipher and/or an empty string key
    if (nExpectedCiphers == 1) {
        if (settings.cipherType.empty()) {
            settings.cipherType.push_back(CipherType::Caesar);
        }
        if (settings.cipherKey.empty()) {
            settings.cipherKey.push_back("");
        }
    }

    // Check that we have information on the expected number of ciphers
    const std::size_t nTypes{settings.cipherType.size()};
    const std::size_t nKeys{settings.cipherKey.size()};
    if (nTypes != nExpectedCiphers || nKeys != nExpectedCiphers) {
        std::cerr << "[error] expected types and keys for " << nExpectedCiphers
                  << " ciphers\n"
                  << "        but received " << nTypes << " types and " << nKeys
                  << " keys" << std::endl;
        processStatus = false;
    }

    return processStatus;
}