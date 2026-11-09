#include "MathsFunctions.h"

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace {

double readNumber(const char* prompt) {
    std::cout << prompt;
    double value;
    if (!(std::cin >> value) || !std::isfinite(value)) {
        throw std::invalid_argument("Enter a finite number");
    }
    return value;
}

} // namespace

int main() {
    std::cout << "1. Add\n"
                 "2. Subtract\n"
                 "3. Multiply\n"
                 "4. Divide\n"
                #ifdef ENABLE_ADVANCED_OPERATIONS
                 "5. Square root\n"
                 "6. Power\n"
                #endif // ENABLE_ADVANCED_OPERATIONS
                ;

    try {
        #ifdef ENABLE_ADVANCED_OPERATIONS
            const double choice = readNumber("Choose an operation (1-6): ");
        #else
            const double choice = readNumber("Choose an operation (1-4): ");
        #endif // ENABLE_ADVANCED_OPERATIONS
        if (choice < 1 || choice > 6 || std::trunc(choice) != choice) {
            throw std::invalid_argument("Choose an operation from 1 to 6");
        }

        const double first = readNumber("Enter first number: ");
        double result;
        #ifdef ENABLE_ADVANCED_OPERATIONS
        if (choice == 5) {
            result = MathsFunctions::squareRoot(first);
        } else
        #endif // ENABLE_ADVANCED_OPERATIONS
        {
            const double second = readNumber("Enter second number: ");
            switch (static_cast<int>(choice)) {
                case 1: result = MathsFunctions::add(first, second); break;
                case 2: result = MathsFunctions::subtract(first, second); break;
                case 3: result = MathsFunctions::multiply(first, second); break;
                case 4: result = MathsFunctions::divide(first, second); break;
                #ifdef ENABLE_ADVANCED_OPERATIONS
                case 6: result = MathsFunctions::power(first, second); break;
                #endif // ENABLE_ADVANCED_OPERATIONS
                default: throw std::invalid_argument("Invalid operation");
            }
        }

        if (!std::isfinite(result)) {
            throw std::overflow_error("Result is outside the finite number range");
        }
        std::cout << "Result: " << result << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
