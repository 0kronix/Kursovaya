#include "src/FBToSVGConverter.h"
#include <argparse/argparse.hpp>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

std::vector<fs::path> getFbtFiles(const fs::path& directory) {
    std::vector<fs::path> files;
    
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        return files;
    }
    
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".fbt") {
            files.push_back(entry.path());
        }
    }
    
    return files;
}

bool createDirectoryIfNotExists(const fs::path& path) {
    if (!fs::exists(path)) {
        return fs::create_directories(path);
    }
    return true;
}

int main(int argc, char* argv[]) {
    argparse::ArgumentParser program("fb_to_svg_converter", "1.0.0");
    
    program.add_argument("-i", "--input")
        .help("Input directory with .fbt files")
        .default_value(std::string("resources/fbt"));
    
    program.add_argument("-o", "--output")
        .help("Output directory for .svg files")
        .default_value(std::string("resources/svg"));
    
    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }
    
    std::string inputDir = program.get<std::string>("--input");
    std::string outputDir = program.get<std::string>("--output");
    
    if (!createDirectoryIfNotExists(outputDir)) {
        std::cerr << "Error: Cannot create output directory: " << outputDir << std::endl;
        return 1;
    }
    
    std::vector<fs::path> fbtFiles = getFbtFiles(inputDir);
    
    if (fbtFiles.empty()) {
        std::cerr << "Error: No .fbt files found in directory: " << inputDir << std::endl;
        std::cerr << "Please add .fbt files to the directory or specify another directory with --input" << std::endl;
        return 1;
    }
    
    FBToSVGConverter converter;
    int successCount = 0;
    int failCount = 0;
    
    for (size_t i = 0; i < fbtFiles.size(); ++i) {
        const auto& inputFile = fbtFiles[i];
        fs::path outputFile = fs::path(outputDir) / inputFile.stem().concat(".svg");
        
        if (converter.loadFromXML(inputFile.string())) {
            std::cout << "  ✓ XML loaded successfully" << std::endl;
            if (converter.createSVG(outputFile.string())) {
                std::cout << "  ✓ SVG created successfully" << std::endl;
            } else {
                std::cout << "  ✗ Failed to create SVG" << std::endl;
            }
        } else {
            std::cout << "  ✗ Failed to load XML" << std::endl;
        }
    }
    
    return 0;
}