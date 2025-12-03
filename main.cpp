#include "FBToSVGConverter.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

int main() {
    std::string inputDir = "resources/fbt";
    std::string outputDir = "resources/svg";
    
    std::vector<std::string> fbtFiles;
    
    try {
        for (const auto& entry : fs::directory_iterator(inputDir)) {
            if (entry.is_regular_file()) {
                std::string path = entry.path().string();
                fbtFiles.push_back(path);
            }
        }
        
        FBToSVGConverter converter;
        int successCount = 0;
        int failCount = 0;
        
        for (const auto& inputFile : fbtFiles) {
            fs::path pathObj(inputFile);
            std::string filename = pathObj.stem().string();
            std::string outputFile = outputDir + "/" + filename + ".svg";
            
            converter.loadFromXML(inputFile);
            converter.createSVG(outputFile);
        }
        
    } catch (const std::exception& e) {
        return 1;
    }

    return 0;
}