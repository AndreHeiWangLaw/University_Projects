// Assignment 1, Question 2, Week 3
// Hei Wang Andre Law, 4017 5600
// Program: Temperature conversion from Celsius to Fahrenheit and Kelvin

#include <iostream>
using namespace std;

int main() {
	//defined variables
	float cel, fah, kel; //celsius, fahrenheit, kelvin
	const float kelConst = 273.15; //absolute zero kelvin constant

	//ask user to input the celsius temperature
	cout << "Enter the temperature in celsius: ";
	cin >> cel;

	//celsius into fahrenheit equation
	fah = (cel * 9 / 5) + 32;
	
	//celsius into kelvin equation
	kel = cel + kelConst;

	//display the results
	cout << "\nFahrenheit: " << fah << endl;
	cout << "\nKelvin: " << kel << endl;

	return 0;
}