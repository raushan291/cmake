#include "PhysicsFunctions.h"

namespace Physics {
    double calculateForce(double mass, double acceleration) {
        return mass * acceleration; // F = m * a
    }

    double calculateKineticEnergy(double mass, double velocity) {
        return 0.5 * mass * velocity * velocity; // KE = 0.5 * m * v^2
    }

    double calculatePotentialEnergy(double mass, double gravity, double height) {
        return mass * gravity * height; // PE = m * g * h
    }
} // namespace Physics
