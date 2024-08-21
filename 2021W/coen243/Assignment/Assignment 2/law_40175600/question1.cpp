// Assignment 2, Question 1, Week 5
// Hei Wang Andre Law, 4017 5600
// Program: Temperature conversion from 1-50 Celsius to Fahrenheit and Kelvin

#include <iostream>
#include <iomanip>
using namespace std;

// main body of the program
int main() {
	double kel, fah; // kelvin and fahrenheit variables
	const float kelConst = 273.15; // absolute zero kelvin constant

	// top header layout of the three temperatures with corresponding spacing between
	cout << setw (9) << "Celsius" << setw(15) << "Farhrenheit" << setw(10) << "Kelvin" << endl;

	// for-loop that repeats body code 50 times
	for (double cel = 1; cel <= 50; cel++)
	{
		fah = (cel * 9 / 5) + 32; // celsius to fahrenheit equation
		kel = cel + kelConst; // celsius to kelvin equation

		// 3 version of same output, but different 'setw' to clean up output format
		if (cel <= 9){
			cout << setw(3) << setprecision(0) << cel << fixed; // celsius. 0 decimal place
			cout << setw(14) << setprecision (1) << fah; // fahrenheit, 1 decimal palce
			cout << setw(17) << setprecision(2) << kel << endl; // kelvin, 2 decimal place
		}else if (cel <=37) {
			cout << setw(4) << setprecision(0) << cel << fixed; // celsius, 0 decimal place
			cout << setw(13) << setprecision(1) << fah; // fahrenheit, 1 decimal palce
			cout << setw(17) << setprecision(2) << kel << endl; // kelvin, 2 decimal place
		}
		else {
			cout << setw(4) << setprecision(0) << cel << fixed; // celsius, 0 decimal place
			cout << setw(14) << setprecision(1) << fah; // fahrenheit, 1 decimal palce
			cout << setw(16) << setprecision(2) << kel << endl; // kelvin, 2 decimal place
		}
	}
	return 0;
}