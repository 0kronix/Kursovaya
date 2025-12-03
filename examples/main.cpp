#include <iostream>
#include "FBToSVGConverter.h"

int main() {
    std::cout << "=== FB to SVG Converter ===" << std::endl;
    
    // Создаем экземпляр конвертера
    FBToSVGConverter converter;
    
    // Пытаемся загрузить XML
    std::string xmlFile = "function_block.xml";
    if (converter.loadFromXML(xmlFile)) {
        std::cout << "✓ XML файл загружен успешно" << std::endl;
        
        // Создаем SVG
        std::string svgFile = "function_block.svg";
        if (converter.createSVG(svgFile)) {
            std::cout << "✓ SVG файл создан успешно: " << svgFile << std::endl;
        } else {
            std::cerr << "✗ Ошибка при создании SVG файла" << std::endl;
            return 1;
        }
    } else {
        std::cerr << "✗ Ошибка при загрузке XML файла" << std::endl;
        return 1;
    }
    
    std::cout << "=== Программа завершена успешно ===" << std::endl;
    return 0;
}