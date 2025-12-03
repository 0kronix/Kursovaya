#ifndef FBTO_SVG_CONVERTER_H
#define FBTO_SVG_CONVERTER_H

#include <string>
#include <vector>
#include <map>

struct Event {
    std::string name;
    std::string type;
    std::string eventType; // "input" или "output"
    std::string comment;
    
    Event(const std::string& n, const std::string& t, const std::string& et, const std::string& c) : 
        name(n), type(t), eventType(et), comment(c) {}
};

struct Data {
    std::string name;
    std::string type;
    std::string dataType; // "input" или "output"
    std::string comment;
    
    Data(const std::string& n, const std::string& t, const std::string& dt, const std::string& c) : 
        name(n), type(t), dataType(dt), comment(c) {}
};

struct FunctionBlock {
    int x;
    int y;
    std::string name;
    std::string type;
    std::string comment;
    std::string version;
    std::string author;
    std::string date;
    std::vector<Event> events;
    std::vector<Data> data;
};

class FBToSVGConverter {
public:
    FBToSVGConverter();
    ~FBToSVGConverter();
    
    bool loadFromXML(const std::string& filename);
    bool createSVG(const std::string& outputFilename);
    const FunctionBlock& getFunctionBlock() const;

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
    std::string extractXMLValue(const std::string& content, const std::string& tag) const;
    void parseEvents(const std::string& content, const std::string& eventsTag, const std::string& type);
    void parseData(const std::string& content, const std::string& dataTag, const std::string& dataType);
};

#endif