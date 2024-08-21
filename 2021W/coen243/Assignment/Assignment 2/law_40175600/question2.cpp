// Assignment 2, Question 2, Week 5
// Hei Wang Andre Law, 4017 5600
// Program: Prime number filter

#include <iostream>
#include <cmath>
using namespace std;

// main body of program
int main() {
	int num; // user input integer
	bool isNotPrime = true; // true means it is indeed NOT prime

	// ask user to input an integer
	cout << "Enter an integer: ";
	cin >> num; // store that integer into 'num'

	// anything below 1 isn't a prime number, continue otherwise
	if (num > 1){
		isNotPrime = false; // set integer being prime and prove otherwise

		// prime only 2 factors, themselves and 1, so don't loop unless i < num
		for (double i = 2; i < num; ++i) {
			// check if exists WHOLE divisible factor
			if (floor(num / i) == num / i) {
				isNotPrime = true; // set integer being NOT prime
				break; // break loop because there is a divisible factor
			}		
		}
	}	
	// display results based on the boolean
	if (isNotPrime) {
		cout << endl << num << " is NOT a prime number." << endl;
	}
	else {
		cout << endl << num << " is a prime number." << endl;
	}
	return 0;
}