#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(time(0));
    int secretNumber = rand() % 100 + 1;
    int guess, attempts = 0;
    const int maxAttempts = 7;
    
    cout << "Welcome to the Number Guessing Game!" << endl;
    cout << "I'm thinking of a number between 1 and 100." << endl;
    cout << "You have " << maxAttempts << " attempts to guess it!" << endl;
    
    while (attempts < maxAttempts) {
        cout << "\nAttempt " << (attempts + 1) << ": Enter your guess: ";
        cin >> guess;
        attempts++;
        
        if (guess == secretNumber) {
            cout << "Congratulations! You guessed it in " << attempts << " attempts!" << endl;
            break;
        } else if (guess < secretNumber) {
            cout << "Too low! Try a higher number." << endl;
        } else {
            cout << "Too high! Try a lower number." << endl;
        }
        
        if (attempts == maxAttempts) {
            cout << "Sorry! You've used all " << maxAttempts << " attempts." << endl;
            cout << "The number was: " << secretNumber << endl;
        }
    }
    
    return 0;
}