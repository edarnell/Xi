#include "wiki.h"
#include "neural.h"
#include <iostream>
#include <fstream>
#include <bzlib.h>

int main() {
    // Initialize neural network and parser
    NeuralNet net({128, 64, 10}); // Example layer sizes
    WikiParser parser(net);

    // Open the .bz2 file
    FILE* file = fopen("wiki.xml.bz2", "rb");
    if (!file) {
        std::cerr << "Failed to open file.\n";
        return 1;
    }

    int bzError;
    BZFILE* bzf = BZ2_bzReadOpen(&bzError, file, 0, 0, nullptr, 0);
    if (bzError != BZ_OK) {
        std::cerr << "Error during bz2 read operation. bzError code: " << bzError << std::endl;
        fclose(file);
        return 1;
    }

    char buffer[4096];
    std::string articleBuffer;

    // Read and process chunks of the file
    while (true) {
        int bytesRead = BZ2_bzRead(&bzError, bzf, buffer, sizeof(buffer));
        if (bzError == BZ_OK || bzError == BZ_STREAM_END) {
            articleBuffer.append(buffer, bytesRead);
            
            // Print debug information for bytes read
            std::cout << "Read " << bytesRead << " bytes successfully.\n";

            // Check for end of file
            if (bzError == BZ_STREAM_END) {
                std::cout << "End of compressed stream reached.\n";
                break;
            }
        } else {
            std::cerr << "Error during bz2 read operation. bzError code: " << bzError << std::endl;
            break;
        }

        // Process articles if a complete one is found
        articleBuffer = parser.processArticle(articleBuffer);
    }

    // Clean up
    BZ2_bzReadClose(&bzError, bzf);
    fclose(file);
    std::cout << "Processing complete.\n";

    return 0;
}
