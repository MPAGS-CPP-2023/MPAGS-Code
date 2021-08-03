#ifndef MPAGSCIPHER_PROCESSCOMMANDLINE_HPP
#define MPAGSCIPHER_PROCESSCOMMANDLINE_HPP

#include <string>
#include <vector>

struct ProgramSettings {
    /// Indicates the presence of the help flag in the arguments
    bool helpRequested;
    /// Indicates the presence of the version flag in the arguments
    bool versionRequested;
    /// Name of the input file
    std::string inputFile;
    /// Name of the output file
    std::string outputFile;
    /// Key to be used in encrypting/decrypting routine
    std::string cipherKey;
    /// Flag indicating whether the cipher should encrypt (true) or decrypt (false)
    bool encrypt;
};

bool processCommandLine(const std::vector<std::string>& cmdLineArgs,
                        ProgramSettings& settings);

#endif    // MPAGSCIPHER_PROCESSCOMMANDLINE_HPP