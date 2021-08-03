#include "ProcessCommandLine.hpp"

#include <iostream>
#include <string>
#include <vector>

bool processCommandLine(const std::vector<std::string>& cmdLineArgs,
                        bool& helpRequested, bool& versionRequested,
                        std::string& inputFile, std::string& outputFile,
                        std::string& cipherKey, bool& encrypt)
{
    // Status flag to indicate whether or not the parsing was successful
    bool processStatus{true};

    // Process the arguments - ignore zeroth element, as we know this to be
    // the program name and don't need to worry about it
    const std::size_t nCmdLineArgs{cmdLineArgs.size()};
    for (std::size_t i{1}; i < nCmdLineArgs; ++i) {
        if (cmdLineArgs[i] == "-h" || cmdLineArgs[i] == "--help") {
            // Set the indicator and terminate the loop
            helpRequested = true;
            break;
        } else if (cmdLineArgs[i] == "--version") {
            // Set the indicator and terminate the loop
            versionRequested = true;
            break;
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
                inputFile = cmdLineArgs[i + 1];
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
                outputFile = cmdLineArgs[i + 1];
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
                cipherKey = cmdLineArgs[i + 1];
                ++i;
            }
        } else if (cmdLineArgs[i] == "--encrypt") {
            encrypt = true;
        } else if (cmdLineArgs[i] == "--decrypt") {
            encrypt = false;
        } else {
            // Have encoutered an unknown flag, output an error message,
            // set the flag to indicate the error and terminate the loop
            std::cerr << "[error] unknown argument '" << cmdLineArgs[i]
                      << "'\n";
            processStatus = false;
            break;
        }
    }
    return processStatus;
}