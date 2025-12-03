#include <stdio.h>
void calculateFuel(int fuel, int consumption, int recharge, 
                   int solarBonus, int planet, int totalPlanets)
{
    // BASE CASES
    if (fuel <= 0) {
        printf("Mission Failed! Fuel exhausted before reaching all planets.\n");
        return;
    }
    if (planet > totalPlanets) {
        printf("Mission Successful! Spacecraft completed its journey.\n");
        return;
    }
    // FUEL CONSUMPTION AT EACH PLANET
    fuel = fuel - consumption;
    // Gravitational assist fuel regeneration (for example: every 3rd planet)
    if (planet % 3 == 0) {
        fuel += recharge; 
    }
    // Solar recharge after every 4th planetary stop
    if (planet % 4 == 0) {
        fuel += solarBonus;
    }
    // Avoid negative fuel printing
    if (fuel < 0) fuel = 0;
    // Print status of current planet
    printf("Planet %d: Fuel Remaining = %d\n", planet, fuel);
    // RECURSIVE CALL for next planet
    calculateFuel(fuel, consumption, recharge, solarBonus, planet + 1, totalPlanets);
}
int main() {
    int initialFuel = 500;      // Starting fuel
    int consumption  = 70;      // Fuel consumed at each planet
    int recharge     = 30;      // Gravitational assist gain
    int solarBonus   = 50;      // Fuel gained after every 4 planets
    int totalPlanets = 10;      // Total planets in journey
    calculateFuel(initialFuel, consumption, recharge, solarBonus, 1, totalPlanets);
    return 0;
}
