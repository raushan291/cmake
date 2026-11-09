#include "PhysicsFunctions.h"

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
    std::cout << "1. Force\n"
                 "2. Kinetic energy\n"
                 "3. Potential energy\n";

    try {
        const double choice = readNumber("Choose an operation (1-3): ");
        if (choice < 1 || choice > 3 || std::trunc(choice) != choice) {
            throw std::invalid_argument("Choose an operation from 1 to 3");
        }

        const double mass = readNumber("Enter mass (kg): ");
        double result;
        const char* unit;

        switch (static_cast<int>(choice)) {
            case 1: {
                const double acceleration =
                    readNumber("Enter acceleration (m/s^2): ");
                result = Physics::calculateForce(mass, acceleration);
                unit = "N";
                break;
            }

            case 2: {
                const double velocity = readNumber("Enter velocity (m/s): ");
                result = Physics::calculateKineticEnergy(mass, velocity);
                unit = "J";
                break;
            }

            case 3: {
                const double gravity = readNumber("Enter gravity (m/s^2): ");
                const double height = readNumber("Enter height (m): ");
                result = Physics::calculatePotentialEnergy(mass, gravity, height);
                unit = "J";
                break;
            }

            default:
                throw std::invalid_argument("Invalid operation");
        }

        if (!std::isfinite(result)) {
            throw std::overflow_error(
                "Result is outside the finite number range"
            );
        }

        std::cout << "Result: " << result << ' ' << unit << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}