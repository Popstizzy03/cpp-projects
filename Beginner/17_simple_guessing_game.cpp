// Beginner/17_simple_guessing_game.cpp
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(std::time(0)); // Seed the random number generator
    int secretNumber = std::rand() % 100 + 1; // Generate a number between 1 and 100
    int guess;
    int attempts = 0;

    std::cout << "Guess the number between 1 and 100: ";

    do {
        std::cin >> guess;
        attempts++;

        if (guess > secretNumber) {
            std::cout << "Too high! Try again: ";
        } else if (guess < secretNumber) {
            std::cout << "Too low! Try again: ";
        } else {
            std::cout << "Congratulations! You guessed the number in " << attempts << " attempts." << std::endl;
        }
    } while (guess != secretNumber);

    return 0;
}