#ifndef PHYSICSFUNCTIONS_H
#define PHYSICSFUNCTIONS_H

namespace Physics {
    // Handle import/export for cross-platform shared library support
    #if defined(_WIN32) || defined(_WIN64)
        #ifdef PHYSICS_FUNCTIONS_EXPORTS
            #define PHYSICS_API __declspec(dllexport)
        #else
            #define PHYSICS_API __declspec(dllimport)
        #endif
    #elif defined(__linux__) || defined(__APPLE__)
        #ifdef PHYSICS_FUNCTIONS_EXPORTS
            #define PHYSICS_API __attribute__((visibility("default")))
        #else
            #define PHYSICS_API
        #endif
    #else
        #define PHYSICS_API
    #endif

    // Function to calculate force given mass and acceleration (F = m * a)
    PHYSICS_API double calculateForce(double mass, double acceleration);

    // Function to calculate kinetic energy given mass and velocity (KE = 0.5 * m * v^2)
    PHYSICS_API double calculateKineticEnergy(double mass, double velocity);

    // Function to calculate potential energy given mass, gravity, and height (PE = m * g * h)
    PHYSICS_API double calculatePotentialEnergy(double mass, double gravity, double height);

} // namespace Physics

#endif // PHYSICSFUNCTIONS_H