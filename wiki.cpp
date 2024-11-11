#include "wiki.h"
#include <iostream>
#include <regex>

WikiParser::WikiParser(NeuralNet& net) : neuralNet(net) {}

// Extracts and processes a complete article, returning any remaining content
std::string WikiParser::processArticle(std::string& article) {
    size_t endPos = article.find("</page>"); // Adjust the tag as needed for your data

    if (endPos != std::string::npos) {
        std::string completeArticle = article.substr(0, endPos + 7); // Include the closing tag
        std::cout << "Processing article of size: " << completeArticle.size() << " bytes\n";
        std::cout << "Article title: " << extractTitle(completeArticle) << "\n";

        extractFeatures(completeArticle);

        // Preprocess the article content to generate the input vector
        std::vector<double> processedInput = neuralNet.preprocessArticle(completeArticle);

        // Check if the input size differs from the current number of input neurons
        if (processedInput.size() != neuralNet.getInputNeuronCount()) {
            std::cout << "Adjusting the number of input neurons from "
                      << neuralNet.getInputNeuronCount() << " to " << processedInput.size() << ".\n";

            // Adjust the input layer size dynamically in the neural network
            neuralNet.adjustInputLayerSize(processedInput.size());
        }

        // Train the neural network with the processed input
        neuralNet.train(processedInput);

        return article.substr(endPos + 7); // Return any remaining content
    }

    return article; // Return the original buffer if no complete article is found
}

std::string WikiParser::extractTitle(const std::string& article) {
    size_t start = article.find("<title>");
    size_t end = article.find("</title>");
    if (start != std::string::npos && end != std::string::npos) {
        return article.substr(start + 7, end - (start + 7));
    }
    return "Unknown Title";
}

void WikiParser::extractFeatures(const std::string& article) {
    // Check for disambiguation
    if (article.find("<disambiguation>") != std::string::npos) {
        std::cout << "Disambiguation found in article.\n";
    }

    // Extract categories
    std::regex categoryRegex("\\[\\[Category:(.*?)\\]\\]");
    auto categoriesBegin = std::sregex_iterator(article.begin(), article.end(), categoryRegex);
    auto categoriesEnd = std::sregex_iterator();

int categoryCount = std::distance(categoriesBegin, categoriesEnd);
std::cout << "Total categories detected: " << categoryCount << "\n";

    // Extract section headings
    std::regex headingRegex("==([^=]+)==");
    auto headingsBegin = std::sregex_iterator(article.begin(), article.end(), headingRegex);
int headingCount = std::distance(headingsBegin, std::sregex_iterator());
std::cout << "Total section headings detected: " << headingCount << "\n";

    // Detect links and references
    std::regex linkRegex("\\[\\[(.*?)\\]\\]");
    auto linksBegin = std::sregex_iterator(article.begin(), article.end(), linkRegex);
    auto linksEnd = std::sregex_iterator();

int linkCount = std::distance(linksBegin, linksEnd);
std::cout << "Total links detected: " << linkCount << "\n";

    // Check for redirects
    if (article.find("#REDIRECT") != std::string::npos) {
        std::cout << "Redirect detected in article.\n";
    }
}
