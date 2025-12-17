#ifndef FBTO_SVG_CONVERTER_H
#define FBTO_SVG_CONVERTER_H

#include <string>
#include <vector>

namespace tinyxml2 {
    class XMLDocument;
    class XMLElement;
}

struct Event {
    std::string name;
    std::string type;
    std::string eventType;
    std::string comment;
    
    Event(const std::string& n, const std::string& t, 
          const std::string& et, const std::string& c);
    
    ~Event() = default;
    Event(const Event&) = default;
    Event& operator=(const Event&) = default;
};

struct Data {
    std::string name;
    std::string type;
    std::string dataType;
    std::string comment;
    
    Data(const std::string& n, const std::string& t, 
         const std::string& dt, const std::string& c);
    
    ~Data() = default;
    Data(const Data&) = default;
    Data& operator=(const Data&) = default;
};

struct FunctionBlock {
    int x;
    int y;
    std::string name;
    std::string version;
    std::vector<Event> events;
    std::vector<Data> data;
    
    ~FunctionBlock() = default;
    FunctionBlock(const FunctionBlock&) = default;
    FunctionBlock& operator=(const FunctionBlock&) = default;
};

class FBToSVGConverter {
public:
    ~FBToSVGConverter() = default;
    FBToSVGConverter(const FBToSVGConverter&) = default;
    FBToSVGConverter& operator=(const FBToSVGConverter&) = default;
    
    bool loadFromXML(const std::string& filename);
    bool createSVG(const std::string& outputFilename);

private:
    FunctionBlock fb;
    
    std::string createSVGHeader(int width, int height) const;
    std::string createSVGFooter() const;
    std::string createBlockBody(int width, int height) const;
    std::string createEvents(int width) const;
    std::string createData(int width) const;
    std::string createBlockName(int width) const;
    void calculateDimensions(int& width, int& height) const;

    bool parseXML(const std::string& filename);
    void parseEvents(tinyxml2::XMLElement* fbType);
    void parseData(tinyxml2::XMLElement* fbType);
};

#endif