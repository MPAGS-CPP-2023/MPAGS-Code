#include "ProcessCommandLine.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

ProgramSettings processCommandLine(const std::vector<std::string>& cmdLineArgs)
{
    // Options that might be set by the command-line arguments
    ProgramSettings settings{false, false, "", "", {}, {}, CipherMode::Encrypt};

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
        } else if (cmdLineArgs[i] == "-v" || cmdLineArgs[i] == "--version") {
            // Set the indicator and terminate the loop
            settings.versionRequested = true;
            break;
        } else if (cmdLineArgs[i] == "--multi-cipher") {
            // Handle multi-cipher option
            // Next element should be the number of ciphers
            if (i == nCmdLineArgs - 1) {
                // Throw an exception to indicate the missing argument
                throw MissingArgument{
                    "--multi-cipher requires a positive integer argument"};
            } else {
                const std::string& arg{cmdLineArgs[i + 1]};
                // First, explicitly check for negative numbers - these will convert successfully but will not lead to expected results
                if (arg.front() == '-') {
                    throw InvalidArgument{
                        "--multi-cipher requires a positive integer argument, the supplied string (" +
                        arg + ") could not be successfully converted"};
                }
                // The conversion function will throw one of two possible exceptions
                // if the string does not represent a valid unsigned long integer
                try {
                    nExpectedCiphers = std::stoul(arg);
                } catch (const std::invalid_argument&) {
                    throw InvalidArgument{
                        "--multi-cipher requires a positive integer argument, the supplied string (" +
                        arg + ") could not be successfully converted"};
                } catch (const std::out_of_range&) {
                    throw InvalidArgument{
                        "--multi-cipher requires a positive integer argument, the supplied string (" +
                        arg + ") could not be successfully converted"};
                }
                settings.cipherType.reserve(nExpectedCiphers);
                settings.cipherKey.reserve(nExpectedCiphers);
                ++i;
            }
        } else if (cmdLineArgs[i] == "-i" || cmdLineArgs[i] == "--infile") {
            // Handle input file option
            // Next element is filename unless "-i" is the last argument
            if (i == nCmdLineArgs - 1) {
                // Throw an exception to indicate the missing argument
                throw MissingArgument{
                    "-i/--infile requires a filename argument"};
            } else {
                // Got filename, so assign value and advance past it
                settings.inputFile = cmdLineArgs[i + 1];
                ++i;
            }
        } else if (cmdLineArgs[i] == "-o" || cmdLineArgs[i] == "--outfile") {
            // Handle output file option
            // Next element is filename unless "-o" is the last argument
            if (i == nCmdLineArgs - 1) {
                // Throw an exception to indicate the missing argument
                throw MissingArgument{
                    "-o/--outfile requires a filename argument"};
            } else {
                // Got filename, so assign value and advance past it
                settings.outputFile = cmdLineArgs[i + 1];
                ++i;
            }
        } else if (cmdLineArgs[i] == "-k" || cmdLineArgs[i] == "--key") {
            // Handle cipher key option
            // Next element is the key unless -k is the last argument
            if (i == nCmdLineArgs - 1) {
                // Throw an exception to indicate the missing argument
                throw MissingArgument{"-k/--key requres a string argument"};
            } else {
                // Got the key, so assign the value and advance past it
                settings.cipherKey.push_back(cmdLineArgs[i + 1]);
                ++i;
            }
        } else if (cmdLineArgs[i] == "--encrypt") {
            settings.cipherMode = CipherMode::Encrypt;
        } else if (cmdLineArgs[i] == "--decrypt") {
            settings.cipherMode = CipherMode::Decrypt;
        } else if (cmdLineArgs[i] == "-c" || cmdLineArgs[i] == "--cipher") {
            // Handle cipher type option
            // Next element is the name of the cipher, unless -c is the last argument
            if (i == nCmdLineArgs - 1) {
                // Throw an exception to indicate the missing argument
                throw MissingArgument{"-c/--cipher requres a string argument"};
            } else {
                // Got the cipher name, so assign the value and advance past it
                if (cmdLineArgs[i + 1] == "caesar") {
                    settings.cipherType.push_back(CipherType::Caesar);
                } else if (cmdLineArgs[i + 1] == "playfair") {
                    settings.cipherType.push_back(CipherType::Playfair);
                } else if (cmdLineArgs[i + 1] == "vigenere") {
                    settings.cipherType.push_back(CipherType::Vigenere);
                } else {
                    throw UnknownArgument{"unknown cipher '" +
                                          cmdLineArgs[i + 1] + "'"};
                }
                ++i;
            }
        } else {
            // Have encoutered an unknown flag, so throw the corresponding exception
            throw UnknownArgument{"unknown argument '" + cmdLineArgs[i] + "'"};
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
        std::stringstream errorMsg;
        errorMsg << "expected types and keys for " << nExpectedCiphers
                 << " ciphers\n"
                 << "                                but received " << nTypes
                 << " types and " << nKeys << " keys";
        throw InconsistentArguments{errorMsg.str()};
    }

    return settings;
}
