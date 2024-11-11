#ifndef WIKI_H
#define WIKI_H

#include <string>
#include "neural.h"

class WikiParser {
public:
    // Constructor
    WikiParser(NeuralNet& net);

    // Method to process an article and train the network
    std::string processArticle(std::string& article);

private:
    // Method to extract and print features from an article
    void extractFeatures(const std::string& article);

    // Reference to the neural network instance
    NeuralNet& neuralNet;

    // Helper method to extract the title from an article
    std::string extractTitle(const std::string& article);
};

#endif // WIKI_H
