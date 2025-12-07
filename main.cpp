#include "src/FBToSVGConverter.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

int main() {
    fs::path inputDir = "resources/fbt";
    fs::path outputDir = "resources/svg";
    
    std::cout << "Input directory: " << fs::absolute(inputDir) << std::endl;
    std::cout << "Output directory: " << fs::absolute(outputDir) << std::endl;
    
    if (!fs::exists(inputDir)) {
        std::cerr << "ERROR: Input directory does not exist!" << std::endl;
        std::cin.get();
        return 1;
    }
    
    if (!fs::exists(outputDir)) {
        std::cout << "Creating output directory..." << std::endl;
        fs::create_directories(outputDir);
    }
    
    std::vector<fs::path> fbtFiles;
    int fileCount = 0;
    
    for (const auto& entry : fs::directory_iterator(inputDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".fbt") {
            fbtFiles.push_back(entry.path());
            fileCount++;
        }
    }
    
    if (fileCount == 0) {
        std::cout << "\nNo .fbt files found!" << std::endl;
        std::cin.get();
        return 1;
    }
    
    std::cout << "\nFound " << fileCount << " .fbt file(s)" << std::endl;
    
    FBToSVGConverter converter;
    int success = 0;
    
    for (const auto& inputFile : fbtFiles) {
        fs::path outputFile = outputDir / inputFile.stem().concat(".svg");
        
        if (converter.loadFromXML(inputFile.string())) {
            if (converter.createSVG(outputFile.string())) {
                success++;
            } else {
                std::cout << "Failed to create SVG" << std::endl;
            }
        } else {
            std::cout << "Failed to load FBT" << std::endl;
        }
    }
    
    std::cout << "RESULTS: " << success << "/" << fileCount << " files converted" << std::endl;
    
    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    
    return 0;
}