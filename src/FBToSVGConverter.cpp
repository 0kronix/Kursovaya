#include "FBToSVGConverter.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <tinyxml2.h>

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

    int centerY = std::max(inputEvents, outputEvents) * 20;
    
    ss << "<text x=\"" << fb.x + centerX << "\" y=\"" << fb.y + centerY + 20 << "\" font-family=\"Arial\" font-size=\"10\" text-anchor=\"middle\">";
    ss << fb.name << "</text>\n";
    ss << "<text x=\"" << fb.x + centerX << "\" y=\"" << fb.y + centerY + 40 << "\" font-family=\"Arial\" font-size=\"10\" text-anchor=\"middle\">";
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

    int eventsHeight = std::max(inputEvents, outputEvents) * 20;
    int dataHeight = std::max(inputData, outputData) * 20;

    height = eventsHeight + dataHeight + 50;
    
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

void FBToSVGConverter::parseEvents(tinyxml2::XMLElement* parent) {
    tinyxml2::XMLElement* eventInputs = parent->FirstChildElement("EventInputs");
    if (eventInputs) {
        tinyxml2::XMLElement* event = eventInputs->FirstChildElement("Event");
        while (event) {
            std::string name = event->Attribute("Name") ? event->Attribute("Name") : "";
            std::string type = event->Attribute("Type") ? event->Attribute("Type") : "";
            std::string comment = event->Attribute("Comment") ? event->Attribute("Comment") : "";
            
            fb.events.push_back(Event(name, type, "input", comment));
            
            event = event->NextSiblingElement("Event");
        }
    }

    tinyxml2::XMLElement* eventOutputs = parent->FirstChildElement("EventOutputs");
    if (eventOutputs) {
        tinyxml2::XMLElement* event = eventOutputs->FirstChildElement("Event");
        while (event) {
            std::string name = event->Attribute("Name") ? event->Attribute("Name") : "";
            std::string type = event->Attribute("Type") ? event->Attribute("Type") : "";
            std::string comment = event->Attribute("Comment") ? event->Attribute("Comment") : "";
            
            fb.events.push_back(Event(name, type, "output", comment));
            
            event = event->NextSiblingElement("Event");
        }
    }
}

void FBToSVGConverter::parseData(tinyxml2::XMLElement* parent) {
    tinyxml2::XMLElement* inputVars = parent->FirstChildElement("InputVars");
    if (inputVars) {
        tinyxml2::XMLElement* var = inputVars->FirstChildElement("VarDeclaration");
        while (var) {
            std::string name = var->Attribute("Name") ? var->Attribute("Name") : "";
            std::string type = var->Attribute("Type") ? var->Attribute("Type") : "";
            std::string comment = var->Attribute("Comment") ? var->Attribute("Comment") : "";
            
            fb.data.push_back(Data(name, type, "input", comment));
            
            var = var->NextSiblingElement("VarDeclaration");
        }
    }

    tinyxml2::XMLElement* outputVars = parent->FirstChildElement("OutputVars");
    if (outputVars) {
        tinyxml2::XMLElement* var = outputVars->FirstChildElement("VarDeclaration");
        while (var) {
            std::string name = var->Attribute("Name") ? var->Attribute("Name") : "";
            std::string type = var->Attribute("Type") ? var->Attribute("Type") : "";
            std::string comment = var->Attribute("Comment") ? var->Attribute("Comment") : "";
            
            fb.data.push_back(Data(name, type, "output", comment));
            
            var = var->NextSiblingElement("VarDeclaration");
        }
    }
}

bool FBToSVGConverter::parseXML(const std::string& filename) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        return false;
    }

    fb.x = 500;
    fb.y = 100;
    fb.events.clear();
    fb.data.clear();

    tinyxml2::XMLElement* fbType = doc.FirstChildElement("FBType");
    if (!fbType) return false;

    fb.name = fbType->Attribute("Name") ? fbType->Attribute("Name") : "";
    
    fb.version = "1.0";
    tinyxml2::XMLElement* versionInfo = fbType->FirstChildElement("VersionInfo");
    if (versionInfo && versionInfo->Attribute("Version")) {
        fb.version = versionInfo->Attribute("Version");
    }

    tinyxml2::XMLElement* interfaceList = fbType->FirstChildElement("InterfaceList");
    if (!interfaceList) {
        parseEvents(fbType);
        parseData(fbType);
    } else {
        parseEvents(interfaceList);
        parseData(interfaceList);
    }

    return true;
}