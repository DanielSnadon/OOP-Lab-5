#include <iostream>
#include "spisok.h"
#include "square.h"

int main() {
    std::cout << "Тестируем Spisok..." << std::endl;
    
    Spisok spisok(5);
    
    Square* square1 = new Square();
    std::cout << "Введите 4 точки квадрата: ";
    std::cin >> *square1;
    
    spisok.addFigure(square1);

    Square* square2 = new Square();
    std::cout << "Введите 4 точки квадрата: ";
    std::cin >> *square2;
    
    spisok.addFigure(square2);

    if (square1 == square2) {
        std::cout << "Равны";
    }
    return 0;
}