#ifndef MATHS_FUNCTIONS_H
#define MATHS_FUNCTIONS_H

namespace MathsFunctions {

double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b);
#ifdef ENABLE_ADVANCED_OPERATIONS
    double squareRoot(double value);
    double power(double base, double exponent);
#endif // ENABLE_ADVANCED_OPERATIONS

} // namespace MathsFunctions

#endif // MATHS_FUNCTIONS_H
