#include <iostream>

// prevent the addition of ["using namespace std"] to prevent namespace pollution

int main() {
    // Declare the Variable celcius, fahrenheight and temperature as "double"
    double 
     celcius,
     fahrenheight,
     temperature;

    // User input prompt
    std::cout << "Enter the temperature in Celcius: ";
    std::cin >> celcius;

    // Concersion Logic
    fahrenheight = (celcius * 9 / 5) + 32;
    std::cout << temperature << " Fahrenheit" << std::endl;
    std::cout << fahrenheight << " Fahrenheit" << std::endl;
    
    // Results of conversions
    std::cout << "Temperature in Fahrenheit: " << fahrenheight << std::endl;
    std::cout << "Temperature in Celcius: " << celcius << std::endl;
    std::cout << "Temperature in Kelvin: " << celcius + 273.15 << std::endl;
    return 0;
}