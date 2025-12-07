#include "FBToSVGConverter.h"
#include <fstream>
#include <sstream>
#include <algorithm>

FBToSVGConverter::FBToSVGConverter() {}
FBToSVGConverter::~FBToSVGConverter() {}

bool FBToSVGConverter::loadFromXML(const std::string& filename) {
    return parseXML(filename);
}

bool FBToSVGConverter::createSVG(const std::string& outputFilename) {
    int width, height;
    calculateDimensions(width, height);
    
    std::ofstream file(outputFilename);
    
    file << createSVGHeader(width, height);
    file << createBlockBody(width, height);
    file << createBlockName(width);
    file << createEvents(width);
    file << createData(width);
    file << createSVGFooter();
    
    file.close();
    return true;
}

const FunctionBlock& FBToSVGConverter::getFunctionBlock() const {
    return fb;
}

std::string FBToSVGConverter::createSVGHeader(int width, int height) const {
    std::stringstream ss;
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    ss << "<svg width=\"" << fb.x + width + 200 << "\" height=\"" << fb.y + height + 200 << "\" ";
    ss << "xmlns=\"http://www.w3.org/2000/svg\">\n";
    ss << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
    return ss.str();
}

std::string FBToSVGConverter::createSVGFooter() const {
    return "</svg>";
}

std::string FBToSVGConverter::createBlockBody(int width, int height) const {
    std::stringstream ss;

    int inputData = 0, outputData = 0;
    
    for (const auto& data : fb.data) {
        if (data.dataType == "input") inputData++;
        else outputData++;
    }

    int dataMax = std::max(inputData, outputData);
    
    ss << "<rect x=\"" << fb.x << "\" y=\"" << fb.y << "\" width=\"" << width << "\" height=\"" << height << "\" ";
    ss << "fill=\"#ffffff\" stroke=\"#000000\" stroke-width=\"1\" rx=\"4\"/>\n";
    ss << "<line x1=\"" << fb.x - 10 << "\" y1=\"" << fb.y + 15 << "\" x2=\"" << fb.x - 10 << "\" y2=\"" << fb.y + height - 15 - (dataMax - inputData) * 20 << "\" stroke=\"#000000\" stroke-width=\"1\"/>";
    ss << "<line x1=\"" << fb.x + width + 10 << "\" y1=\"" << fb.y + 15 << "\" x2=\"" << fb.x + width + 10 << "\" y2=\"" << fb.y + height - 15 - (dataMax - outputData) * 20 << "\" stroke=\"#000000\" stroke-width=\"1\"/>";
    
    return ss.str();
}

std::string FBToSVGConverter::createBlockName(int width) const {
    std::stringstream ss;
    int centerX = width / 2;

    int inputEvents = 0, outputEvents = 0;
    
    for (const auto& event : fb.events) {
        if (event.eventType == "input") inputEvents++;
        else outputEvents++;
    }

    int centerY = std::max(inputEvents, outputEvents);
    
    ss << "<text x=\"" << fb.x + centerX << "\" y=\"" << fb.y + 20 + centerY * 20 << "\" font-family=\"Arial\" font-size=\"10\" text-anchor=\"middle\">";
    ss << fb.name << "</text>\n";
    ss << "<text x=\"" << fb.x + centerX << "\" y=\"" << fb.y + 40 + centerY * 20 << "\" font-family=\"Arial\" font-size=\"10\" text-anchor=\"middle\">";
    ss << fb.version << "</text>\n";
    
    return ss.str();
}

std::string FBToSVGConverter::createEvents(int width) const {
    std::stringstream ss;
    int y_pos = fb.y + 20;

    for (const auto& event : fb.events) {
        if (event.eventType == "input") {
            std::string points = 
                std::to_string(fb.x) + "," + std::to_string(y_pos) + " " + 
                std::to_string(fb.x + 5) + "," + std::to_string(y_pos - 5) + " " + 
                std::to_string(fb.x) + "," + std::to_string(y_pos - 10);

            ss << "<text x=\"" << fb.x + 10 << "\" y=\"" << y_pos << "\" font-family=\"Arial\" font-size=\"10\" >";
            ss << event.name << "</text>\n";
            ss << "<text x=\"" << fb.x - 25 << "\" y=\"" << y_pos << "\" ";
            ss << "font-family=\"Arial\" font-size=\"10\" text-anchor=\"end\" >";
            ss << event.comment + " — " + event.type << "</text>\n";

            ss << "<polygon points=\"" << points << "\" fill=\"lime\" stroke=\"lime\" stroke-width=\"1\"/>";
            ss << "<line x1=\"" << fb.x << "\" y1=\"" << y_pos - 5 << "\" x2=\"" << fb.x - 20 << "\" y2=\"" << y_pos - 5 << "\" stroke=\"#000000\" stroke-width=\"1\"/>";
            ss << "<rect x=\"" << fb.x - 14 << "\" y=\"" << y_pos - 9 << "\" width=\"8\" height=\"8\" ";
            ss << "fill-opacity=\"0\" stroke=\"#000000\" stroke-width=\"1\" />\n";

            y_pos += 20;
        }
    }

    y_pos = fb.y + 20;
    for (const auto& event : fb.events) {
        if (event.eventType == "output") {
            std::string points = 
                std::to_string(fb.x + width - 5) + "," + std::to_string(y_pos) + " " + 
                std::to_string(fb.x + width) + "," + std::to_string(y_pos - 5) + " " + 
                std::to_string(fb.x + width - 5) + "," + std::to_string(y_pos - 10);

            ss << "<text x=\"" << fb.x + width - 10 << "\" y=\"" << y_pos << "\" ";
            ss << "font-family=\"Arial\" font-size=\"10\" text-anchor=\"end\" >";
            ss << event.name << "</text>\n";
            ss << "<text x=\"" << fb.x + width + 25 << "\" y=\"" << y_pos << "\" ";
            ss << "font-family=\"Arial\" font-size=\"10\" >";
            ss << event.type + " — " + event.comment << "</text>\n";

            ss << "<polygon points=\"" << points << "\" fill=\"lime\" stroke=\"lime\" stroke-width=\"1\"/>";
            ss << "<line x1=\"" << fb.x + width << "\" y1=\"" << y_pos - 5 << "\" x2=\"" << fb.x + width + 20 << "\" y2=\"" << y_pos - 5 << "\" stroke=\"#000000\" stroke-width=\"1\"/>";
            ss << "<rect x=\"" << fb.x + width + 6 << "\" y=\"" << y_pos - 9 << "\" width=\"8\" height=\"8\" ";
            ss << "fill-opacity=\"0\" stroke=\"#000000\" stroke-width=\"1\" />\n";

            y_pos += 20;
        }
    }
    
    return ss.str();
}

std::string FBToSVGConverter::createData(int width) const {
    std::stringstream ss;
    
    int inputEvents = 0, outputEvents = 0;
    
    for (const auto& event : fb.events) {
        if (event.eventType == "input") inputEvents++;
        else outputEvents++;
    }

    int y_pos = fb.y + std::max(inputEvents, outputEvents) * 20 + 60;
    
    for (const auto& data : fb.data) {
        if (data.dataType == "input") {
            std::string points = 
                std::to_string(fb.x) + "," + std::to_string(y_pos) + " " + 
                std::to_string(fb.x + 5) + "," + std::to_string(y_pos - 5) + " " + 
                std::to_string(fb.x) + "," + std::to_string(y_pos - 10);

            ss << "<text x=\"" << fb.x + 10 << "\" y=\"" << y_pos << "\" ";
            ss << "font-family=\"Arial\" font-size=\"10\">";
            ss << data.name << "</text>\n";
            ss << "<text x=\"" << fb.x - 25 << "\" y=\"" << y_pos << "\" ";
            ss << "font-family=\"Arial\" font-size=\"10\" text-anchor=\"end\" >";
            ss << data.comment + " — " + data.type << "</text>\n";

            ss << "<polygon points=\"" << points << "\" fill=\"blue\" stroke=\"blue\" stroke-width=\"1\"/>";
            ss << "<line x1=\"" << fb.x << "\" y1=\"" << y_pos - 5 << "\" x2=\"" << fb.x - 20 << "\" y2=\"" << y_pos - 5 << "\" stroke=\"#000000\" stroke-width=\"1\"/>";
            ss << "<rect x=\"" << fb.x - 14 << "\" y=\"" << y_pos - 9 << "\" width=\"8\" height=\"8\" ";
            ss << "fill-opacity=\"0\" stroke=\"#000000\" stroke-width=\"1\" />\n";

            y_pos += 20;
        }
    }

    y_pos = fb.y + std::max(inputEvents, outputEvents) * 20 + 60;
    for (const auto& data : fb.data) {
        if (data.dataType == "output") {
            std::string points = 
                std::to_string(fb.x + width - 5) + "," + std::to_string(y_pos) + " " + 
                std::to_string(fb.x + width) + "," + std::to_string(y_pos - 5) + " " + 
                std::to_string(fb.x + width - 5) + "," + std::to_string(y_pos - 10);

            ss << "<text x=\"" << fb.x + width - 10 << "\" y=\"" << y_pos << "\" ";
            ss << "font-family=\"Arial\" font-size=\"10\" text-anchor=\"end\">";
            ss << data.name << "</text>\n";
            ss << "<text x=\"" << fb.x + width + 25 << "\" y=\"" << y_pos << "\" ";
            ss << "font-family=\"Arial\" font-size=\"10\" >";
            ss << data.type + " — " + data.comment << "</text>\n";

            ss << "<polygon points=\"" << points << "\" fill=\"blue\" stroke=\"blue\" stroke-width=\"1\"/>";
            ss << "<line x1=\"" << fb.x + width << "\" y1=\"" << y_pos - 5 << "\" x2=\"" << fb.x + width + 20 << "\" y2=\"" << y_pos - 5 << "\" stroke=\"#000000\" stroke-width=\"1\"/>";
            ss << "<rect x=\"" << fb.x + width + 6 << "\" y=\"" << y_pos - 9 << "\" width=\"8\" height=\"8\" ";
            ss << "fill-opacity=\"0\" stroke=\"#000000\" stroke-width=\"1\" />\n";

            y_pos += 20;
        }
    }
    
    return ss.str();
}

void FBToSVGConverter::calculateDimensions(int& width, int& height) const {
    int inputEvents = 0, outputEvents = 0;
    int inputData = 0, outputData = 0;
    
    for (const auto& event : fb.events) {
        if (event.eventType == "input") inputEvents++;
        else outputEvents++;
    }
    
    for (const auto& data : fb.data) {
        if (data.dataType == "input") inputData++;
        else outputData++;
    }
    
    int maxElements = std::max(inputEvents + inputData, outputEvents + outputData);
    height = maxElements * 20 + 50;
    
    int maxNameLength = 0;
    
    for (const auto& event : fb.events) {
        int nameLength = event.name.length();
        if (nameLength > maxNameLength) {
            maxNameLength = nameLength;
        }
    }
    
    for (const auto& data : fb.data) {
        int nameLength = data.name.length();
        if (nameLength > maxNameLength) {
            maxNameLength = nameLength;
        }
    }
    
    width = maxNameLength * 8 + 100;

    height = std::max(height, 90);
    width = std::max(width, 100);
}

std::string FBToSVGConverter::extractXMLValue(const std::string& content, const std::string& tag) const {
    std::string startTag = tag;
    std::string endTag = ">";
    
    size_t startPos = content.find(startTag);
    if (startPos == std::string::npos) return "";
    
    // Ищем закрывающую кавычку для атрибута
    size_t quoteStart = content.find("\"", startPos);
    if (quoteStart == std::string::npos) return "";
    
    // Ищем вторую кавычку
    size_t quoteEnd = content.find("\"", quoteStart + 1);
    if (quoteEnd == std::string::npos) return "";
    
    return content.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
}

void FBToSVGConverter::parseEvents(const std::string& content, const std::string& eventsTag, const std::string& eventType) {
    std::string startTag = "<" + eventsTag + ">";
    std::string endTag = "</" + eventsTag + ">";
    
    size_t startPos = content.find(startTag);
    if (startPos == std::string::npos) return;
    
    size_t endPos = content.find(endTag, startPos);
    if (endPos == std::string::npos) return;
    
    std::string eventsSection = content.substr(startPos, endPos - startPos + endTag.length());
    
    size_t eventStart = 0;
    while ((eventStart = eventsSection.find("<Event", eventStart)) != std::string::npos) {
        size_t eventEnd = eventsSection.find("</Event>", eventStart);
        if (eventEnd == std::string::npos) break;
        
        std::string eventStr = eventsSection.substr(eventStart, eventEnd - eventStart + 8);
        
        // Извлекаем имя события
        std::string name = extractXMLValue(eventStr, "Name=");
        std::string type = extractXMLValue(eventStr, "Type=");
        std::string comment = extractXMLValue(eventStr, "Comment=");
        
        if (!name.empty()) {
            Event event(name, type, eventType, comment);
            fb.events.push_back(event);
        }
        
        eventStart = eventEnd + 8;
    }
}

void FBToSVGConverter::parseData(const std::string& content, const std::string& dataTag, const std::string& dataType) {
    std::string startTag = "<" + dataTag + ">";
    std::string endTag = "</" + dataTag + ">";
    
    size_t startPos = content.find(startTag);
    if (startPos == std::string::npos) return;
    
    size_t endPos = content.find(endTag, startPos);
    if (endPos == std::string::npos) return;
    
    std::string dataSection = content.substr(startPos, endPos - startPos + endTag.length());
    
    size_t dataStart = 0;
    while ((dataStart = dataSection.find("<VarDeclaration", dataStart)) != std::string::npos) {
        size_t dataEnd = dataSection.find("/>", dataStart);
        if (dataEnd == std::string::npos) break;
        
        std::string dataStr = dataSection.substr(dataStart, dataEnd - dataStart + 8);
        
        std::string name = extractXMLValue(dataStr, "Name=");
        std::string type = extractXMLValue(dataStr, "Type=");
        std::string comment = extractXMLValue(dataStr, "Comment=");
        
        if (!name.empty()) {
            Data data(name, type, dataType, comment);
            fb.data.push_back(data);
        }
        
        dataStart = dataEnd + 8;
    }
}

bool FBToSVGConverter::parseXML(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    std::string content;
    while (std::getline(file, line)) {
        content += line;
    }
    file.close();

    fb.x = 500;
    fb.y = 100;

    fb.events.clear();
    fb.data.clear();

    fb.name = extractXMLValue(content, "Name=");
    fb.version = extractXMLValue(content, "Version=");

    if (content.find("<BasicFB>") != std::string::npos) {
        fb.type = "BasicFB";
    } else if (content.find("<CompositeFB>") != std::string::npos) {
        fb.type = "CompositeFB";
    } else if (content.find("<ServiceInterfaceFB>") != std::string::npos) {
        fb.type = "ServiceInterfaceFB";
    } else if (content.find("<SimpleFB>") != std::string::npos) {
        fb.type = "SimpleFB";
    } else {
        fb.type = "BasicFB";
    }

    parseEvents(content, "EventInputs", "input");
    parseEvents(content, "EventOutputs", "output");

    parseData(content, "InputVars", "input");
    parseData(content, "OutputVars", "output");

    return true;
}