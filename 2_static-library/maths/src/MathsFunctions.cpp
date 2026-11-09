#include "MathsFunctions.h"

#include <cmath>
#include <stdexcept>

namespace MathsFunctions {

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

#ifdef ENABLE_ADVANCED_OPERATIONS
double squareRoot(double value) {
    if (value < 0) {
        throw std::invalid_argument("Square root requires a nonnegative number");
    }
    return std::sqrt(value);
}

double power(double base, double exponent) {
    if (base == 0 && exponent < 0) {
        throw std::invalid_argument("Zero cannot be raised to a negative power");
    }
    if (base < 0 && std::trunc(exponent) != exponent) {
        throw std::invalid_argument("A negative base requires an integer exponent");
    }
    return std::pow(base, exponent);
}
#endif // ENABLE_ADVANCED_OPERATIONS

} // namespace MathsFunctions
