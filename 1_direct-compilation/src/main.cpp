#include <iostream>
#include "Calculator.h"

int main() {
    Calculator calc;
    double num1, num2;
    char operation;
    std::cout << "Enter first number: ";
    std::cin >> num1;
    std::cout << "Enter an operation (+, -, *, /): ";
    std::cin >> operation;
    std::cout << "Enter second number: ";
    std::cin >> num2;

    try{
        double result;

        switch (operation) {
            case '+':
                result = calc.add(num1, num2);
                break;
            case '-':
                result = calc.subtract(num1, num2);
                break;
            case '*':
                result = calc.multiply(num1, num2);
                break;
            case '/':
                result = calc.divide(num1, num2);
                break;
            default:
                std::cout << "Error: Invalid operation!" << std::endl;
                return 1;
        }
        std::cout << "Result: " << result << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
